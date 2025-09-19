#pragma once
// 座標用変数
typedef struct Screen
{
	int x;
	int y;
}Screen;

typedef struct ConstAndVar
{
	const int CON;
	int var = CON;

	void Reset();
}CAV;

typedef struct Jump
{
	CAV spd;
	int g;
	int fallSpd;
	bool is = 0, isFall = 0;
}Jump;

class Transform
{
public:
	Screen radius, pos;

	void DrawBoxT(size_t color, Screen scroll = { 0,0 }); // T = Transform

	void DrawCircleT(size_t color, Screen scroll = { 0,0 }, bool fillFlag = 1);
};

class Key
{
public:
	char now[256], old[256];

	// キー情報を得る
	void GetKeyState();

	// キーを押した瞬間trueになる
	bool PushButtion(int Buttion);
};

class Shake
{
public:
	int range;	  // シェイクの範囲記録
	bool is = 0; // シェイク中か?
	Screen num = { 0,0 };   // シェイク値記録
	int time = 0;	  // 経過時間記録

	// shakeFlag == 0 縦横に揺れる
	// shakeFlag == 1 横に揺れる
	// shakeFlag == 2 縦に揺れる
	void Func(const int SHAKE_INTERVAL, int shakeFlag)
	{
		if (!is) return;

		// シェイクが一定時間ごとに緩やかになる
		time++;
		if (time >= SHAKE_INTERVAL)
		{
			time = 0;
			range /= 2;
		}
		// シェイク値を代入
		switch (shakeFlag)
		{
			case 0:
			case 1:
				num.x = rand() % (range + 1) - range / 2;
				if (shakeFlag == 1) { break; }
			case 2:
				num.y = rand() % (range + 1) - range / 2;
				break;
		}
		// シェイク終わり
		if (range == 0) { is = 0; }
	}
};

typedef struct Object
{
	Transform transform;
	int spd;
	Jump jump;
	Shake shake;
	Screen pressPos[2]{};

	void MoveAndJump(Key keys, const int EDGE_MIN, const int EDGE_MAX,
		const int GROUND_H, const int CEILING_H);

	void Press(Key keys, const int GROUND_H, const int SHAKE_RANGE);
}Object;

const int PARTICLE_NUM = 8;
struct Particle
{
	CAV time;
	Transform Transform[PARTICLE_NUM];
	bool isExist;
	int dis;
	float angle;
};
struct Particle2
{
	CAV time;
	Transform Transform;
	bool isExist;
	int dis;
};