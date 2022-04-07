#include "my_solver.h"

#define SOLNUMS 2
#define MATCOLS SOLNUMS
#define MATROWS SOLNUMS
#define TOLERANCE 0.0000001

void fcn3_7(int* n, double* x, double* fvec, int* iflag)
{
	/********************************/
	fvec[0] = 2 * x[0] * x[0] * x[0] - 12 * x[0] - x[1] - 1;
	fvec[1] = 3 * x[1] * x[1] - 6 * x[1] - x[0] - 3;
	/********************************/
}

void practice3_7(void)
{
	int n = SOLNUMS;
	double x[SOLNUMS] = { -4.0, 5.0 };	//need to initilize x0
	double fvec[SOLNUMS];
	double tol = TOLERANCE;
	int info;
	double wa[(SOLNUMS * (3 * SOLNUMS + 13)) / 2];
	int lwa = (SOLNUMS * (3 * SOLNUMS + 13)) / 2;

	FILE* fp_w = fopen("roots_3-7.txt", "w");
	if (fp_w == NULL) {
		printf("%s file open error...\n", "roots_3-7.txt");
		return;
	}

	/********************************/
	double i, j;
	for (i = -4.0; i <= 4.0; i += 0.1) {
		for (j = -5.0; j <= 5.0; j += 0.1) {
			//printf("%lf %lf\n", i, j);
			x[0] = i; //x
			x[1] = j; //y
			hybrd1_(fcn3_7, &n, x, fvec, &tol, &info, wa, &lwa);
			if (info == 1) {
				if ((i < x[0] && x[0] < i + 0.1) && (j < x[1] && x[1] < j + 0.1)) {
					fprintf(fp_w, "  x1  = %lf,  x2  = %lf\n", x[0], x[1]);
					fprintf(fp_w, "f(x1) = %lf, f(x2) = %lf\ninfo : %d\n\n", fvec[0], fvec[1], info);
				}
			}
		}
	}
	/********************************/

	fclose(fp_w);
}