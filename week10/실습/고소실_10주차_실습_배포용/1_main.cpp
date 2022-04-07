#include <stdio.h>
#include <string.h>
#include <random>
#include <time.h>

#include <math.h>
#include <time.h>
#include <Windows.h>

__int64 start, freq, end;
#define CHECK_TIME_START QueryPerformanceFrequency((LARGE_INTEGER*)&freq); QueryPerformanceCounter((LARGE_INTEGER*)&start)
#define CHECK_TIME_END(a) QueryPerformanceCounter((LARGE_INTEGER*)&end); a = (float)((float)(end - start) / (freq / 1000.0f))
float compute_time;


#define MATDIM 1024
double *pMatA, *pMatB, *pMatC;
void MultiplySquareMatrices_1(double *pDestMatrix, double *pLeftMatrix, double *pRightMatrix, int MatSize);
void MultiplySquareMatrices_2(double *pDestMatrix, double *pLeftMatrix, double *pRightMatrix, int MatSize);
void MultiplySquareMatrices_3(double *pDestMatrix, double *pLeftMatrix, double *pRightMatrix, int MatSize);
void MultiplySquareMatrices_4(double *pDestMatrix, double *pLeftMatrix, double *pRightMatrix, int MatSize);

void init_MatMat(void);

void main(void)
{
	init_MatMat();
	printf("start!\n");
	CHECK_TIME_START;
	MultiplySquareMatrices_1(pMatC, pMatA, pMatB, MATDIM);
	//Sleep(500);
	CHECK_TIME_END(compute_time);
	printf("MultiplySquareMatrices_1 : %f ms\n", compute_time);

	CHECK_TIME_START;
	MultiplySquareMatrices_2(pMatC, pMatA, pMatB, MATDIM);
	//Sleep(500);
	CHECK_TIME_END(compute_time);
	printf("MultiplySquareMatrices_2 = %f ms\n", compute_time);

	CHECK_TIME_START;
	MultiplySquareMatrices_3(pMatC, pMatA, pMatB, MATDIM);
	//Sleep(500);
	CHECK_TIME_END(compute_time);
	printf("MultiplySquareMatrices_3 = %f ms\n", compute_time);

	CHECK_TIME_START;
	MultiplySquareMatrices_4(pMatC, pMatA, pMatB, MATDIM);
	//Sleep(500);
	CHECK_TIME_END(compute_time);
	printf("MultiplySquareMatrices_4 = %f ms\n", compute_time);
}

void MultiplySquareMatrices_1(double *pDestMatrix, double *pLeftMatrix ,double *pRightMatrix, int MatSize)
{
	int i, j, k;
	for (i = 0; i < MATDIM; i++) {
		for (j = 0; j < MATDIM; j++) {
			pDestMatrix[i * MATDIM + j] = 0;
			for (k = 0; k < MATDIM; k++) {
				//printf("i : %d j : %d k : %d\n", i, j, k);
				pDestMatrix[i * MATDIM + j] += pLeftMatrix[i * MATDIM + k] * pRightMatrix[k * MATDIM + j];
			}
		}
	}
}

void MultiplySquareMatrices_2(double *pDestMatrix, double *pLeftMatrix, double *pRightMatrix, int MatSize)
{
	int i, j, k, tmp, lin_idx;
	// pRight transpose
	for (i = 0; i < MATDIM; i++) {
		for (j = 0; j < MATDIM; j++) {
			tmp = pRightMatrix[i * MATDIM + j];
			pRightMatrix[i * MATDIM + j] = pRightMatrix[j * MATDIM + i];
			pRightMatrix[j * MATDIM + i] = tmp;
		}
	}
	//pDest[lin_idx] = pLeftik * (pRightkj)^t
	for (i = 0; i < MATDIM; i++) {
		for (j = 0; j < MATDIM; j++) {
			lin_idx = i * MATDIM + j;
			pDestMatrix[lin_idx] = 0;
			for (k = 0; k < MATDIM; k++) {
				pDestMatrix[lin_idx] += pLeftMatrix[i * MATDIM + k] * pRightMatrix[i * MATDIM + k];
			}
		}
	}
}

void MultiplySquareMatrices_3(double *pDestMatrix, double *pLeftMatrix, double *pRightMatrix, int MatSize)
{
	int i, j, k, tmp, idx1, idx2;
	// pRight already transpose
	for (i = 0; i < MATDIM; i++) {
		for (j = 0; j < MATDIM; j++) {
			idx1 = i * MATDIM + j;
			pDestMatrix[idx1] = 0;
			for (k = 0; k < MATDIM; k += 4) {
				idx2 = i * MATDIM + k;
				pDestMatrix[idx1] += pLeftMatrix[idx2] * pRightMatrix[idx2];
				idx2++;
				pDestMatrix[idx1] += pLeftMatrix[idx2] * pRightMatrix[idx2];
			}
		}
	}
}

void MultiplySquareMatrices_4(double *pDestMatrix, double *pLeftMatrix, double *pRightMatrix, int MatSize)
{
	int i, j, k, tmp, idx1, idx2;
	// pRight already transpose
	for (i = 0; i < MATDIM; i++) {
		for (j = 0; j < MATDIM; j++) {
			idx1 = i * MATDIM + j;
			pDestMatrix[idx1] = 0;
			for (k = 0; k < MATDIM; k += 4) {
				idx2 = i * MATDIM + k;
				pDestMatrix[idx1] += pLeftMatrix[idx2] * pRightMatrix[idx2];
				idx2++;
				pDestMatrix[idx1] += pLeftMatrix[idx2] * pRightMatrix[idx2];
				idx2++;
				pDestMatrix[idx1] += pLeftMatrix[idx2] * pRightMatrix[idx2];
				idx2++;
				pDestMatrix[idx1] += pLeftMatrix[idx2] * pRightMatrix[idx2];
			}
		}
	}
}

void init_MatMat(void)
{
	double *ptr;
	pMatA = (double *)malloc(sizeof(double)*MATDIM*MATDIM);
	pMatB = (double *)malloc(sizeof(double)*MATDIM*MATDIM);
	pMatC = (double *)malloc(sizeof(double)*MATDIM*MATDIM);
	srand((unsigned)time(NULL));
	ptr = pMatA;
	for (int i = 0; i < MATDIM*MATDIM; i++)
		*ptr++ = (double)rand() / ((double)RAND_MAX);
	ptr = pMatB;
	for (int i = 0; i < MATDIM*MATDIM; i++)
		*ptr++ = (double)rand() / ((double)RAND_MAX);
}
