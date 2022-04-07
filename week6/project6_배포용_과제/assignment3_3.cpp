#include "my_solver.h"

#define SOLNUMS 2
#define MATCOLS SOLNUMS
#define MATROWS SOLNUMS
#define TOLERANCE 0.0000001
void fcn_a3_3_3(int* n, double* x, double* fvec, int* iflag) {
	fvec[0] = sin(x[0] * x[1] + M_PI / 6) + sqrt(pow(x[0] * x[1], 2) + 1);
	fvec[0] += 2.8 * cos(x[0] - x[1]);

	fvec[1] = x[0] * exp(x[0] * x[1] + M_PI / 6) - sin(x[0] - x[1]);
	fvec[1] -= 1.66 * sqrt(pow(x[0] * x[1], 2) + 1);
}


void assignment3_3() {
	int n = SOLNUMS;
	double x[SOLNUMS] = { 20.0, 0.0};	//need to initilize x0
	double fvec[SOLNUMS];
	int ldfjac = SOLNUMS;
	double tol = TOLERANCE;
	int info;
	double wa[(SOLNUMS * (3 * SOLNUMS + 13)) / 2];
	int lwa = (SOLNUMS * (3 * SOLNUMS + 13)) / 2;

	FILE* fp_w;
	fp_w = fopen("roots_found_3-3.txt", "w");
	if (fp_w == NULL) {
		printf("roots_found_3-3.txt : cannot open file\n");
		return;
	}
	fprintf(fp_w, "initial x\nx1 = %lf, x2 = %lf\n", x[0], x[1]);
	hybrd1_(fcn_a3_3_3, &n, x, fvec, &tol, &info, wa, &lwa);
	fprintf(fp_w, "-------------------------------------------------------------\n");
	fprintf(fp_w, "x1 = %lf, x2 = %lf\n", x[0], x[1]);
	fprintf(fp_w, "f_1(x,y) = %lf, ", fvec[0]);
	fprintf(fp_w, "f_2(x,y) = %lf\n", fvec[1]);



	fclose(fp_w);
}