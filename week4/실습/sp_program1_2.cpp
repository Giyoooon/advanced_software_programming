#include "my_solver.h"

extern float (*_sp_f)(float);
extern float (*_sp_fp)(float);

/*********************************************
  Secant Method
**********************************************/
void sp_program1_2(FILE* fp) {
	float x0, x1, temp;
	int n;

	if (fp == NULL)
		return;
	printf("input x_0, x_1 : ");
	scanf("%f %f", &x0, &x1);

	fprintf(fp, "n		 x			|f(x)|\n");
	//printf( "n		 x			|f(x)|\n");
	while (n < Nmax) {
		double _fx0 = _sp_f(x0);
		double _fx1 = _sp_f(x1);

		fprintf(fp, "%-2d %25.15e %25.15e\n", n, x1, fabs(_fx1));
		//printf( "%-2d %25.15e %25.15e\n", n, x1, fabs(_fx1));

		if (fabs(_fx1) < DELTA) break;

		temp = x0 - _fx0 * ((x1 - x0) / (_fx1 - _fx0));

		if (fabs(x1 - x0) < EPSILON) break;

		x0 = x1;
		x1 = temp;
		n++;
	}
	//printf("iter : %d	", n);
	printf("%25.15e\n", x0);
}
