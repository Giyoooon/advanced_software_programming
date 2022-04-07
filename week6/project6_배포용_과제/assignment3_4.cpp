#include "my_solver.h"
#define MAX_N 32

void assignment3_4() {
	/*
	char fn_r1[256];
	char fn_r2[256];
	char fn_w1[256];
	char fn_w2[256];
	*/ 
//	int cnt;
//	int index[10] = {2,3,4,5,7,9,12,16,24,32};
	/*
	for (cnt = 0; cnt < 10; cnt++) {
		sprintf(fn_r1, "General_%d.txt",index[cnt]);
		sprintf(fn_r2, "Hilbert_%d.txt", index[cnt]);

		sprintf(fn_w1, "General_%d_result.txt", index[cnt]);
		sprintf(fn_w2, "Hilbert_%d_result.txt", index[cnt]);

		FILE* fp_r1, * fp_r2, *fp_w1, *fp_w2;
		fp_r1 = fopen(fn_r1,"r");
		fp_r2 = fopen(fn_r2, "r");
		fp_w1 = fopen(fn_w1, "w");
		fp_w2 = fopen(fn_w2, "w");

		if (fp_r1 == NULL) {
			printf("%s : cannt open file.\n", fn_r1);
			return;
		}
		if (fp_r2 == NULL) {
			printf("%s : cannt open file.\n", fn_r2);
			return;
		}
		if (fp_w1 == NULL) {
			printf("%s : cannt open file.\n", fn_w1);
			return;
		}
		if (fp_w2 == NULL) {
			printf("%s : cannt open file.\n", fn_w2);
			return;
		}

		int ia, n, l[MAX_N];
		float a[MAX_N * MAX_N], b[MAX_N], x[MAX_N], s[MAX_N];
		float A[MAX_N * MAX_N], B[MAX_N];

		fscanf(fp_r1, "%d", &n);
		for (int i = 0; i < n * n; i++) {
			fscanf(fp_r1, "%f", &A[i]);
			a[i] = A[i];
		}
		for (int i = 0; i < n; i++) {
			fscanf(fp_r1, "%f", &B[i]);
			b[i] = B[i];
		}
		ia = n;

		gespp_(&n, a, &ia, l, s);
		solve_(&n, a, &ia, l, b, x);
		fprintf(fp_w1, "%d\n", n);
		for (int i = 0; i < n; i++) {
			fprintf(fp_w1, "x%d : %.6f\n",i, x[i]);
		}
		
		float AX[MAX_N];
		float norm = 0;
		float norm_b = 0;
		for (int i = 0; i < n; i++) {
			AX[i] = 0;
			for (int j = 0; j < n; j++) {
				int idx = n * j + i;
				AX[i] += A[idx] * x[j];
			}
			norm += (AX[i] - B[i])* (AX[i] - B[i]);
			norm_b += B[i] * B[i];
		}
		fprintf(fp_w1, "%.6f", norm / norm_b);

		fclose(fp_w1);
		fclose(fp_r1);


		fscanf(fp_r2, "%d", &n);
		for (int i = 0; i < n * n; i++) {
			fscanf(fp_r2, "%f", &A[i]);
			a[i] = A[i];
		}
		for (int i = 0; i < n; i++) {
			fscanf(fp_r2, "%f", &B[i]);
			b[i] = B[i];
		}
		ia = n;

		gespp_(&n, a, &ia, l, s);
		solve_(&n, a, &ia, l, b, x);
		fprintf(fp_w2, "%d\n", n);
		for (int i = 0; i < n; i++) {
			fprintf(fp_w2, "x%d : %.6f\n",i, x[i]);
		}

//		float AX[MAX_N];
//		float norm = 0;
//		float norm_b = 0;
		norm = 0;
		norm_b = 0;
		for (int i = 0; i < n; i++) {
			AX[i] = 0;
			for (int j = 0; j < n; j++) {
				int idx = n * j + i;
				AX[i] += A[idx] * x[j];
			}
			norm += (AX[i] - B[i]) * (AX[i] - B[i]);
			norm_b += B[i] * B[i];
		}
		fprintf(fp_w2, "%.6f", norm / norm_b);
		fclose(fp_w2);
		fclose(fp_r2);
	}
	*/
//	/*
	FILE* fp_r, *fp_w;
	
	fp_r = fopen("linear system 3-4.txt", "r");
	fp_w = fopen("solution_3-4.txt", "w");

	if (fp_r == NULL) {
		printf("linear system 3-4.txt : cannt open file.\n");
		return;
	}

	int ia, n, l[MAX_N];
	float a[MAX_N *MAX_N], b[MAX_N], x[MAX_N], s[MAX_N];
	float A[MAX_N*MAX_N], B[MAX_N];

	fscanf(fp_r, "%d", n);
	ia = n;
	if (n > 32) {
		printf("n = %d is too big!\n",n);
		fclose(fp_r);
		fclose(fp_w);
		return;	
	}
	for (int i = 0; i < n; i++) {
		fscanf(fp_r, "%f", &a[i]);
		A[i] = a[i];
		
	}
	for (int i = 0; i < n; i++) {
		fscanf(fp_r, "%f", &b[i]);
		B[i] = b[i];
	}
	ia = n;
	gespp_(&n, a, &ia, l, s);
	solve_(&n, a, &ia, l, b, x);


//	*/
}