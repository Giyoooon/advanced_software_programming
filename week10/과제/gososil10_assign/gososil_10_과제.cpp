#include <stdio.h>
#include <string.h>
#include <random>
#include <time.h>

#include <math.h>
#include <time.h>
#include <Windows.h>

#pragma optimize("",off);

__int64 start, freq, end;
#define CHECK_TIME_START QueryPerformanceFrequency((LARGE_INTEGER*)&freq); QueryPerformanceCounter((LARGE_INTEGER*)&start)
#define CHECK_TIME_END(a) QueryPerformanceCounter((LARGE_INTEGER*)&end); a = (float)((float)(end - start) / (freq / 1000.0f))
float compute_time;
float compute_time1, compute_time2;


#define MATDIM 1024
#define HW1_N 1000000
float *hw1_x, hw1_E, hw1_var1, hw1_var2;
float hw2_a, hw2_b, hw2_c, hw2_naive_ans[2], hw2_pre_ans[2];

/* hw1 */
void init_hw1(int fixed);
void hw1_calc_e();
void hw1_calc_var1();
void hw1_calc_var2();
/* hw2 */
void hw2_naive();
void hw2_safe();
float hw2_verify(float x);
/* hw3 */
#define HW3_MAX 1000000
int hw3_SR_ori[HW3_MAX];
int hw3_SR_opt[HW3_MAX];

float hw3_LI[100][100][100];
float hw3_LP_x[HW3_MAX], hw3_LP_y[HW3_MAX];

void hw3_Constant_Propagation();
void hw3_Strength_Reduction();
void hw3_Loop_Interchange();
void hw3_Loop_Peeling();
void hw3_Loop_Inversion();


void main(void)
{
	srand((unsigned)time(NULL));

	/* hw1 */
	puts("====== hw1 ======");
	init_hw1(1);
	CHECK_TIME_START;
	hw1_calc_var1();
	CHECK_TIME_END(compute_time);
	compute_time1 = compute_time;
	printf("hw1_calc_var1 = %f ms\n", compute_time);
	printf("hw1_calc_var1 value = %.15f\n", hw1_var1);


	CHECK_TIME_START;
	hw1_calc_e();
	hw1_calc_var2();
	CHECK_TIME_END(compute_time);
	compute_time2 = compute_time;
	printf("hw1_calc_var2 = %f ms\n", compute_time);
	printf("hw1_calc_var2 value = %.15f\n", hw1_var2);
	puts("");
	
	/* hw2 */
	puts("====== hw2 ======");
	printf("a, b, c : ");
	scanf("%f %f %f", &hw2_a, &hw2_b, &hw2_c);
	hw2_naive();
	printf("naive result    : %.15f, %.15f\n", hw2_naive_ans[0], hw2_naive_ans[1]);
	hw2_safe();
	printf("advanced result : %.15f, %.15f\n", hw2_pre_ans[0], hw2_pre_ans[1]);
	puts("");
	printf("Verifying naive ans    : %.15f, %.15f\n", hw2_verify(hw2_naive_ans[0]), hw2_verify(hw2_naive_ans[1]));
	printf("Verifying advanced ans : %.15f, %.15f\n", hw2_verify(hw2_pre_ans[0]), hw2_verify(hw2_pre_ans[1]));
	puts("");

	/* hw3 */
	printf("\nConststant Propagation\n");
	hw3_Constant_Propagation();
	
	printf("\nStrength Reduction\n");
	hw3_Strength_Reduction();
	
	printf("\nLoop Interchange\n");
	hw3_Loop_Interchange();

	printf("\nLoop Peeling\n");
	hw3_Loop_Peeling();

	printf("\nLoop Inversion\n");
	hw3_Loop_Inversion();
}

void init_hw1(int fixed)
{
	float *ptr;
	hw1_x = (float *)malloc(sizeof(float)*HW1_N);

	if (fixed)
	{
		float tmp = HW1_N;
		for (int i = 0; i < HW1_N; i++)
		{
			if( i & 1) tmp -= 0.0001;
			hw1_x[i] = tmp;
		}
	}
	else
	{
		srand((unsigned)time(NULL));
		ptr = hw1_x;
		for (int i = 0; i < HW1_N; i++)
			*ptr++ = ((float)rand() / (float) RAND_MAX) * 2;
	}
}
void hw1_calc_e()
{
	double tmp = 0;
	for (int i = 0; i < HW1_N; i++)
		tmp += hw1_x[i];
	hw1_E = (float)(tmp / (double)HW1_N);
}
void hw1_calc_var1()
{
	double hw1_x_sqr = 0, hw1_x_sum = 0;
	for (int i = 0; i < HW1_N; i++) {
		hw1_x_sum += hw1_x[i];
		hw1_x_sqr += hw1_x[i] * hw1_x[i];
	}
	hw1_var1 = (float)((hw1_x_sqr - (hw1_x_sum * hw1_x_sum) / (double)HW1_N) / ((double)HW1_N - 1.0));
}
void hw1_calc_var2()
{
	double hw1_x_sum = 0;

	for (int i = 0; i < HW1_N; i++) {
		hw1_x_sum += ((double)(hw1_x[i] - (double)hw1_E) * (double)(hw1_x[i] - (double)hw1_E));
	}
	hw1_var2 = (float)(hw1_x_sum / (double)(HW1_N - 1.0));
}


/* hw2 */
void hw2_naive()
{	
	//printf("hw2_naive start\n");
	//hw2_a, hw2_b, hw2_c
	double deter = sqrt(hw2_b * hw2_b - 4.0 * hw2_a * hw2_c);
	hw2_naive_ans[0] = (float)((-hw2_b + deter) / (2.0 * hw2_a));
	hw2_naive_ans[1] = (float)((-hw2_b - deter) / (2.0 * hw2_a));
	//printf("hw2_naive end\n");
}
void hw2_safe()
{
	// b^2 - 4ac < 0 이면 허수근 = 0 이면 중근
	double bb_4ac = hw2_b * hw2_b - 4.0 * hw2_a * hw2_c;
	double deter = sqrt(bb_4ac);
	if (bb_4ac <= 0) {
		hw2_pre_ans[0] = (float) (-hw2_b / (2 * hw2_a));
		hw2_pre_ans[1] = (float) (-hw2_b / (2 * hw2_a));
	}
	else {
		
		if (hw2_b > 0) {
			// b 양수, b^2
			hw2_pre_ans[0] = (-2.0 * hw2_c) / (hw2_b + deter);
			hw2_pre_ans[1] = (-hw2_b - deter)/(2.0 * hw2_a);
		}
		else {
			hw2_pre_ans[0] = (-hw2_b + deter) / (2.0 * hw2_a);
			hw2_pre_ans[1] = (2.0 * hw2_c) / (deter - hw2_b);
		}
	}

}
float hw2_verify(float x)
{
	return hw2_a * x * x + hw2_b*x + hw2_c;
}

/* hw3 */
//#pragma optimize("",off);
void hw3_Constant_Propagation() {
	// 메모리 로드 횟수를 줄이기 위해 값 자체를 사용하는 것이 효율적

	float x1, x2;
	float n1 = 0.33f, n2 = 0.33f, n3 = 0.33f, n4 = 0.33f, n5= 0.33f;
	float t;
	srand(time(NULL));
	t = rand() - RAND_MAX / 2.0f;
	x1 = t;
	x2 = t;
	//original 
	CHECK_TIME_START;
	for (int i = 0; i < (1 << 25); i++) {
		x1 = n1 * n2 * n3 * n4;
	}
	CHECK_TIME_END(compute_time);
	printf("Original time : %f ms\n", compute_time);

	//optimization
	CHECK_TIME_START;
	for (int i = 0; i < (1 << 25); i++) {
		x2 = 0.33f * 0.33f * 0.33f *0.33f;
	}
	CHECK_TIME_END(compute_time);
	printf("Optimization time : %f ms\n", compute_time);


}
//#pragma optimize("",on);
//#pragma optimize("",off);
void hw3_Strength_Reduction() {
	// bit shifting 연산이 곱셈, 나눗셈보다 빠름
	srand(time(NULL));
	
	for (int i = 0; i < HW3_MAX; i++) {
		hw3_SR_ori[i] = rand();
		hw3_SR_opt[i] = rand();
	}
	CHECK_TIME_START;
	for (int i = 0; i < HW3_MAX; i++) {
		hw3_SR_ori[i] = hw3_SR_ori[i] / 513;
		hw3_SR_ori[i] = hw3_SR_ori[i] * 513;
		hw3_SR_ori[i] = hw3_SR_ori[i] / 513;
		hw3_SR_ori[i] = hw3_SR_ori[i] * 513;
	}
	CHECK_TIME_END(compute_time);
	printf("Original time : %f ms\n", compute_time);

	CHECK_TIME_START;
	for (int i = 0; i < HW3_MAX; i++) {
		hw3_SR_opt[i] = hw3_SR_opt[i] >> 9;
		hw3_SR_opt[i] = hw3_SR_opt[i] << 9;
		hw3_SR_opt[i] = hw3_SR_opt[i] >> 9;
		hw3_SR_opt[i] = hw3_SR_opt[i] << 9;
	}
	CHECK_TIME_END(compute_time);
	printf("Optimization time : %f ms\n", compute_time);
}
//#pragma optimize("",on);
//#pragma optimize("",off);
void hw3_Loop_Interchange() {
	// cache 방문순서를 고려하여 선형으로 코드 구현
	CHECK_TIME_START;
	//hw3_LI
	for (int k = 0; k < 100; k++) {
		for (int i = 0; i < 100; i++) {
			for (int j = 0; j < 100; j++) {
				hw3_LI[j][i][k] = hw3_LI[j][i][k] + i * j * k;
			}
		}
	}
	CHECK_TIME_END(compute_time);
	printf("Original time : %f ms\n", compute_time);

	CHECK_TIME_START;
	//hw3_LI
	for (int i = 0; i < 100; i++) {
		for (int j = 0; j < 100; j++) {
			for (int k = 0; k < 1100; k++) {
				hw3_LI[i][j][k] = hw3_LI[i][j][k] + i * j * k;
			}
		}
	}
	CHECK_TIME_END(compute_time);
	printf("Optimization time : %f ms\n", compute_time);
}
void hw3_Loop_Peeling() {
	//반복문 내부의 동작을 최소화한다.
	//반복문 확장을 하지 않는다.
	srand(time(NULL));
	for (int i = 0; i < HW3_MAX; i++) {
		hw3_LP_x[i] = rand() - RAND_MAX / 2.0f;
	}

	CHECK_TIME_START;
	int tmp = HW3_MAX - 1;
	for (int i = 0; i < HW3_MAX; i++) {
		hw3_LP_y[i] = hw3_LP_x[i] +  hw3_LP_x[tmp];
		tmp = i;
	}
	CHECK_TIME_END(compute_time);
	printf("Original time : %f ms\n", compute_time);

	CHECK_TIME_START;
	hw3_LP_y[0] = hw3_LP_x[0] + hw3_LP_x[HW3_MAX - 1];
	for (int i = 1; i < HW3_MAX; i++) {
		hw3_LP_y[i] = hw3_LP_x[i] + hw3_LP_x[i-1];
	}
	CHECK_TIME_END(compute_time);
	printf("Optimization time : %f ms\n", compute_time);
}
void hw3_Loop_Inversion() {
	//while 비교 연산이 for문보다 빠르다.
	float x1, x2;
	srand(time(NULL));
	CHECK_TIME_START;
	x1 = 1;
	x2 = 0.00001;

	for (int i = 0; i < (1 << 25); i++) {
		x1 = x1 + x2;
	}
	CHECK_TIME_END(compute_time);
	printf("Original time : %f ms\n", compute_time);

	CHECK_TIME_START;
	x1 = 1;
	x2 = 0.00001;
	unsigned int i = 1 << 25;
	while(i-- ){
		x1 = x1 + x2;
	}
	CHECK_TIME_END(compute_time);
	printf("Optimization time : %f ms\n", compute_time);
}
//#pragma optimize("",on);