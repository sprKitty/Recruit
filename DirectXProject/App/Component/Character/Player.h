#pragma once
#include "Character.h"
#include <System/ClassDesign/State.h>

class Image;
class BillBoardRenderer;

class Player : public Character
{
public:
	enum class State
	{
		WAIT,
		WALK,
		ATTACK,
		SKILL1,
		SKILL2,

		MAX,
	};

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
	State m_State;
	Vector2 m_vDestination;
	Vector2 m_vMove;
	Transform m_Transform;
	const float OneSecMoveSpeed = 3.0f;

	std::vector<std::shared_ptr<Image> > m_pImageList;
	std::weak_ptr<BillBoardRenderer> m_pBBR;  // BillBoardRendererコンポーネント
};