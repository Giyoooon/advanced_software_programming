#include "my_solver.h"

#define SOLNUMS 3
#define MATCOLS SOLNUMS
#define MATROWS SOLNUMS
#define TOLERANCE 0.0000001

void fcn3_4(int* n, double* x, double* fvec, double* fjac, int* ldfjac, int* iflag)
{
	// origin function F(x)
	if (*iflag == 1) {
		/********************************/
		fvec[0] = x[0] + x[1] + x[2] - 3;
		fvec[1] = x[0] * x[0] + x[1] * x[1] + x[2] * x[2] - 5;
		fvec[2] = exp(x[0]) + x[0]*x[1] - x[0]*x[2] - 1;
		/********************************/
	}
	// Jacobi matrix J(x)
	else if (*iflag == 2) {
		/********************************/
//		fjac[0] = 1; fjac[3] = 2 * x[0]; fjac[6] = exp(x[0]) + x[1] - x[2];
//		fjac[1] = 1; fjac[4] = 2 * x[1]; fjac[7] = x[0];
//		fjac[2] = 1; fjac[5] = 2 * x[2]; fjac[8] = -x[0];
		
		fjac[0] = 1;                       fjac[3] = 1;        fjac[6] = 1;
		fjac[1] = 2 * x[0];                fjac[4] = 2 * x[1]; fjac[7] = 2 * x[2]; 
		fjac[2] = exp(x[0]) + x[1] - x[2]; fjac[5] = x[0];     fjac[8] = -x[0];

		/********************************/
	}
}

void practice3_4(void) {
	int n = SOLNUMS;
	double x[SOLNUMS] = { 0.1, 1.2, 2.5 };	//need to initilize x0
	double fvec[SOLNUMS], fjac[MATCOLS * MATROWS];
	int ldfjac = SOLNUMS;
	double tol = TOLERANCE;
	int info;
	double wa[(SOLNUMS * (SOLNUMS + 13)) / 2];
	int lwa = (SOLNUMS * (SOLNUMS + 13)) / 2;

	FILE* fp_w = fopen("roots_3-4.txt", "w");
	if (fp_w == NULL) {
		printf("%s file open error...\n", "roots_3-4.txt");
		return;
	}

	/********************************/
	hybrj1_(fcn3_4, &n, x, fvec, fjac, &ldfjac, &tol, &info, wa, &lwa);
	fprintf(fp_w, "  x1  = %lf,  x2  = %lf,  x3  = %lf\n", x[0], x[1], x[2]);
	fprintf(fp_w, "f(x1) = %lf, f(x2) = %lf, f(x3) = %lf\ninfo : %d\n\n", fvec[0], fvec[1], fvec[2], info);

//	printf("  x1  = %lf,  x2  = %lf,  x3  = %lf\n", x[0], x[1], x[2]);
//	printf("f(x1) = %lf, f(x2) = %lf, f(x3) = %lf\ninfo : %d\n\n", fvec[0], fvec[1], fvec[2], info);

	x[0] = 1.0;
	x[1] = 0.0;
	x[2] = 1.0;
	hybrj1_(fcn3_4, &n, x, fvec, fjac, &ldfjac, &tol, &info, wa, &lwa);
	fprintf(fp_w, "  x1  = %lf,  x2  = %lf,  x3  = %lf\n", x[0], x[1], x[2]);
	fprintf(fp_w, "f(x1) = %lf, f(x2) = %lf, f(x3) = %lf\ninfo : %d\n", fvec[0], fvec[1], fvec[2], info);
	
	//printf("  x1  = %lf,  x2  = %lf,  x3  = %lf\n", x[0], x[1], x[2]);
	//printf("f(x1) = %lf, f(x2) = %lf, f(x3) = %lf\ninfo : %d\n", fvec[0], fvec[1], fvec[2], info);

	/********************************/

	fclose(fp_w);
}
