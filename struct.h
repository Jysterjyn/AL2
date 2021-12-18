#pragma once
// ���W�p�ϐ�
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

// �V�F�C�N�Ɏg��
class Shake
{
public:
	int range;	  // �V�F�C�N�͈̔͋L�^
	bool isShake = 0; // �V�F�C�N����?
	Screen num = { 0,0 };   // �V�F�C�N�l�L�^
	int time = 0;	  // �o�ߎ��ԋL�^

	void Func(const int SHAKE_RANGE, const int SHAKE_INTERVAL)
	{
		if (!isShake) return;

		// �V�F�C�N����莞�Ԃ��ƂɊɂ₩�ɂȂ�
		time++;
		if (time >= SHAKE_INTERVAL)
		{
			time = 0;
			range /= 2;
		}
		// �V�F�C�N�l����
		num.x = rand() % (range + 1) - range / 2;
		num.y = rand() % (range + 1) - range / 2;
		// �V�F�C�N�I���
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

	// �L�[���𓾂�
	void GetKeyState()
	{
		for (int i = 0; i < 256; ++i) old[i] = now[i];
		GetHitKeyStateAll(now);
	}

	// �L�[���������u��true�ɂȂ�
	bool PushButtion(int Buttion)
	{
		if (!old[Buttion] && now[Buttion]) { return true; }
		return false;
	}
};