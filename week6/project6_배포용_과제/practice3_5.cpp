#include "my_solver.h"

#define SOLNUMS 3
#define MATCOLS SOLNUMS
#define MATROWS SOLNUMS
#define TOLERANCE 0.0000001

void fcn3_5(int* n, double* x, double* fvec, int* iflag)
{
	/********************************/
	fvec[0] = 10 * x[0] - 2 * x[1] * x[1] + x[1] - 2 * x[2] - 5;
	fvec[1] = 8 * x[1] * x[1] + 4 * x[2] * x[2] - 9;
	fvec[2] = 8 * x[1] * x[2] + 4;
	/********************************/
}

void practice3_5(void)
{
	int n = SOLNUMS;
	double x[SOLNUMS] = { 1.0, -1.0, 1.0 };	//need to initilize x0
	double fvec[SOLNUMS];
	double tol = TOLERANCE;
	int info;
	double wa[(SOLNUMS * (3 * SOLNUMS + 13)) / 2];
	int lwa = (SOLNUMS * (3 * SOLNUMS + 13)) / 2;

	FILE* fp_w = fopen("roots_3-5.txt", "w");
	if (fp_w == NULL) {
		printf("%s file open error...\n", "roots_3-5.txt");
		return;
	}

	/********************************/
	hybrd1_(fcn3_5, &n, x, fvec, &tol, &info, wa, &lwa);
	fprintf(fp_w, "  x1  = %lf,  x2  = %lf,  x3  = %lf\n", x[0], x[1], x[2]);
	fprintf(fp_w, "f(x1) = %lf, f(x2) = %lf, f(x3) = %lf\ninfo : %d\n\n", fvec[0], fvec[1], fvec[2], info);

	x[0] = 1.0;
	x[1] = 1.0;
	x[2] = -1.0;
	hybrd1_(fcn3_5, &n, x, fvec, &tol, &info, wa, &lwa);
	fprintf(fp_w, "  x1  = %lf,  x2  = %lf,  x3  = %lf\n", x[0], x[1], x[2]);
	fprintf(fp_w, "f(x1) = %lf, f(x2) = %lf, f(x3) = %lf\ninfo : %d\n", fvec[0], fvec[1], fvec[2], info);
	/********************************/

	fclose(fp_w);
}