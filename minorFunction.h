#pragma once
// �g�p�p�x���Ⴂ�֐�

// ������2��̊֐�
int Square(int num)
{
	return num * num;
}

// �������[�v������֐�
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