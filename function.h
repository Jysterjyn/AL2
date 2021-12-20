#pragma once

// デバッグ用に使います(但しint型のみ)
void DrawDebugNumber(int num, int y = 0)
{
	DrawFormatString(0, y, GetColor(255, 255, 255), "%d", num);
}