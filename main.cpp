#include <math.h>
#include "DxLib.h"
#include "enum.h"
#include "minorFunction.h"
#include "struct.h"
#include "function.h"

// ウィンドウのサイズ
const Screen WIN_SIZE = { 800,800 };

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	ChangeWindowMode(TRUE);
	SetWindowSizeChangeEnableFlag(FALSE, FALSE);
	SetMainWindowText("演出王");
	SetGraphMode(WIN_SIZE.x, WIN_SIZE.y, 32);
	SetWindowSizeExtendRate(1.0);
	SetBackgroundColor(20, 20, 20);
	SetDrawScreen(DX_SCREEN_BACK);

	if (DxLib_Init() == -1)return -1;

	// ---フォントハンドル作成---


	// ---定数の宣言と初期化---
	// 色定数
	const size_t COLOR[] =
	{
		GetColor(255, 255, 255),GetColor(0, 0, 0),
		GetColor(222, 0, 0),GetColor(0, 222, 0),
		GetColor(0, 0, 222)
	};

	const int GROUND_HEIGHT = 700;

	const int PRESS_SHAKE_RANGE = 64;
	const int PRESS_SHAKE_INTERVAL = 6;

	const int J_CIRCLE_START_R = 16;
	const int J_CIRCLE_SPREAD_SPD = 6;

	const int PARTICLE_SPREAD_SPD = 4;
	const float PARTICLE_ROTATE_SPD = DX_PI_F / 36.0f;

	// ---変数の宣言と初期化---
	// キーボード情報用
	Key keys{};

	Object player =
	{
		{32,32,WIN_SIZE.x / 2,GROUND_HEIGHT - player.transform.radius.y},
		5,{{30},2,40},{}
	};

	Particle2 jumpCircle = { {10},{J_CIRCLE_START_R} };

	Particle particle = { 60 };

	for (size_t i = 0; i < PARTICLE_NUM; i++)
	{
		particle.Transform[i].radius.x = 6;
	}

	while (!(ProcessMessage() == -1 || CheckHitKey(KEY_INPUT_ESCAPE)))
	{
		ClearDrawScreen();
		keys.GetKeyState();

		// ---更新処理---
		player.MoveAndJump(keys, player.transform.radius.x, WIN_SIZE.x - player.transform.radius.x,
			GROUND_HEIGHT - player.transform.radius.y, player.transform.radius.y);
		//player.Jump(keys, GROUND_HEIGHT - player.transform.radius.y, player.transform.radius.y);
		player.Press(keys, GROUND_HEIGHT - player.transform.radius.y, PRESS_SHAKE_RANGE);
		player.shake.Func(PRESS_SHAKE_INTERVAL, 2);
		player.wave.Draw(240, 20, COLOR[RED]);

		if (keys.PushButtion(KEY_INPUT_SPACE))
		{
			particle.isExist = 1;
		}
		if (particle.isExist)
		{
			particle.time.var--;
			particle.dis += PARTICLE_SPREAD_SPD;
			particle.angle += PARTICLE_ROTATE_SPD;

			for (size_t i = 0; i < PARTICLE_NUM; i++)
			{
				particle.Transform[i].pos.x = player.transform.pos.x + particle.dis *
					cosf(DX_TWO_PI_F / (float)(PARTICLE_NUM) * (float)(i)+particle.angle);
				particle.Transform[i].pos.y = player.transform.pos.y + particle.dis *
					sinf(DX_TWO_PI_F / (float)(PARTICLE_NUM) * (float)(i)+particle.angle);
			}

			if (particle.time.var <= 0)
			{
				particle.isExist = 0;
				particle.time.Reset();
				particle.dis = 0;
				particle.angle = 0.0f;
			}
		}

		if (keys.PushButtion(KEY_INPUT_UP))
		{
			jumpCircle.isExist = 1;
			jumpCircle.time.Reset();
			jumpCircle.Transform.radius.x = J_CIRCLE_START_R;
			jumpCircle.Transform.pos =
			{
				player.transform.pos.x,
				player.transform.pos.y + player.transform.radius.y * 2
			};
		}
		if (jumpCircle.isExist)
		{
			jumpCircle.time.var--;
			jumpCircle.Transform.radius.x += J_CIRCLE_SPREAD_SPD;

			if (jumpCircle.time.var <= 0)
			{
				jumpCircle.time.Reset();
				jumpCircle.isExist = 0;
				jumpCircle.Transform.radius.x = J_CIRCLE_START_R;
			}
		}

		// ---描画処理---
		DrawFillBox(0+player.shake.num.x, GROUND_HEIGHT + player.shake.num.y,
			WIN_SIZE.x + player.shake.num.x, WIN_SIZE.y, COLOR[BLUE]);
		player.transform.DrawBoxT(COLOR[WHITE], player.shake.num);
		if (particle.isExist)
		{
			for (size_t i = 0; i < PARTICLE_NUM; i++)
			{
				particle.Transform[i].DrawCircleT(COLOR[RED]);
			}
		}
		if (jumpCircle.isExist) { jumpCircle.Transform.DrawCircleT(~WHITE, {}, 0); }

		ScreenFlip();
	}

	// 全リソースファイル削除
	InitGraph();
	InitSoundMem();

	DxLib_End();
	return 0;
}
