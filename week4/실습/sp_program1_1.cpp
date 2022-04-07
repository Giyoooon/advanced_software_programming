#include "my_solver.h"

extern float (*_sp_f)(float);
extern float (*_sp_fp)(float);

/*********************************************
  Newton-Rapson Method
**********************************************/
void sp_program1_1(FILE* fp) {
	float x0, x1;
	int n = 0;

	if (fp == NULL)
		return;

	printf("input x_0 : ");
	scanf("%f", &x0);

	fprintf(fp, "n		 x			|f(x)|\n");
	//printf( "n		 x			|f(x)|\n");

	while (n < Nmax) {
		float _sp_fx = _sp_f(x0);

		fprintf(fp, "%-2d %25.15e %25.15e\n", n, x0, fabs(_sp_fx));
		//printf( "%-2d %25.15e %25.15e\n", n, x0, fabs(_sp_fx));
		if (fabs(_sp_fx) < DELTA) break;

		x1 = x0 - (_sp_fx / _sp_fp(x0));

		if (fabs(x1 - x0) < EPSILON) break;

		x0 = x1;
		n++;
	}
	printf("%25.15e\n", x0);

}
