#include "my_solver.h"

#define SOLNUMS 4
#define MATCOLS SOLNUMS
#define MATROWS SOLNUMS
#define TOLERANCE 0.0000001

double C, B;
double P[SOLNUMS][SOLNUMS];
double T[SOLNUMS];
double TR[SOLNUMS];


void program3_1() {
	char fn_r[256];
	char fn_w1[256];
	char fn_w2[256];

	for (int i = 0; i < 3; i++) {
		sprintf(fn_r, "GPS_signal_%d.txt", i);
		sprintf(fn_w1, "GPS_position_3-1_%d.txt", i);
		sprintf(fn_w2, "GPS_position_3-2_%d.txt", i);

		FILE* fp_r = fopen(fn_r, "r");
		if (fp_r == NULL) {
			printf("%s: error file not exist..\n", fn_r);
			return;
		}

		FILE* fp_w1 = fopen(fn_w1, "w");
		if (fp_r == NULL) {
			printf("%s: error file not exist..\n", fn_r);
			return;
		}

		FILE* fp_w2 = fopen(fn_w2, "w");
		if (fp_r == NULL) {
			printf("%s: error file not exist..\n", fn_r);
			return;
		}

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

	}
}