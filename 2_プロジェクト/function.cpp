#include "function.h"

int NumberLoop(int num, int max, int min)
{
	if (num > max)num = min;
	if (num < min)num = max;
	return num;
}

void Clamp(int& num, int min, int max)
{
	if (num < min) { num = min; }
	if (num > max) { num = max; }
}