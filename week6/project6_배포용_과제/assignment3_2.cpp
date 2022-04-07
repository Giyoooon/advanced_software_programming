#include "my_solver.h"

#define SOLNUMS 4
#define MATCOLS SOLNUMS
#define MATROWS SOLNUMS
#define TOLERANCE 0.0000001

void fcn_a2_3_2(int* n, double* x, double* fvec, int* iflag) {
	fvec[0] = x[0] + 10 * x[1] + 9;
	fvec[1] = sqrt(5) * (x[2] - x[3] - 2);
	fvec[2] = (x[1] - 2*x[2] - 3) * (x[1] - 2 * x[2] + 3);
	fvec[3] = sqrt(10) *((x[0] - x[3]) * (x[0] - x[3]) - 2);
}
void assignment3_2() {
	int n = SOLNUMS;
	double x[SOLNUMS] = { 0.9, -0.9, 1.25, -1.25 };	//need to initilize x0
	double fvec[SOLNUMS];
	int ldfjac = SOLNUMS;
	double tol = TOLERANCE;
	int info;
	double wa[(SOLNUMS * (3 * SOLNUMS + 13)) / 2];
	int lwa = (SOLNUMS * (3 * SOLNUMS + 13)) / 2;

	FILE* fp_w;
	fp_w = fopen("roots_found_3-2.txt", "w");

	if (fp_w == NULL) {
		printf("roots_found_3-2.txt : cannot open file\n");
		return;
	}

	fprintf(fp_w, "initial x\nx1 = %lf, x2 = %lf\nx3 = %lf, x4 = %lf\n", x[0], x[1], x[2], x[3]);
	hybrd1_(fcn_a2_3_2, &n, x, fvec, &tol, &info, wa, &lwa);
	fprintf(fp_w, "-------------------------------------------------------------\n");
	fprintf(fp_w, "x1 = %lf, x2 = %lf\nx3 = %lf, x4 = %lf\n",x[0], x[1], x[2], x[3]);
	fprintf(fp_w, "f_1(x,y,z,w) = %lf, ", fvec[0]);
	fprintf(fp_w, "f_2(x,y,z,w) = %lf\n", fvec[1]);
	fprintf(fp_w, "f_3(x,y,z,w) = %lf, ", fvec[2]);
	fprintf(fp_w, "f_4(x,y,z,w) = %lf\n", fvec[3]);

	fclose(fp_w);
}