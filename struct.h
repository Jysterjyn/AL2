#pragma once
// 座標用変数
typedef struct Screen
{
	int x;
	int y;
}Screen;

typedef struct ConstAndVar
{
	const int con;
	int var = con;

	void Reset()
	{
		var = con;
	}
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

	//bool BoxCollision(Transform obj)
	//{
	//	return
	//		abs(pos.x - obj.pos.x) <= radius.x + obj.radius.x &&
	//		abs(pos.y - obj.pos.y) <= radius.y + obj.radius.y;
	//}
	//bool CircleCollision(Transform obj)
	//{
	//	return
	//		Square(pos.x - obj.pos.x) + Square(pos.y - obj.pos.y) <=
	//		radius.x + obj.radius.x;
	//}

	void DrawBoxT(size_t color, Screen scroll = { 0,0 }) // T = Transform
	{
		DrawFillBox(pos.x - radius.x - scroll.x, pos.y - radius.y - scroll.y,
			pos.x + radius.x - scroll.x, pos.y + radius.y - scroll.y, color);
	}

	void DrawCircleT(size_t color, Screen scroll = { 0,0 }, bool fillFlag = 1)
	{
		DrawCircle(pos.x - scroll.x, pos.y - scroll.y, radius.x, color, fillFlag);
	}
};

class Key
{
public:
	char now[256], old[256];

	// キー情報を得る
	void GetKeyState()
	{
		for (int i = 0; i < 256; ++i) old[i] = now[i];
		GetHitKeyStateAll(now);
	}

	// キーを押した瞬間trueになる
	bool PushButtion(int Buttion)
	{
		if (!old[Buttion] && now[Buttion]) { return true; }
		return false;
	}
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

struct Wave
{
	Screen pos[2];
	int arrive;
	bool isExist;

	void Draw(int waveLength, int waveSpd, int waveColor)
	{
		if (!isExist) { return; }

		arrive += waveSpd;
		if (arrive > waveLength)
		{
			isExist = 0;
			arrive = 0;
		}

		for (size_t i = 0; i < arrive; i++)
		{
			DrawLine(pos[0].x - i, pos[0].y,
				pos[0].x - 32 - i, pos[0].y - 32,
				waveColor);
			DrawLine(pos[1].x + i, pos[1].y,
				pos[1].x + 32 + i, pos[1].y - 32,
				waveColor);
		}
	}
};

typedef struct Object
{
	Transform transform;
	int spd;
	Jump jump;
	Shake shake;
	Wave wave;

	void MoveAndJump(Key keys, const int EDGE_MIN, const int EDGE_MAX,
		const int GROUND_H, const int CEILING_H)
	{
		transform.pos.x += (keys.now[KEY_INPUT_RIGHT] - keys.now[KEY_INPUT_LEFT]) * spd;
		Clamp(transform.pos.x, EDGE_MIN, EDGE_MAX);

		if (keys.PushButtion(KEY_INPUT_UP))
		{
			jump.is = 1;
			jump.spd.Reset();
		}
		if (jump.is)
		{
			transform.pos.y -= jump.spd.var;
			jump.spd.var -= jump.g;
		}
		if (transform.pos.y <= CEILING_H)
		{
			transform.pos.y = CEILING_H;
			jump.spd.var = 0;
			jump.spd.var -= jump.g;
			transform.pos.y -= jump.spd.var;
		}
		if (transform.pos.y >= GROUND_H)
		{
			jump.is = 0;
			transform.pos.y = GROUND_H;
			jump.spd.Reset();
		}
	}

	void Press(Key keys, const int GROUND_H, const int SHAKE_RANGE)
	{
		if (keys.PushButtion(KEY_INPUT_DOWN) && jump.is)
		{
			jump.isFall = 1;
		}
		if (!jump.isFall) { return; }

		transform.pos.y += jump.fallSpd;

		if (!(transform.pos.y >= GROUND_H)) { return; }

		jump.isFall = 0;
		jump.spd.var = 0;
		transform.pos.y = GROUND_H;
		shake.is = 1;
		shake.range = SHAKE_RANGE;
		wave.pos[0] =
		{
			transform.pos.x - transform.radius.x,
			transform.pos.y + transform.radius.y
		};
		wave.pos[1] =
		{
			transform.pos.x + transform.radius.x,
			transform.pos.y + transform.radius.y
		};
		wave.isExist = 1;
		wave.arrive = 0;
	}
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