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
		//SKILL1,
		//SKILL2,

		MAX,
	};
}

class RootMotion;

class Player : public Character
{
public:
	Player() {}
	~Player()override {}

	void Init() override;
	void Uninit() override;
	void Update() override;

	void SetMousePos(const Vector3& vPos);
	void EnableChangeDestination();
	void EnableAttack();

protected:
	const bool Move()override;

private:
	const bool SetDestination();
	const bool CollisionMove();
	const bool CalcDestination();
	const bool DestinationCollision();
	const bool AttackAction();

	const int WaitStateChange();
	const int WalkStateChange();
	const int AttackStateChange();

private:
	const float OneSecMoveSpeed = 3.0f;

	std::weak_ptr<Object> m_pTalkOccurObj;
	std::weak_ptr<RootMotion> m_pRootMotion;
	State<Player>::PTRLIST m_pStateList;
	Player_State::Kind m_state;
	Vector3 m_vMousePos;
	Vector3 m_vDestination;
	Vector3 m_vMove;
	Vector3 m_vOldPos;
	bool m_isMove;
	bool m_isAttack;
	bool m_isChangeDestination;
};