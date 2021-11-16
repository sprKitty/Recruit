#pragma once
#include <Vector.h>


class Transform
{
public:
	Transform() :pos(0), rot(0), scale(1), move(0) {}
	~Transform() {}

	Vector3 pos;
	Vector3 rot;
	Vector3 scale;
	Vector3 move;

};