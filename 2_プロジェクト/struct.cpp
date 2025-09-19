#include "DxLib.h"
#include "struct.h"
#include "function.h"

void CAV::Reset()
{
	var = CON;
}

void Transform::DrawBoxT(size_t color, Screen scroll) // T = Transform
{
	DrawFillBox(pos.x - radius.x - scroll.x, pos.y - radius.y - scroll.y,
		pos.x + radius.x - scroll.x, pos.y + radius.y - scroll.y, color);
}

void Transform::DrawCircleT(size_t color, Screen scroll, bool fillFlag)
{
	DrawCircle(pos.x - scroll.x, pos.y - scroll.y, radius.x, color, fillFlag);
}

void Key::GetKeyState()
{
	for (int i = 0; i < 256; ++i) old[i] = now[i];
	GetHitKeyStateAll(now);
}

bool Key::PushButtion(int Buttion)
{
	if (!old[Buttion] && now[Buttion]) { return true; }
	return false;
}

void Object::MoveAndJump(Key keys, const int EDGE_MIN, const int EDGE_MAX,
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

void Object::Press(Key keys, const int GROUND_H, const int SHAKE_RANGE)
{
	if (keys.PushButtion(KEY_INPUT_DOWN) && jump.is)
	{
		jump.isFall = 1;
		pressPos[0] = transform.pos;
	}
	if (!jump.isFall) { return; }

	transform.pos.y += jump.fallSpd;
	pressPos[1] = transform.pos;

	if (transform.pos.y < GROUND_H) { return; }

	jump.isFall = 0;
	jump.spd.var = 0;
	transform.pos.y = GROUND_H;
	shake.is = 1;
	shake.range = SHAKE_RANGE;
}