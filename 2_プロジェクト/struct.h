#pragma once
// ���W�p�ϐ�
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

	// �L�[���𓾂�
	void GetKeyState();

	// �L�[���������u��true�ɂȂ�
	bool PushButtion(int Buttion);
};

class Shake
{
public:
	int range;	  // �V�F�C�N�͈̔͋L�^
	bool is = 0; // �V�F�C�N����?
	Screen num = { 0,0 };   // �V�F�C�N�l�L�^
	int time = 0;	  // �o�ߎ��ԋL�^

	// shakeFlag == 0 �c���ɗh���
	// shakeFlag == 1 ���ɗh���
	// shakeFlag == 2 �c�ɗh���
	void Func(const int SHAKE_INTERVAL, int shakeFlag)
	{
		if (!is) return;

		// �V�F�C�N����莞�Ԃ��ƂɊɂ₩�ɂȂ�
		time++;
		if (time >= SHAKE_INTERVAL)
		{
			time = 0;
			range /= 2;
		}
		// �V�F�C�N�l����
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
		// �V�F�C�N�I���
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