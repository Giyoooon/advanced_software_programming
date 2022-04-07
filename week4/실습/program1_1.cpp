#include "my_solver.h"

extern double (*_f)(double);
extern double (*_fp)(double);

/*********************************************
  Newton-Rapson Method
**********************************************/
void program1_1(FILE* fp) {
	double x0, x1;
	int n = 0;
	//int count = 0;
	//int N_max = Nmax;
	if (fp == NULL)
		return;
	printf("input x_0 : ");
	scanf("%lf", &x0);
	
	fprintf(fp,"n		 x			|f(x)|			\n");
	//printf("n		 x			|f(x)|			epsilon(n)			epilson(n)/(epsilon(n-1)^2)\n");
	double prev_ep = 1, ep = 0;
	while (n < Nmax) {
		double _fx = _f(x0);
		double _fpx = _fp(x0);
		ep = fabs(x0 - 3.057103549998436);
		fprintf(fp,"%-2d %25.15e %25.15e\n",n, x0, fabs(_fx));
		//printf( "%-2d %25.15e %25.15e %25.15e %25.15e\n", n, x0, fabs(_fx), prev_ep, ep/(prev_ep*prev_ep));
		
		if (fabs(_fx) < DELTA) break;
		x1 = x0 - (_fx / _fpx);
		if (fabs(x1 - x0) < EPSILON) break;
		prev_ep = ep;
		x0 = x1;
		n++;
	}
	//printf("iter : %d	", n);
	
	printf("%25.15e\n", x0);
	//scanf 입력값 받고 뉴튼 뭐시기 호출 _f _f
}
