#pragma once
#include <App/Component/Component.h>


class Renderer3D;

namespace Chara_Direction
{
	enum Kind
	{
		UP,
		LEFT,
		RIGHT,
		DOWN,
		UP_LEFT,
		UP_RIGHT,
		DOWN_LEFT,
		DOWN_RIGHT,

		MAX,
	};
};

class Character : public Component
{
public:
	Character() {}
	~Character()override {}

	virtual void Init()override {}
	virtual void Uninit()override {}
	virtual void Update()override {}

protected:
	virtual void Move() = 0;
	virtual void PointAtoB_AStar();
	const Chara_Direction::Kind CalcDirection(const float fDeg);

private:

};