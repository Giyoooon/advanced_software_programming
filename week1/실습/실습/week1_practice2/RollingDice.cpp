#include "pch.h"
#include "tipsware.h"
#include "time.h"
#define _USE_MATH_DEFINES
#include "math.h"

NOT_USE_MESSAGE

#define MAX_TRY 100
#define HEIGHT 20
#define WIDTH 5
int main()
{
	//srand(time(NULL));

	SelectPenObject(RGB(0, 0, 0));

	int hit = 0;
	int miss = 0;
	float relProb = 5.0 / 36.0;
	float prob = 0;
	
	// TODO 
	int dice1, dice2;
	for (int i = 0; i < HEIGHT; i++) {
		for (int j = 0; j < WIDTH; j++) {
			dice1 = rand() % 6 + 1;
			dice2 = rand() % 6 + 1;
			if (dice1 + dice2 == 8) {
				hit++;
				TextOut(120 * (i / HEIGHT) + j * 120, 20 * (i % HEIGHT), "try %d: %d,%d (hit!)", i + j * 5, dice1, dice2);
			}
			else {
				TextOut(120 * (i / HEIGHT) + j * 120, 20 * (i % HEIGHT), "try %d: %d,%d", i + j * 5, dice1, dice2);
				miss++;
			}
		}
	}
	prob = (float) hit / MAX_TRY;
	//TextOut(120 * (0 / 20), 20 * (0 % 20), "try %d: %d,%d (hit!)", 0 + 1, 6, 2);
	//TextOut(120 * (1 / 20), 20 * (1 % 20), "try %d: %d,%d", 0 + 2, 6, 6);
	
	//

	SetTextColor(RGB(128, 0, 0));
	// 실제 주사위 값
	TextOut(0, 20 * HEIGHT + 20, "실제 값: %.6f", relProb);
	// 계산된 값
	TextOut(0, 20 * HEIGHT + 40, "계산된 값: %.6f", prob);

	ShowDisplay();

	return 0;
}
