#define _USE_MATH_DEFINES
#include <math.h>
//#include <stdio.h>
double _f1(double x) {
	double result ;
	result = (x * x) - (4.0 * x) + 4.0 - log(x);
	return result;
}

double _fp1(double x) {
	
	double result;
	result = (2 * x) - 4.0 - (1.0 / x);
	return result;
}

double _f2(double x) {
	// f2 = x +1 -2sin(PI*x) = 0
	double result;
	result = x + 1.0 - 2 * sin(M_PI * x);
	return result;
}

double _fp2(double x) {
	// f2 = x +1 -2sin(PI*x) = 0
	double result;
	result = 1.0 - 2 * M_PI * cos(M_PI * x);
	return result;
}

double _f3(double x) {
	double result;
	result = (x * x * x * x) - 11.0 * (x * x * x) + 42.35 * (x * x) - 66.55 * x + 35.1384;
	return result;
}

double _fp3(double x) {
	double result;
	result = 4 * (x * x * x) - 11.0 * 3 * (x * x) + 42.35 * 2 * x - 66.55;
	return result;
}

double _f_sqrt(double x) {
	return x * x - 2.0;
}

double _fp_sqrt(double x) {
	return 2.0 * x;
}

double _f_vehicle(double x) {
	double result;
	double l = 89.0;
	double h = 49.0;
	double D = 55.0;
	double beta1 = 11.5 * (M_PI / 180);

	double A = l * sin(beta1);
	double B = l * cos(beta1);
	double C = (h + 0.5 * D) * sin(beta1) - 0.5 * D * tan(beta1);
	double E = (h + 0.5 * D) * cos(beta1) - 0.5 * D;
	x *= (M_PI / 180);
	result = A * sin(x) * cos(x) + B * sin(x) * sin(x) - C * cos(x) - E * sin(x);
	return result;
}

double _fp_vehicle(double x) {
	double result;
	double l = 89.0;
	double h = 49.00;
	double D = 55.0;
	double beta1 = 11.5 * (M_PI / 180);

	double A = l * sin(beta1); //17.743746
	double B = l * cos(beta1); //87.213298
	double C = (h + 0.5 * D) * sin(beta1) - 0.5 * D * tan(beta1);//9.656709
	double E = (h + 0.5 * D) * cos(beta1) - 0.5 * D;// 47.464240
	x *= (M_PI / 180);
	//printf("A: %lf\nB: %lf\nC: %lf\nE: %lf\n",A,B,C,E);
	result = -A * sin(x) * sin(x) + 2 * B * cos(x) * sin(x) + C * sin(x) - E * cos(x);
	result *= (M_PI / 180);
	return result;
}

double _f_comp(double x) {
	double result;
	result = log(x) - 1.0;
	return result;
}

double _fp_comp(double x) {
	double result;
	result = 1.0 / x;
	return result;
}
