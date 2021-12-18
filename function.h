#pragma once

// デバッグ用に使います(但しint型のみ)
void DrawDebugNumber(int num, int y = 0)
{
	DrawFormatString(0, y, GetColor(255, 255, 255), "%d", num);
}

void Clamp(int& num, int min, int max)
{
	if (num < min) { num = min; }
	if (num > max) { num = max; }
}
