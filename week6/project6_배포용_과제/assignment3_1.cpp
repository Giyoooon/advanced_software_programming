#include "my_solver.h"

#define SOLNUMS 4
#define MATCOLS SOLNUMS
#define MATROWS SOLNUMS 
#define TOLERANCE 0.0000001

double C, B;
double p[SOLNUMS][SOLNUMS];
double t[SOLNUMS], tr[SOLNUMS];



void fcn_a1_3_1(int* n, double* x, double* fvec, double* fjac, int* ldfjac, int* iflag)
{	
	int i, j;
	if (*iflag == 1) {
		for (i = 0; i < 4; i++) {
			fvec[i] = -C * C *(tr[i] + x[3] - t[i]) * ( tr[i] + x[3] - t[i]);
			for ( j = 0; j < 3; j++) {
				fvec[i] += (x[j] - p[i][j]) * (x[j] - p[i][j]);
			}
		}
	}
	else if (*iflag == 2) {
		for (i = 0; i < 4; i++) {
			for (j = 0; j < 3; j++) {
				fjac[4 * j + i] = 2 * (x[j] - p[i][j]);
			}
			fjac[4 * 3 + i] = -2 * C * C * (tr[i] + x[3] - t[i]);
		}
		
	}
}

void fcn_a1_3_2(int* n, double* x, double* fvec, int* iflag)
{
	/********************************/
	for (int i = 0; i < 4; i++) {
		fvec[i] = -C * C * (tr[i] + x[3] - t[i]) * (tr[i] + x[3] - t[i]);
		for (int j = 0; j < 3; j++) {
			fvec[i] += (x[j] - p[i][j]) * (x[j] - p[i][j]);
		}
	}
	/********************************/
}

void assignment3_1(){
	char fn_r[256];
	char fn_w1[256];
	char fn_w2[256];

	for (int cnt = 0; cnt < 3; cnt++) {
		sprintf(fn_r, "GPS_signal_%d.txt", cnt);
		sprintf(fn_w1, "GPS_position_3-1_%d.txt", cnt);
		sprintf(fn_w2, "GPS_position_3-2_%d.txt", cnt);
		FILE* fp_r; FILE* fp_w1; FILE* fp_w2;
		fp_r = fopen(fn_r, "r");
		fp_w1 = fopen(fn_w1, "w");
		fp_w2 = fopen(fn_w2, "w");

		if (fp_r == NULL) {
			printf("%s : cannot open file\n", fn_r);
			return;
		}
		if (fp_w1 == NULL) {
			printf("%s : cannot open file\n", fn_w1);
			return;
		}
		if (fp_w2 == NULL) {
			printf("%s : cannot open file\n", fn_w1);
			return;
		}

		int n = SOLNUMS;
		double x[SOLNUMS];
		double fvec[SOLNUMS], fjac[MATCOLS * MATROWS];
		int ldfjac = SOLNUMS;
		double tol = TOLERANCE;
		int info;
		double wa[(SOLNUMS * (SOLNUMS + 13)) / 2];
		int lwa = (SOLNUMS * (SOLNUMS + 13)) / 2;
		double wa_d[(SOLNUMS * (3 * SOLNUMS + 13)) / 2];
		int lwa_d = (SOLNUMS * (3 * SOLNUMS + 13)) / 2;
		fscanf(fp_r, "%lf %lf\n", &C, &B);

		//printf("%lf %lf\n", C, B);
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < 3; j++) {
				fscanf(fp_r, "%lf", &p[i][j]);
				//printf("%lf ", p[i][j]);
			}
			fscanf(fp_r, "%lf %lf",&t[i], &tr[i]);
			//printf("%lf %lf\n", t[i], tr[i]);
		}
		printf("hybrj1_ : Input initial value of x1, x2, x3 : \n");
		scanf("%lf %lf %lf", &x[0], &x[1], &x[2]);
		x[3] = B;
		fprintf(fp_w1, "calculated by hybrj1 :\n");
		fprintf(fp_w1, "user initial x1 : %lf, x2 : %lf, x3 : %lf, x4 : %lf\n", x[0], x[1], x[2], x[3]);
		
		hybrj1_(fcn_a1_3_1, &n, x, fvec, fjac, &ldfjac, &tol, &info, wa, &lwa);

		fprintf(fp_w1, "-------------------------------------------------------------------------------------------------------------\n");
		fprintf(fp_w1, "x1 : %-10lf, x2: %-10lf, x3: %-10lf, x4: %-10lf\n", x[0], x[1], x[2], x[3]);
		fprintf(fp_w1, "f(x1) : %-10lf, f(x2) : %-10lf, f(x3) : %-10lf, f(x4) : %-10lf\n(info = %d)\n\n", fvec[0], fvec[1], fvec[2], fvec[3], info);

		/*--------------------------------------------------------*/
		x[0] = x[1] = x[2] = x[3] = 0;
		printf("hybrd1_ : Input initial value of x1, x2, x3 :\n");
		scanf("%lf %lf %lf", &x[0], &x[1], &x[2]);
		x[3] = B;
		fprintf(fp_w2, "calculated by hybrd1 :\n");
		fprintf(fp_w2, "user initial x1 : %lf, x2 : %lf, x3 : %lf, x4 : %lf \n", x[0], x[1], x[2], x[3]);

		fprintf(fp_w2, "-------------------------------------------------------------------------------------------------------------\n");
		hybrd1_(fcn_a1_3_2, &n, x, fvec, &tol, &info, wa_d, &lwa_d);
		fprintf(fp_w2, "x1 : %-10lf, x2: %-10lf, x3: %-10lf, x4: %-10lf\n", x[0], x[1], x[2], x[3]);
		fprintf(fp_w2, "f(x1) : %-10lf, f(x2) : %-10lf, f(x3) : %-10lf, f(x4) : %-10lf\n(info = %d)\n\n", fvec[0], fvec[1], fvec[2], fvec[3], info);
		
		fclose(fp_r);
		fclose(fp_w1);
		fclose(fp_w2);
	}
}