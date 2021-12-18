#pragma once
// 座標用変数
typedef struct Screen
{
	int x;
	int y;
}Screen;

class Transform
{
public:
	Screen radius;
	Screen pos;

	bool BoxCollision(Transform obj)
	{
		return
			abs(pos.x - obj.pos.x) <= radius.x + obj.radius.x &&
			abs(pos.y - obj.pos.y) <= radius.y + obj.radius.y;
	}

	bool CircleCollision(Transform obj)
	{
		return
			Square(pos.x - obj.pos.x) + Square(pos.y - obj.pos.y) <=
			radius.x + obj.radius.x;
	}

	void DrawBoxT(size_t color, Screen scroll = { 0,0 }) // T = Transform
	{
		DrawFillBox(pos.x - radius.x - scroll.x, pos.y - radius.y - scroll.y,
			pos.x + radius.x - scroll.x, pos.y + radius.y - scroll.y, color);
	}

	void DrawCircleT(size_t color, Screen scroll = { 0,0 })
	{
		DrawCircle(pos.x - scroll.x, pos.y - scroll.y, radius.x, color);
	}
};

typedef struct ConstAndVar
{
	const int con;
	int var = con;

	void Reset()
	{
		var = con;
	}
}CAV;

// シェイクに使う
class Shake
{
public:
	int range;	  // シェイクの範囲記録
	bool isShake = 0; // シェイク中か?
	Screen num = { 0,0 };   // シェイク値記録
	int time = 0;	  // 経過時間記録

	void Func(const int SHAKE_RANGE, const int SHAKE_INTERVAL)
	{
		if (!isShake) return;

		// シェイクが一定時間ごとに緩やかになる
		time++;
		if (time >= SHAKE_INTERVAL)
		{
			time = 0;
			range /= 2;
		}
		// シェイク値を代入
		num.x = rand() % (range + 1) - range / 2;
		num.y = rand() % (range + 1) - range / 2;
		// シェイク終わり
		if (range == 0) { isShake = 0; }
	}
};

class Particle
{

};

class Key
{
public:
	char now[256];
	char old[256];

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