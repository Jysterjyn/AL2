#include <math.h>
#include <stdlib.h>
#include <time.h>
#include "DxLib.h"
#include "enum.h"
#include "function.h"
#include "struct.h"

// ウィンドウのサイズ
const Screen WIN_SIZE = { 800,800 };

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	ChangeWindowMode(TRUE);
	SetWindowSizeChangeEnableFlag(FALSE, FALSE);
	SetMainWindowText("LC1B_17_ナカヤマ_シュウヤ");
	SetGraphMode(WIN_SIZE.x, WIN_SIZE.y, 32);
	SetWindowSizeExtendRate(1.0);
	SetBackgroundColor(20, 20, 20);
	SetDrawScreen(DX_SCREEN_BACK);

	if (DxLib_Init() == -1)return -1;

	// ---定数の宣言と初期化---
	// 色定数
	const size_t COLOR[] =
	{
		GetColor(255, 255, 255),GetColor(0, 0, 0),
		GetColor(222, 0, 0),GetColor(0, 222, 0),
		GetColor(0, 0, 222)
	};

	const int GROUND_HEIGHT = 700;

	const int PRESS_SHAKE[] = { 64,6 };

	const int JUMP_CIRCLE[] = { 16,6 };

	const int PARTICLE_SPREAD_SPD[2] = { 4,8 };
	const float PARTICLE_ROTATE_SPD = DX_PI_F / 18.0f;

	const int SPARK_NUM = 30;

	// ---変数の宣言と初期化---
	Key keys{};

	Screen mouse{};

	Object player =
	{
		{32,32,WIN_SIZE.x / 2,GROUND_HEIGHT - player.transform.radius.y},
		5,{{30},2,80},{}
	};

	int playerShape = BOX;

	Particle2 jumpCircle = { {10},{JUMP_CIRCLE[START_RADIUS]} };
	Screen sparkSpd[SPARK_NUM]{};
	Particle2 spark[SPARK_NUM] =
	{
		{20},
		{20},
		{20},
		{20},
		{20},
		{20},
		{20},
		{20},
		{20},
		{20},
		{20},
		{20},
		{20},
		{20},
		{20},
		{20},
		{20},
		{20},
		{20},
		{20},
		{20},
		{20},
		{20},
		{20},
		{20},
		{20},
		{20},
		{20},
		{20},
		{20},
	};
	int sparkTime = 0;
	int sparkNumNow = 0;

	for (size_t i = 0; i < SPARK_NUM; i++)
	{
		spark[i].Transform.radius = { 4,4 };
	}

	Particle particle[2] = { {120},{5} };

	for (size_t i = 0; i < PARTICLE_NUM; i++)
	{
		particle[PALSE].Transform[i].radius.x = 6;
		particle[CHANGE_SHAPE].Transform[i].radius.x = 6;
	}
	particle[PALSE].dis = player.transform.radius.x;

	while (!(ProcessMessage() == -1 || CheckHitKey(KEY_INPUT_ESCAPE)))
	{
		ClearDrawScreen();
		keys.GetKeyState();
		GetMousePoint(&mouse.x, &mouse.y);

		// ---更新処理---
		if (!player.jump.isFall)
		{
			player.MoveAndJump(keys, player.transform.radius.x, WIN_SIZE.x - player.transform.radius.x,
				GROUND_HEIGHT - player.transform.radius.y, player.transform.radius.y);
		}
		player.Press(keys, GROUND_HEIGHT - player.transform.radius.y, PRESS_SHAKE[RANGE]);
		player.shake.Func(PRESS_SHAKE[INTERVAL], 2);

		if (keys.PushButtion(KEY_INPUT_RETURN))
		{
			playerShape++;
			if (playerShape > CIRCLE) { playerShape = BOX; }
		}

		if (keys.PushButtion(KEY_INPUT_SPACE)) { particle[PALSE].isExist = 1; }
		if (particle[PALSE].isExist)
		{
			particle[PALSE].time.var--;
			particle[PALSE].dis += PARTICLE_SPREAD_SPD[PALSE];
			particle[PALSE].angle += PARTICLE_ROTATE_SPD;

			for (size_t i = 0; i < PARTICLE_NUM; i++)
			{
				particle[PALSE].Transform[i].pos.x = player.transform.pos.x + particle[PALSE].dis *
					cosf(DX_TWO_PI_F / (float)(PARTICLE_NUM) * (float)(i)+particle[PALSE].angle);
				particle[PALSE].Transform[i].pos.y = player.transform.pos.y + particle[PALSE].dis *
					sinf(DX_TWO_PI_F / (float)(PARTICLE_NUM) * (float)(i)+particle[PALSE].angle);
			}
		}
		if (particle[PALSE].time.var <= 0)
		{
			particle[PALSE].isExist = 0;
			particle[PALSE].time.Reset();
			particle[PALSE].dis = 0;
			particle[PALSE].angle = 0.0f;
		}

		if (keys.PushButtion(KEY_INPUT_RETURN)) { particle[CHANGE_SHAPE].isExist = 1; }
		if (particle[CHANGE_SHAPE].isExist)
		{
			particle[CHANGE_SHAPE].time.var--;
			particle[CHANGE_SHAPE].dis += PARTICLE_SPREAD_SPD[CHANGE_SHAPE];

			for (size_t i = 0; i < PARTICLE_NUM; i++)
			{
				particle[CHANGE_SHAPE].Transform[i].pos.x = player.transform.pos.x + particle[CHANGE_SHAPE].dis *
					cosf(DX_TWO_PI_F / (float)(PARTICLE_NUM) * (float)(i));
				particle[CHANGE_SHAPE].Transform[i].pos.y = player.transform.pos.y + particle[CHANGE_SHAPE].dis *
					sinf(DX_TWO_PI_F / (float)(PARTICLE_NUM) * (float)(i));
			}
		}
		if (particle[CHANGE_SHAPE].time.var <= 0)
		{
			particle[CHANGE_SHAPE].isExist = 0;
			particle[CHANGE_SHAPE].time.Reset();
			particle[CHANGE_SHAPE].dis = player.transform.radius.x;
		}

		if (keys.PushButtion(KEY_INPUT_UP) && !player.jump.isFall)
		{
			jumpCircle.isExist = 1;
			jumpCircle.time.Reset();
			jumpCircle.Transform.radius.x = JUMP_CIRCLE[START_RADIUS];
			jumpCircle.Transform.pos =
			{
				player.transform.pos.x,
				player.transform.pos.y + player.transform.radius.y * 2
			};
		}
		if (jumpCircle.isExist)
		{
			jumpCircle.time.var--;
			jumpCircle.Transform.radius.x += JUMP_CIRCLE[SPREAD_SPD];
		}
		if (jumpCircle.time.var <= 0)
		{
			jumpCircle.time.Reset();
			jumpCircle.isExist = 0;
			jumpCircle.Transform.radius.x = JUMP_CIRCLE[START_RADIUS];
		}

		if (GetMouseInput() & MOUSE_INPUT_LEFT)
		{
			spark[sparkNumNow].isExist = 1;
			spark[sparkNumNow].Transform.pos = mouse;
			sparkSpd[sparkNumNow] = { rand() % 11 - 5,rand() % 11 - 5 };
			++sparkNumNow = NumberLoop(sparkNumNow, SPARK_NUM - 1);
		}
		for (size_t i = 0; i < SPARK_NUM; i++)
		{
			if (spark[i].isExist)
			{
				spark[i].time.var--;
				spark[i].Transform.pos.x += sparkSpd[i].x;
				spark[i].Transform.pos.y += sparkSpd[i].y;

				if (spark[i].time.var < 0)
				{
					spark[i].time.Reset();
					spark[i].isExist = 0;
				}
			}
		}

		if (keys.PushButtion(KEY_INPUT_R))
		{
			player.transform.pos =
			{
				WIN_SIZE.x / 2,GROUND_HEIGHT - player.transform.radius.y
			};
		}
		// ---描画処理---
		DrawFillBox(0 + player.shake.num.x, GROUND_HEIGHT + player.shake.num.y,
			WIN_SIZE.x + player.shake.num.x, WIN_SIZE.y, COLOR[BLUE]);
		if (jumpCircle.isExist) { jumpCircle.Transform.DrawCircleT(COLOR[WHITE], {}, 0); }
		if (player.jump.isFall)
		{
			DrawLine(player.pressPos[0].x, player.pressPos[0].y,
				player.pressPos[1].x, player.pressPos[1].y, COLOR[RED], 32);
		}
		switch (playerShape)
		{
			case BOX:
				player.transform.DrawBoxT(COLOR[WHITE], player.shake.num);
				break;
			case CIRCLE:
				player.transform.DrawCircleT(COLOR[WHITE], player.shake.num);
				break;
		}
		for (size_t i = 0; i < 2; i++)
		{
			if (particle[i].isExist)
			{
				for (size_t j = 0; j < PARTICLE_NUM; j++)
				{
					particle[i].Transform[j].DrawCircleT(COLOR[RED + i]);
				}
			}
		}
		DrawCircle(mouse.x, mouse.y, 8, COLOR[RED]);
		for (size_t i = 0; i < SPARK_NUM; i++)
		{
			if (spark[i].isExist)
			{
				spark[i].Transform.DrawBoxT(COLOR[RED]);
			}
		}

		DrawString(20, 720,
			"←→ 移動\n↑ ジャンプ\n↓ プレス\nSPACE 波動っぽいもの",
			COLOR[WHITE]);
		DrawString(220, 720,
			"ENTER 自機の変形\nマウス左長押し 俺(れ)の体から美(つく)しい火花が散るぜ\nR リセット",
			COLOR[WHITE]);

		ScreenFlip();
	}

	DxLib_End();
	return 0;
}
