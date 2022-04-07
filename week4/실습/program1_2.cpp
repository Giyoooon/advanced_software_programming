#include "my_solver.h"

extern double (*_f)(double);
extern double (*_fp)(double);

/*********************************************
  Secant Method
**********************************************/
void program1_2(FILE* fp) {
	double x0, x1, temp;
	int n = 0;

	if (fp == NULL)
		return;
	printf("input x_0, x_1 : ");
	scanf("%lf %lf", &x0, &x1);
	
	fprintf(fp, "n		 x			|f(x)|\n");
	//printf( "n		 x			|f(x)|		epsilon(n)		epsilon/(epsilon(n-1)^1.62)\n");

	double prev_ep = 1, ep = 0;
	while (n < Nmax) {
		double _fx0 = _f(x0);
		double _fx1 = _f(x1);

		fprintf(fp, "%-2d %25.15e %25.15e\n", n, x1, fabs(_fx1));
		ep = fabs(x1 - 0.68974818860416);
		//printf( "%-2d %25.15e %25.15e %25.15e %25.15e\n", n, x1, fabs(_fx1),ep, ep/pow(prev_ep,1.62));

		if (fabs(_fx1) < DELTA) break;

		temp = x0 - _fx0 * ((x1 - x0) / (_fx1 - _fx0));

		if (fabs(x1 - x0) < EPSILON) break;

		x0 = x1;
		x1 = temp;
		n++;

		prev_ep = ep;
	}
	//printf("iter : %d	", n);
	printf("%25.15e\n", x0);
}
