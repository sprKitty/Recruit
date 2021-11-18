#pragma once
#include "Character.h"
#include <System/ClassDesign/StateBase.h>


namespace Player_State
{
	enum Kind
	{
		WAIT,
		WALK,
		ATTACK,
		SKILL1,
		SKILL2,

		MAX,
	};
}

class Player : public Character
{
public:
	Player() {}
	~Player()override {}

	void Init() override;
	void Uninit() override;
	void Update() override;

	void CalcDestination(const Vector3& vPos);

protected:
	void DestinationCollision();
	void Move()override;

private:
	const float OneSecMoveSpeed = 3.0f;

	Player_State::Kind m_State;
	Vector2 m_vDestination;
	Vector2 m_vMove;
	bool m_isMove;
};