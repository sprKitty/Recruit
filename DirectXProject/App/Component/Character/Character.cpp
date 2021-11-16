#include "Character.h"

void Character::PointAtoB_AStar()
{
}

const Chara_Direction::Kind Character::CalcDirection(const float fDeg)
{
	const float fDir = 22.5f;

	if ((fDeg > -fDir) && (fDeg <= fDir))
	{
		return Chara_Direction::RIGHT;
	}
	else if ((fDeg > fDir) && (fDeg <= fDir * 3))
	{
		return Chara_Direction::UP_RIGHT;
	}
	else if ((fDeg > fDir * 3) && (fDeg <= fDir * 5))
	{
		return Chara_Direction::UP;
	}
	else if ((fDeg > fDir * 5) && (fDeg <= fDir * 7))
	{
		return Chara_Direction::UP_LEFT;
	}
	else if ((fDeg > -fDir * 3) && (fDeg <= -fDir))
	{
		return Chara_Direction::DOWN_RIGHT;

	}
	else if ((fDeg > -fDir * 5) && (fDeg <= -fDir * 3))
	{
		return Chara_Direction::DOWN;

	}
	else if ((fDeg > -fDir * 7) && (fDeg <= -fDir * 5))
	{
		return Chara_Direction::DOWN_LEFT;

	}
	else if ((fDeg > fDir * 7) && (fDeg <= fDir * 8)
		|| (fDeg >= -fDir * 8) && (fDeg <= -fDir * 7))
	{
		return Chara_Direction::LEFT;
	}
	return Chara_Direction::MAX;
}
