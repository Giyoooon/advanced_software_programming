#pragma once

#include "cuda_code.cuh"

#include <stdio.h>
#include<stdio.h>
#include<stdlib.h>
#include <math.h>
#include <Windows.h>
#include <time.h>
#include <assert.h>

#if USE_CPU_TIMER == 1
__int64 start, freq, end;
#define CHECK_TIME_START { QueryPerformanceFrequency((LARGE_INTEGER*)&freq); QueryPerformanceCounter((LARGE_INTEGER*)&start); }
#define CHECK_TIME_END(a) { QueryPerformanceCounter((LARGE_INTEGER*)&end); a = (float)((float)(end - start) / (freq / 1000.0f)); }
#else
#define CHECK_TIME_START
#define CHECK_TIME_END(a)
#endif

#if USE_GPU_TIMER == 1
cudaEvent_t cuda_timer_start, cuda_timer_stop;
#define CUDA_STREAM_0 (0)

void create_device_timer()
{
	CUDA_CALL(cudaEventCreate(&cuda_timer_start));
	CUDA_CALL(cudaEventCreate(&cuda_timer_stop));
}

void destroy_device_timer()
{
	CUDA_CALL(cudaEventDestroy(cuda_timer_start));
	CUDA_CALL(cudaEventDestroy(cuda_timer_stop));
}

inline void start_device_timer()
{
	cudaEventRecord(cuda_timer_start, CUDA_STREAM_0);
}

inline TIMER_T stop_device_timer()
{
	TIMER_T ms;
	cudaEventRecord(cuda_timer_stop, CUDA_STREAM_0);
	cudaEventSynchronize(cuda_timer_stop);

	cudaEventElapsedTime(&ms, cuda_timer_start, cuda_timer_stop);
	return ms;
}

#define CHECK_TIME_INIT_GPU() { create_device_timer(); }
#define CHECK_TIME_START_GPU() { start_device_timer(); }
#define CHECK_TIME_END_GPU(a) { a = stop_device_timer(); }
#define CHECK_TIME_DEST_GPU() { destroy_device_timer(); }
#else
#define CHECK_TIME_INIT_GPU()
#define CHECK_TIME_START_GPU()
#define CHECK_TIME_END_GPU(a)
#define CHECK_TIME_DEST_GPU()
#endif

TIMER_T compute_time = 0;
TIMER_T device_time = 0;

#define Window 2

__constant__ float constant_gaussian_kernel[ 25 ];

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	
//	Gaussian 필터링을 하는 커널
//	shared memory를 사용하지 않는다
//	
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

__global__ void Gaussian_kernel_no_shared(IN unsigned char *d_bitmaps, OUT unsigned char *d_Gaussian, long width, long height) {	
	int x = blockDim.x * blockIdx.x + threadIdx.x;
	int y = blockDim.y * blockIdx.y + threadIdx.y;
	int tid = gridDim.x * blockDim.x * y + x;
	int i,j,k;
	double mean =0.0;

	for (int l = -2; l <= 2; l++) {
		for (int k = -2; k <= 2; k++) {
			if ((y + l) < 0 || (y + l) >= height || (x + k) < 0 || (x + k) >= width) 
				continue;
			mean += d_bitmaps[(y + l) * gridDim.x * blockDim.x + (x + k)] * constant_gaussian_kernel[(l + 2) * 5 + (k + 2)];
		}
	}
	d_Gaussian[tid] = mean;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	
//	Gaussian 필터링을 하는 커널
//	shared memory를 사용한다.
//	
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern __shared__ unsigned char sharedBuffer[];
__global__ void Gaussian_kernel_shared(INOUT unsigned char *d_bitmaps, OUT unsigned char *d_Gaussian, long width, long height) {
	int row = blockDim.y * blockIdx.y + threadIdx.y;
	int col = blockDim.x * blockIdx.x + threadIdx.x;
	int x = threadIdx.x + 2;
	int y = threadIdx.y + 2;
	int id, tid;
	if (x == 2) {
		if (col > 0) {
			id = (blockDim.x + 4) * y + (x - 1);
			tid = gridDim.x * blockDim.x * row + (col - 1);
			sharedBuffer[id] = d_bitmaps[tid];
		}
		if (col > 1) {
			id = (blockDim.x + 4) * y + (x - 2);
			tid = gridDim.x * blockDim.x * row + (col - 2);
			sharedBuffer[id] = d_bitmaps[tid];
		}
	}
	if (y == 2) {
		if (row > 0) {
			id = (blockDim.x + 4) * (y - 1) + (x);
			tid = gridDim.x * blockDim.x * (row - 1) + col;
			sharedBuffer[id] = d_bitmaps[tid];
		}
		if (row > 1) {
			id = (blockDim.x + 4) * (y - 2) + (x);
			tid = gridDim.x * blockDim.x * (row - 2) + col;
			sharedBuffer[id] = d_bitmaps[tid];
		}
	}
	if (x == 2 && y == 2) {
		for (int i = 1; i <= 2; i++) {
			for (int j = 1; j <= 2; j++) {
				if (row - i < 0) continue;
				if (col - j < 0) continue;
				id = (blockDim.x + 4) * (y - i) + (x - j);
				tid = gridDim.x * blockDim.x * (row - i) + (col - j);
				sharedBuffer[id] = d_bitmaps[tid];
			}

		}
	}
	if (x == blockDim.x + 1) {
		if (col + 1 < width) {
			id = (blockDim.x + 4) * y + (x + 1);
			tid = gridDim.x * blockDim.x * row + (col + 1);
			sharedBuffer[id] = d_bitmaps[tid];
		}
		if (col + 2 < width) {
			id = (blockDim.x + 4) * y + (x + 2);
			tid = gridDim.x * blockDim.x * row + (col + 2);
			sharedBuffer[id] = d_bitmaps[tid];
		}
	}
	if (y == blockDim.y + 1) {
		if (row + 1 < height) {
			id = (blockDim.x + 4) * (y + 1) + (x);
			tid = gridDim.x * blockDim.x * (row + 1) + (col);
			sharedBuffer[id] = d_bitmaps[tid];
		}
		if (row + 2 < height) {
			id = (blockDim.x + 4) * (y + 2) + (x);
			tid = gridDim.x * blockDim.x * (row + 2) + (col);
			sharedBuffer[id] = d_bitmaps[tid];
		}
	}
	if (x == blockDim.x + 1 && y == blockDim.y + 1) {
		for (int i = 1; i <= 2; i++) {
			for (int j = 1; j <= 2; j++) {
				if (row + i >= height) continue;
				if (col + j >= width) continue;
				id = (blockDim.x + 4) * (y + i) + (x + j);
				tid = gridDim.x * blockDim.x * (row + i) + (col + j);
				sharedBuffer[id] = d_bitmaps[tid];
			}

		}
	}
	id = (blockDim.x + 4) * y + x;
	tid = gridDim.x * blockDim.x * row + col;
	sharedBuffer[id] = d_bitmaps[tid];
	__syncthreads();
	double mean = 0;
	for (int l = -2; l <= 2; l++) {
		for (int k = -2; k <= 2; k++) {
			if ((row + l) < 0 || (row + l) >= height || (col + k) < 0 || (col + k) >= width) continue;
			mean += sharedBuffer[(y + l) * (blockDim.x + 4) + (x + k)] * constant_gaussian_kernel[(l + 2) * 5 + (k + 2)];
		}
	}
	d_Gaussian[tid] = mean;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	
//	Constant variable 인 gaussian kernel을 설정하는 함수
//	후에 gaussian filtering 에서 사용한다.
//	
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Set_Gaussain_Kernel(){
	float _1 = 1.0f / 256.0f;
	float _4 = _1 * 4;
	float _6 = _1 * 6;
	float _16 = _1 * 16;
	float _24 = _1 * 24;
	float _36 = _1 * 36;

	float *p_gaussian_kernel = new float[25];

	p_gaussian_kernel[0] = p_gaussian_kernel[4] = p_gaussian_kernel[20] = p_gaussian_kernel[24] = _1;
	p_gaussian_kernel[1] = p_gaussian_kernel[3] = p_gaussian_kernel[5] = p_gaussian_kernel[9]= _4;
	p_gaussian_kernel[15] = p_gaussian_kernel[19] = p_gaussian_kernel[21] = p_gaussian_kernel[23] = _4;
	p_gaussian_kernel[2] = p_gaussian_kernel[10] = p_gaussian_kernel[14] = p_gaussian_kernel[22] = _6;
	p_gaussian_kernel[6] = p_gaussian_kernel[8] = p_gaussian_kernel[16] = p_gaussian_kernel[18] = _16;
	p_gaussian_kernel[7] = p_gaussian_kernel[11] =p_gaussian_kernel[13] = p_gaussian_kernel[17] = _24;
	p_gaussian_kernel[12] = _36;

	cudaMemcpyToSymbol( constant_gaussian_kernel, p_gaussian_kernel, sizeof( float ) * 25 );

	delete[] p_gaussian_kernel;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	
//	커널을 실행하기 전 필요한 자료들 준비 및 커널을 실행할 디바이스를 설정
//	Shared_flag 입력 시 NO_SHARED 나 SHARED 중 한 개의 매크로를 넣으면
//	flag값에 맞는 커널을 실행
//	
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

float Do_Gaussian_on_GPU(IN unsigned char *p_bitmaps, OUT unsigned char *p_Gaussian, long width, long height, int Shared_flag)
{
	Set_Gaussain_Kernel();
	CUDA_CALL(cudaSetDevice(0));
	unsigned int total_pixel = width * height;

	unsigned char *d_bitmaps, *d_Gaussian;
	size_t mem_size;

	mem_size = width * height * sizeof(unsigned char);
	CUDA_CALL(cudaMalloc(&d_bitmaps, mem_size));

	CUDA_CALL(cudaMemcpy(d_bitmaps, p_bitmaps, mem_size, cudaMemcpyHostToDevice));

	CUDA_CALL(cudaMalloc(&d_Gaussian, mem_size));

	dim3 blockDim(1024, 1024);

	//dim3 gridDim((width + 31) / blockDim.x, (height + 31) / blockDim.y);

	dim3 gridDim((width + 1023) / blockDim.x, (height + 1023) / blockDim.y);
	CHECK_TIME_INIT_GPU();
	CHECK_TIME_START_GPU();

	switch (Shared_flag)
	{
	case NO_SHARED:
		Gaussian_kernel_no_shared << <gridDim , blockDim>> > (d_bitmaps, d_Gaussian, width, height);
		break;
	case SHARED:
		Gaussian_kernel_shared << < gridDim, blockDim, sizeof(unsigned char) * (blockDim.x + 2 * Window) * (blockDim.y + 2 * Window)>> > (d_bitmaps, d_Gaussian, width, height);
		break;
	}

	CUDA_CALL(cudaDeviceSynchronize());
	CHECK_TIME_END_GPU(device_time);
	CHECK_TIME_DEST_GPU();

	CUDA_CALL(cudaMemcpy(p_Gaussian, d_Gaussian, mem_size, cudaMemcpyDeviceToHost));
	
	cudaFree(d_bitmaps);
	cudaFree(d_Gaussian);

	return device_time;
}