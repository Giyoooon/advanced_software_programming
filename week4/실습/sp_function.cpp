#define _USE_MATH_DEFINES
#include <math.h>

float _sp_f_comp(float x) {
	float result;
	result = log(x) - 1.0;
	return result;
}

float _sp_fp_comp(float x) {
	float result;
	result = 1.0 / x;
	return result;
}
