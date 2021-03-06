
#include "cuda_runtime.h"
#include "device_launch_parameters.h"

#include <stdio.h>
#include<stdio.h>
#include<stdlib.h>
#include <math.h>
#include <Windows.h>
#include <time.h>
#include <assert.h>

//1 prac 2
//2 prac 3
#define prac 2


#define CUDA_CALL(x) { const cudaError_t a = (x); if(a != cudaSuccess) { printf("\nCuda Error: %s (err_num=%d) at line:%d\n", cudaGetErrorString(a), a, __LINE__); cudaDeviceReset(); assert(0);}}
typedef float TIMER_T;
#define USE_CPU_TIMER 1
#define USE_GPU_TIMER 1
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

// CUDA event 객체를 사용하여 커널 실행시간 측정
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

#if prac==1



typedef struct {
	int width;
	int height;
	float *elements;
} Array;


#define MAX_N_ELEMENTS	(1 << 20)

void generate_random_float_array(float *array, int n) {

	int i;

	for (i = 0; i < n; i++) {
		array[i] = 3.1415926f*((float)rand() / RAND_MAX);
	}
}
void combine_two_arrays(float *x, float *y, float *z, int n) {
	int i;

	for (i = 0; i < n; i++) {
		z[i] = 1.0f / (sin(x[i])*cos(y[i]) + cos(x[i])*sin(y[i]));
	}
}

__global__ void CombineTwoArrraysKernel(Array A, Array B, Array C) {
	int row = blockDim.y*blockIdx.y + threadIdx.y;
	int col = blockDim.x*blockIdx.x + threadIdx.x;
	int id = gridDim.x*blockDim.x*row + col;
	C.elements[id] = 1.0f / (sin(A.elements[id])*cos(B.elements[id]) + cos(A.elements[id])*sin(B.elements[id]));
	/* 
	* usless한 연산
	int temp = 0;

	for(int  i =0 ; i < 65536; i ++){
		temp++;
	}
	*/
}

cudaError_t combine_two_arrays_GPU(const Array A, const Array B, Array C);

int BLOCK_SIZE = 16;

int main()
{
	int n_elements;

	srand((unsigned int)time(NULL));
	n_elements = MAX_N_ELEMENTS;

	printf("*** Data size : %d\n\n", n_elements);

	Array A, B, C, G;
	A.width = B.width = C.width = G.width = 1024;
	A.height = B.height = C.height = G.height = MAX_N_ELEMENTS / 1024;

	A.elements = (float *)malloc(sizeof(float)*MAX_N_ELEMENTS);
	B.elements = (float *)malloc(sizeof(float)*MAX_N_ELEMENTS);
	C.elements = (float *)malloc(sizeof(float)*MAX_N_ELEMENTS);
	G.elements = (float *)malloc(sizeof(float)*MAX_N_ELEMENTS);
	generate_random_float_array(A.elements, MAX_N_ELEMENTS);
	generate_random_float_array(B.elements, MAX_N_ELEMENTS);

	CHECK_TIME_START;
	combine_two_arrays(A.elements, B.elements, C.elements, n_elements);
	CHECK_TIME_END(compute_time);

	printf("***PU C[10] = %f/ Time taken = %.6fms\n", C.elements[10], compute_time);

	CHECK_TIME_INIT_GPU();
	//CHECK_TIME_START_GPU()
	cudaError_t cudaStatus = combine_two_arrays_GPU(A, B, G);
	//CHECK_TIME_END_GPU(device_time)
	

	if (cudaStatus != cudaSuccess) {
		fprintf(stderr, "combine_two_arrays_GPU failed!");
		return 1;
	}
	printf("***GPU G[10] = %f/ Time taken = %.6fms\n", G.elements[10], device_time);
	CHECK_TIME_DEST_GPU();

	// cudaDeviceReset must be called before exiting in order for profiling and
	// tracing tools such as Nsight and Visual Profiler to show complete traces.
	cudaStatus = cudaDeviceReset();
	if (cudaStatus != cudaSuccess) {
		fprintf(stderr, "cudaDeviceReset failed!");
		return 1;
	}

	return 0;
}
cudaError_t combine_two_arrays_GPU(const Array A, const Array B, Array C) {
	/*
	CHECK_TIME_INIT_GPU()
	CHECK_TIME_START_GPU()
	CHECK_TIME_END_GPU(device_time)
	CHECK_TIME_DEST_GPU()
	*/
	cudaError_t cudaStatus;
	// Choose which GPU to run on, change this on a multi-GPU system.
	cudaStatus = cudaSetDevice(0);
	if (cudaStatus != cudaSuccess) {
		fprintf(stderr, "cudaSetDevice failed!  Do you have a CUDA-capable GPU installed?");
		goto Error;
	}/////////////  if(cu.....  ==CUDA_CALL

	cudaDeviceProp deviceProp;
	CUDA_CALL(cudaGetDeviceProperties(&deviceProp, 0));

	Array d_A, d_B, d_C;
	size_t size;

	
	
	d_A.width = A.width; d_A.height = A.height;
	size = A.width * A.height * sizeof(float);
	CUDA_CALL(cudaMalloc(&d_A.elements, size))

	CUDA_CALL(cudaMemcpy(d_A.elements, A.elements, size, cudaMemcpyHostToDevice))

	d_B.width = B.width; d_B.height = B.height;
	size = B.width * B.height * sizeof(float);
	CUDA_CALL(cudaMalloc(&d_B.elements, size))
	CUDA_CALL(cudaMemcpy(d_B.elements, B.elements, size, cudaMemcpyHostToDevice))

	d_C.width = C.width; d_C.height = C.height;
	size = C.width * C.height * sizeof(float);
	CUDA_CALL(cudaMalloc(&d_C.elements, size))

	// Assume that width and height are multiples of BLOCK SIZE.
	dim3 dimBlock(BLOCK_SIZE, BLOCK_SIZE);
	dim3 dimGrid(A.width / dimBlock.x, A.height / dimBlock.y);

	CHECK_TIME_START_GPU();
	CombineTwoArrraysKernel << < dimGrid, dimBlock >> > (d_A, d_B, d_C);
	CHECK_TIME_END_GPU(device_time);
	
	//CUDA_CALL(cudaGetLastError())

	// cudaDeviceSynchronize waits for the kernel to finish, and returns
	// any errors encountered during the launch.
	CUDA_CALL(cudaDeviceSynchronize())
	CUDA_CALL(cudaMemcpy(C.elements, d_C.elements, size, cudaMemcpyDeviceToHost))


		Error:
	cudaFree(d_A.elements);
	cudaFree(d_B.elements);
	cudaFree(d_C.elements);
	return cudaStatus;
}
#endif

#if prac==2

int n;
#define BLOCK_SIZE 8
#define ELEMENT_SIZE (1 << 10)
const int ELEM_PER_VECTOR = 32;
float(*pVecX), (*pVecY), (*pVecY_G);
float(*pMatA);
void init_MatVec(void)
{
	srand((unsigned)time(NULL));
	FILE* fp = fopen("gen.bin", "rb");
	fread(&n, sizeof(float), 1, fp);

	pVecX = new float[n * ELEM_PER_VECTOR];
	pVecY = new float[n * ELEM_PER_VECTOR];
	pVecY_G = new float[n * ELEM_PER_VECTOR];
	pMatA = new float[ELEM_PER_VECTOR * ELEM_PER_VECTOR];

	fread(pVecX, sizeof(float), n * ELEM_PER_VECTOR, fp);
	fread(pMatA, sizeof(float), ELEM_PER_VECTOR * ELEM_PER_VECTOR, fp);
	fclose(fp);
}
void Mat_Vec_Multiply()
{
	int vec_idx, i, j;

	for(vec_idx = 0; vec_idx < ELEMENT_SIZE; vec_idx++){
		for(i = 0; i < ELEM_PER_VECTOR; i++){
			float sum = 0;
			for(j = 0; j < ELEM_PER_VECTOR; j++){
				sum += pMatA[i * ELEM_PER_VECTOR + j] * pVecX[vec_idx * ELEM_PER_VECTOR + j];
			}
			pVecY[vec_idx * ELEM_PER_VECTOR + i] = sum;
		}
	}
}

__global__ void Mat_Vec_Multiply_Kernel(float *d_VecY, float *d_VecX, float *d_MatA, int Vec_Size) {	
	int row = blockDim.y * blockIdx.y + threadIdx.y;
	int column = blockDim.x * blockIdx.x + threadIdx.x;
	int idx = gridDim.x * blockDim.x * row + column;
	int i;
	float sum = 0.0;

	for (i = 0; i < Vec_Size; i++) {
		sum += d_MatA[column * Vec_Size + i] * d_VecX[row * Vec_Size + i];
	}

	d_VecY[idx] = sum;
}

void Mat_Vec_Multiply_GPU(float *p_VecX, float *p_MatA, float *p_VecY_G) {
	/*
	CHECK_TIME_INIT_GPU()
	CHECK_TIME_START_GPU()
	CHECK_TIME_END_GPU(device_time)
	CHECK_TIME_DEST_GPU()
	*/
	cudaError_t cudaStatus;
	// Choose which GPU to run on, change this on a multi-GPU system.
	cudaStatus = cudaSetDevice(0);
	if (cudaStatus != cudaSuccess) {
		fprintf(stderr, "cudaSetDevice failed!  Do you have a CUDA-capable GPU installed?");
		return;
	}/////////////  if(cu.....  ==CUDA_CALL
	/*
	cudaDeviceProp deviceProp;
	CUDA_CALL(cudaGetDeviceProperties(&deviceProp, 0));
	*/
	float* X, * Y, * Z;
	size_t size;

	

	size = ELEM_PER_VECTOR * ELEM_PER_VECTOR * sizeof(float);
	CUDA_CALL(cudaMalloc(&Z, size))

		CUDA_CALL(cudaMemcpy(Z, p_MatA, size, cudaMemcpyHostToDevice))

	size = ELEMENT_SIZE * ELEM_PER_VECTOR * sizeof(float);
	CUDA_CALL(cudaMalloc(&X, size))
		CUDA_CALL(cudaMemcpy(X, p_VecX, size, cudaMemcpyHostToDevice))

	size = ELEMENT_SIZE * ELEM_PER_VECTOR * sizeof(float);
	CUDA_CALL(cudaMalloc(&Y, size))

	// Assume that width and height are multiples of BLOCK SIZE.
	//dim3 dimBlock(BLOCK_SIZE, BLOCK_SIZE);
	dim3 dimBlock(BLOCK_SIZE, BLOCK_SIZE);
	//dim3 dimGrid(ELEM_PER_VECTOR / dimBlock.x, ELEM_PER_VECTOR / dimBlock.y);
	dim3 dimGrid(ELEM_PER_VECTOR / dimBlock.x, (n==1 ? ELEM_PER_VECTOR : ELEMENT_SIZE) / dimBlock.y);
	

	CHECK_TIME_START_GPU()
	Mat_Vec_Multiply_Kernel << < dimGrid, dimBlock >> > (Y,X,Z, ELEM_PER_VECTOR);
	CHECK_TIME_END_GPU(device_time)
	
	//CUDA_CALL(cudaGetLastError())

	// cudaDeviceSynchronize waits for the kernel to finish, and returns
	// any errors encountered during the launch.
	CUDA_CALL(cudaDeviceSynchronize())
	CUDA_CALL(cudaMemcpy(p_VecY_G, Y, size, cudaMemcpyDeviceToHost))
	


	cudaFree(X);
	cudaFree(Y);
	cudaFree(Z);
}

void init_data(int size) {
	srand((unsigned)time(NULL));
	FILE *fp = fopen("gen.bin", "wb");
	fwrite(&size, sizeof(int), 1, fp);

	int i, j;
	float x;

	for (i = 0; i < size; i++) {
		for (j = 0; j < ELEM_PER_VECTOR; j++) {
			x = 2.0f*((float)rand() / RAND_MAX) - 1.0f;
			fwrite(&x, sizeof(float), 1, fp);
		}
	}

	for (i = 0; i < ELEM_PER_VECTOR ; i++) {
		for (j = 0; j < ELEM_PER_VECTOR; j++) {
			x = 2.0f*((float)rand() / RAND_MAX) - 1.0f;
			fwrite(&x, sizeof(float), 1, fp);
		}
	}

	fclose(fp);

	return;
}

int main()
{
	init_data(ELEMENT_SIZE);
	init_MatVec();
	printf("n = %d  file open ok.\n", n);

	CHECK_TIME_START;
	Mat_Vec_Multiply();
	CHECK_TIME_END(compute_time);

	CHECK_TIME_INIT_GPU()
	printf("***CPU C[10] = %.3f/ Time taken = %.6fms\n", pVecY[0 * ELEM_PER_VECTOR + 0], compute_time);
	Mat_Vec_Multiply_GPU(pVecX, pMatA, pVecY_G);
	printf("***GPU C[10] = %.3f/ Time taken = %.6fms\n", pVecY_G[0 * ELEM_PER_VECTOR + 0], device_time);
	CHECK_TIME_DEST_GPU()
	int vec_idx, i;

	for(i = 0; i < ELEMENT_SIZE * ELEM_PER_VECTOR; i++){
		if(fabs(pVecY[i] - pVecY_G[i]) > 0.001){
			printf("Kernel execution fail!!\n\n");
			break;
		}
	}
}


#endif