#pragma once

// �f�o�b�O�p�Ɏg���܂�(�A��int�^�̂�)
void DrawDebugNumber(int num, int y = 0)
{
	DrawFormatString(0, y, GetColor(255, 255, 255), "%d", num);
}