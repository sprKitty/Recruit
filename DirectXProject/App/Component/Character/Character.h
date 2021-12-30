#pragma once
#include <App/Component/Component.h>
#include <App/TexAnimation.h>


class BillBoardRenderer;
class Transform;
class Collider;

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
	virtual const bool Move() { return true; }
	virtual const bool  PointAtoB_AStar();
	const Chara_Direction::Kind CalcDirection8(const float fDeg);
	const Chara_Direction::Kind CalcDirection4(const float fDeg);
	void SetNeedComponent();
protected:
	std::vector<std::shared_ptr<TexAnimation> > m_pTexAnimList;
	std::weak_ptr<BillBoardRenderer> m_pBBR;  // BillBoardRendererコンポーネント
	std::weak_ptr<Transform> m_pTransform;
	std::weak_ptr<Collider> m_pCollider;
	Chara_Direction::Kind m_Direction;

private:

};