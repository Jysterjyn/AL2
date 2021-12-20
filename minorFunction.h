#pragma once
// 使用頻度が低い関数

// ただの2乗の関数
int Square(int num)
{
	return num * num;
}

// 数をループさせる関数
int NumberLoop(int num, int max, int min = 0)
{
	if (num >= max)num = min;
	if (num < min)num = max - 1;
	return num;
}

void Clamp(int& num, int min, int max)
{
	if (num < min) { num = min; }
	if (num > max) { num = max; }
}