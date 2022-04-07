#include "my_solver.h"

extern double (*_f)(double);
extern double (*_fp)(double);

/*********************************************
  Bisection Method -- HOMEWORK
**********************************************/
void program1_3(FILE *fp)
{
	double a0, b0, x0, x1 , temp;
	double _fa, _fb, _fc;
	int n = 0;
	
	printf("input [a, b] : ");
	scanf("%lf %lf", &a0, &b0);

	fprintf(fp, "n		 a0			b0			(a0+b0)/2			|f((a0+b0)/2)|\n");
	//printf("i         x             |f(x)|          epsilon(n)        epsilon(n)/epsilon(n-1)\n");
	double eps = 1, eps_nx = 0;
	while (n < Nmax) {
		_fa = _f(a0);
		_fb = _f(b0);
		if (_fa * _fb > 0) {
			printf("couldn't find the root\n");
			return;
		}
		temp = (a0 + b0) / 2.0;
		_fc = _f(temp);
		x0 = a0;

		fprintf(fp, "%-2d  %25.15e %25.15e %25.15e %25.15e \n", n, a0, b0, temp, fabs(_fa));
		//printf( "%-2d %25.15e %25.15e\n", n, temp, fabs(_fc));
		//eps_nx = fabs(temp - 4.399999998509884);
		//printf("%d %12.10e %12.10e %12.10e %12.10e\n", n, temp, fabs(_f(temp)), eps_nx, eps / eps_nx);
		if (_fc == 0) {
			x0 = _fc;
			break;
		}
		if (fabs(_fc) < DELTA) break;
		if (fabs(a0 - b0) < EPSILON) break;
		if (fabs(a0 - b0) / fabs(a0) < EPSILON || fabs(_f(a0)) < EPSILON) break;

		if (_fa * _fc < 0 && _fc * _fb > 0) b0 = temp;
		if (_fa * _fc > 0 && _fc * _fb < 0) a0 = temp;
		
		eps = eps_nx;
		n++;
	}
	printf("%25.15e\n", x0);
}