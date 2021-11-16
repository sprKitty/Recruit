#pragma once
#include "Character.h"
#include <System/ClassDesign/State.h>

class TexAnimation;
class BillBoardRenderer;

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

	inline void SetBillBoardRenderer(const std::weak_ptr<BillBoardRenderer>& pBBR)
	{
		m_pBBR = std::move(pBBR);
	}

protected:
	void DestinationCollision();
	void Move()override;

private:
	Player_State::Kind m_State;
	Chara_Direction::Kind m_Direction;
	Vector2 m_vDestination;
	Vector2 m_vMove;
	int m_nAnimFrame;
	Transform m_Transform;
	const float OneSecMoveSpeed = 3.0f;

	std::vector<std::shared_ptr<TexAnimation> > m_pTexAnimList;
	std::weak_ptr<BillBoardRenderer> m_pBBR;  // BillBoardRendererコンポーネント
};