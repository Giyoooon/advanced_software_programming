#include "my_solver.h"

void mult_i(double r1, double i1, double r2, double i2, double* r_res, double* i_res) {
	*r_res = r1 * r2 - i1 * i2;
	*i_res = r1 * i2 + r2 * i1;
}
void practice3_2() {
	char readfile[256];
	char writefile[256];

	int NCOEF, DEGREE;

	double* poly;
	double* zeror, * zeroi;
	long int fail;
	int i;

	for (int t = 1; t <= 6; t++) {

		sprintf(readfile, "polynomial_3-2_%d.txt", t);
		sprintf(writefile, "roots_3-2_%d.txt", t);

		FILE* fp_r = fopen(readfile, "r");
		if (fp_r == NULL) {
			printf("%s file open error...\n", readfile);
			return;
		}

		FILE* fp_w = fopen(writefile, "w");
		if (fp_w == NULL) {
			printf("%s file open error...\n", writefile);
			return;
		}

		fscanf(fp_r, "%d", &DEGREE);
		NCOEF = DEGREE + 1;

		poly = (double*)malloc(sizeof(double) * NCOEF);
		zeror = (double*)malloc(sizeof(double) * DEGREE);
		zeroi = (double*)malloc(sizeof(double) * DEGREE);


		/********************************/
		int j, k;
		for (i = 0; i < NCOEF; i++) {
			fscanf(fp_r, "%lf", &poly[i]);
		}
		rpoly_(poly, &DEGREE, zeror, zeroi, &fail);
		
		if (fail) {// 근을 구할 수 없을 때
			fprintf(fp_w, "no root.\n");
			continue;
		}
		fprintf(fp_w, "       zeror        zeroi     |f(root)| \n", zeror[i]);
		double sum_r, sum_i;
		for (i = 0; i < DEGREE; i++) {
			fprintf(fp_w, "%10f ", zeror[i]); 
			fprintf(fp_w, "%10f ", zeroi[i]);
			//double fx = sqrt(zeroi[i] * zeroi[i] + zeror[i]*zeror[i]);

			sum_r = poly[DEGREE];
			sum_i = 0;
			double tmp_real, tmp_i;
			double mul_real = zeror[i], mul_i = zeroi[i];
			
			for (j = DEGREE - 1; j >= 0; j--) {
				//printf("mul_real : %lf mul_i: %lf\n", mul_real, mul_i);
				sum_r += poly[j] * mul_real;
				sum_i += poly[j] * mul_i;
				
				mult_i(zeror[i], zeroi[i], mul_real, mul_i, &tmp_real, &tmp_i);

				//printf("tmp_real : %lf tmp_i: %lf\n", tmp_real, tmp_i);
				mul_real = tmp_real;
				mul_i = tmp_i;
			}
			//printf("%lf\n%lf\n\n", sum_r, sum_i);

			fprintf(fp_w, "%10f \n", fabs(sqrt(sum_r*sum_r + sum_i * sum_i)));
		}
		/********************************/


		free(zeroi);
		free(zeror);
		free(poly);

		if (fp_r != NULL) fclose(fp_r);
		if (fp_w != NULL) fclose(fp_w);
	}
}