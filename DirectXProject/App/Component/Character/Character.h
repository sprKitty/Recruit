#pragma once
#include <App/Component/Component.h>
#include <App/TexAnimation.h>


class BillBoardRenderer;

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

	inline void SetBillBoardRenderer(const std::weak_ptr<BillBoardRenderer>& pBBR)
	{
		m_pBBR = std::move(pBBR);
	}

protected:
	virtual const bool Move() { return true; }
	virtual const bool  PointAtoB_AStar();
	const Chara_Direction::Kind CalcDirection(const float fDeg);

protected:
	std::vector<std::shared_ptr<TexAnimation> > m_pTexAnimList;
	std::weak_ptr<BillBoardRenderer> m_pBBR;  // BillBoardRendererコンポーネント
	Transform m_Transform;
	Chara_Direction::Kind m_Direction;

private:

};