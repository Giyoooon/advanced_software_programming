#include "cuda_runtime.h"
#include "device_launch_parameters.h"

#include <stdio.h>
#include<stdio.h>
#include<stdlib.h>
#include <math.h>
#include <Windows.h>
#include <time.h>
#include <assert.h>

#define CUDA_CALL(x) { const cudaError_t a = (x); if(a != cudaSuccess) { printf("\nCuda Error: %s (err_num=%d) at line:%d\n", cudaGetErrorString(a), a, __LINE__); cudaDeviceReset(); assert(0);}}
typedef float TIMER_T;
#define USE_CPU_TIMER 1
#define USE_GPU_TIMER 1

#define	IN
#define OUT
#define INOUT

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

#define N_EQUATIONS (1 << 26)	// 전체 데이터 사이즈

#define BLOCK_SIZE (1 << 6)	// CUDA 커널 thread block 사이즈
#define BLOCK_WIDTH (1 << 3)
#define BLOCK_HEIGHT (BLOCK_SIZE / BLOCK_WIDTH)

#define N_ITERATION (1 << 0)	// 실험 반복 횟수

#define FLOAT_ERR 0.01	// 실험 결과 0 판별용

TIMER_T compute_time = 0;
TIMER_T device_time = 0;

int N;

float* A;
float* B;
float* C;
float* X0_CPU;
float* X0_GPU;
float* X1_CPU;
float* X1_GPU;
float* FX0_CPU;
float* FX0_GPU;
float* FX1_CPU;
float* FX1_GPU;

cudaError_t find_roots_GPU(float* A, float* B, float* C,
	float* X0, float* X1, float* FX0, float* FX1, int n);

__global__ void find_roots_Kernel(float* A, float* B, float* C,
	float* X0, float* X1, float* FX0, float* FX1) {
	int row, col, i;
	float a, b, c, d, x0, x1, tmp;
	row = blockDim.y * blockIdx.y + threadIdx.y;
	col = blockDim.x * blockIdx.x + threadIdx.x;
	i = gridDim.x * blockDim.x * row + col;
	a = A[i]; b = B[i]; c = C[i];
	d = sqrtf(b * b - 4.0f * a * c);
	tmp = 1.0f / (2.0f * a);
	X0[i] = x0 = (-b - d) * tmp;
	X1[i] = x1 = (-b + d) * tmp;
	FX0[i] = (a * x0 + b) * x0 + c;
	FX1[i] = (a * x1 + b) * x1 + c;
}

void find_roots_CPU(float* A, float* B, float* C,
	float* X0, float* X1, float* FX0, float* FX1, int n) {
	int i;
	float a, b, c, d, x0, x1, tmp;
	for (i = 0; i < n; i++) {
		a = A[i]; b = B[i]; c = C[i];
		d = sqrtf(b * b - 4.0f * a * c);
		tmp = 1.0f / (2.0f * a);
		X0[i] = x0 = (-b - d) * tmp;
		X1[i] = x1 = (-b + d) * tmp;
		FX0[i] = (a * x0 + b) * x0 + c;
		FX1[i] = (a * x1 + b) * x1 + c;
	}
}

void read_bin_file() {
	printf("**Binary File Read Create..\n");
	//A.bin B.bin 파일을 읽는다
	// 첫 4bytes는 전체 데이터 개수, 다음 4bytes는 Nf의 크기, 그 이후 N개의 int형 데이터저장
	//데이터는 [-100, 100] 구간의 정수
	FILE* fp = fopen("A.bin", "rb");
	A = (float*)malloc(N * sizeof(float));
	fread(A, sizeof(float), N, fp);
	fclose(fp);

	fp = fopen("B.bin", "rb");
	B = (float*)malloc(N * sizeof(float));
	fread(B, sizeof(float), N, fp);
	fclose(fp);

	fp = fopen("C.bin", "rb");
	C = (float*)malloc(N * sizeof(float));
	fread(C, sizeof(float), N, fp);
	fclose(fp);
	printf("**Binary File Read Complete!\n\n");
}

void init_bin_file(IN int n) {
	printf("**Binary File Create Create..\n");
	srand((unsigned)time(NULL));
	FILE* fp1 = fopen("A.bin", "wb");
	FILE* fp2 = fopen("B.bin", "wb");
	FILE* fp3 = fopen("C.bin", "wb");

	int i;
	float a, b, c;
	for (i = 0; i < n; i++) {
		while (1) {
			a = (float)((float)rand() / RAND_MAX * 200.0 - 100.0);
			b = (float)((float)rand() / RAND_MAX * 200.0 - 100.0);
			c = (float)((float)rand() / RAND_MAX * 200.0 - 100.0);
			if (b * b > 4.0 * a * c + 0.01 && b * b - 4.0 * a * c > 10.0)
				break;
		}
		fwrite(&a, sizeof(float), 1, fp1);
		fwrite(&b, sizeof(float), 1, fp2);
		fwrite(&c, sizeof(float), 1, fp3);
	}
	fclose(fp1);
	fclose(fp2);
	fclose(fp3);
	printf("**Binary File Create Complete!\n\n");
}
void save_ans(float* X0, float* X1, float* FX0, float* FX1, int n) {
	printf("**Answer Save Create..\n");
	FILE* fp = fopen("X0.bin", "wb");
	for (int i = 0; i < n; i++) {
		fwrite(&X0[i], sizeof(float), 1, fp);
	}
	fclose(fp);

	fp = fopen("X1.bin", "wb");
	for (int i = 0; i < n; i++) {
		fwrite(&X1[i], sizeof(float), 1, fp);
	}
	fclose(fp);

	fp = fopen("FX0.bin", "wb");
	for (int i = 0; i < n; i++) {
		fwrite(&FX0[i], sizeof(float), 1, fp);
	}
	fclose(fp);

	fp = fopen("FX1.bin", "wb");
	for (int i = 0; i < n; i++) {
		fwrite(&FX1[i], sizeof(float), 1, fp);
	}
	fclose(fp);
	printf("**Answer Save Complete!\n\n");
}
int main()
{
	int i;
	N = N_EQUATIONS;
	init_bin_file(N);
	read_bin_file();

	TIMER_T CPU_time = 0.0f, GPU_time_NO_SHARED = 0.0f;
	X0_CPU = (float*)malloc(N * sizeof(float));
	X0_GPU = (float*)malloc(N * sizeof(float));
	X1_CPU = (float*)malloc(N * sizeof(float));
	X1_GPU = (float*)malloc(N * sizeof(float));
	FX0_CPU = (float*)malloc(N * sizeof(float));
	FX0_GPU = (float*)malloc(N * sizeof(float));
	FX1_CPU = (float*)malloc(N * sizeof(float));
	FX1_GPU = (float*)malloc(N * sizeof(float));
	for (i = 0; i < N_ITERATION; i++) {
		CHECK_TIME_START;
		find_roots_CPU(A, B, C, X0_CPU, X1_CPU, FX0_CPU, FX1_CPU, N);
		CHECK_TIME_END(compute_time);
		CPU_time += compute_time;
		find_roots_GPU(A, B, C, X0_GPU, X1_GPU, FX0_GPU, FX1_GPU, N);
		GPU_time_NO_SHARED += device_time;
	}
	
	for (i = 0; i < N; i++) {
		if (X0_CPU[i] - X0_GPU[i] > FLOAT_ERR) {
			printf("%d : CPU : %d,\tGPU : %d\n", i, X0_CPU[i], X0_GPU[i]);
			break;
		}
	}
	if (i == N)
		printf("**CPU answer and Kernel answer same\n\n");
	else
		printf("**CPU answer and Kernel answer different!!\n\n");

	printf("**N_EQUATIONS : %d BLOCK_SIZE : %d\n", N_EQUATIONS, BLOCK_SIZE);
	printf("**CPU compute time : %f ms\n", CPU_time / N_ITERATION);
	printf("**GPU NO SHARED compute time : %f ms\n", GPU_time_NO_SHARED / N_ITERATION);
	save_ans(X0_GPU, X1_GPU, FX0_GPU, FX1_GPU, N);
	free(A);
	free(B);
	free(C);
	free(X0_CPU);
	free(X0_GPU);
	free(X1_CPU);
	free(X1_GPU);
	free(FX0_CPU);
	free(FX0_GPU);
	free(FX1_CPU);
	free(FX1_GPU);

	return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	
//	커널을 실행하기 전 필요한 자료들 준비 및 커널을 실행할 디바이스를 설정
//	
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

cudaError_t find_roots_GPU(float* A, float* B, float* C,
	float* X0, float* X1, float* FX0, float* FX1, int n) {
	//커널 실행전 필요 자료 준비 밎 커널 실행
	cudaError_t cudaStatus;
	CHECK_TIME_INIT_GPU();
	cudaStatus = cudaSetDevice(0);
	if (cudaStatus != cudaSuccess) {
		fprintf(stderr, "cudaSetDevice failed!  Do you have a CUDA-capable GPU installed?");
		return cudaStatus;
	}
	float* _A;
	float* _B;
	float* _C;
	float* _X0;
	float* _X1;
	float* _FX0;
	float* _FX1;
	size_t size;
	size = N * sizeof(float);
	CUDA_CALL(cudaMalloc(&_A, size))
	CUDA_CALL(cudaMemcpy(_A, A, size, cudaMemcpyHostToDevice))
	CUDA_CALL(cudaMalloc(&_B, size))
	CUDA_CALL(cudaMemcpy(_B, B, size, cudaMemcpyHostToDevice))
	CUDA_CALL(cudaMalloc(&_C, size))
	CUDA_CALL(cudaMemcpy(_C, C, size, cudaMemcpyHostToDevice))
	CUDA_CALL(cudaMalloc(&_X0, size))
	CUDA_CALL(cudaMalloc(&_X1, size))
	CUDA_CALL(cudaMalloc(&_FX0, size))
	CUDA_CALL(cudaMalloc(&_FX1, size))
	dim3 dimBlock(BLOCK_WIDTH, BLOCK_HEIGHT);
	dim3 dimGrid((1 << 13) / BLOCK_WIDTH, (1 << 13) / BLOCK_HEIGHT);
	CHECK_TIME_START_GPU()
	find_roots_Kernel << < dimGrid, dimBlock >> > (_A, _B, _C, _X0, _X1, _FX0, _FX1);
	CHECK_TIME_END_GPU(device_time)
	CUDA_CALL(cudaGetLastError())
	CUDA_CALL(cudaDeviceSynchronize())
	CUDA_CALL(cudaMemcpy(X0, _X0, size, cudaMemcpyDeviceToHost))
	CUDA_CALL(cudaMemcpy(X1, _X1, size, cudaMemcpyDeviceToHost))
	CUDA_CALL(cudaMemcpy(FX0, _FX0, size, cudaMemcpyDeviceToHost))
	CUDA_CALL(cudaMemcpy(FX1, _FX1, size, cudaMemcpyDeviceToHost))
	cudaFree(_A);
	cudaFree(_B);
	cudaFree(_C);
	cudaFree(_X0);
	cudaFree(_X1);
	cudaFree(_FX0);
	cudaFree(_FX1);
	////
	
	return cudaStatus;
}
