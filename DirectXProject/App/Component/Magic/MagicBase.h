#pragma once
#include <App/Component/Component.h>
#include <MyMath.h>


class Transform;
class Collider;

namespace MagicType
{
	enum Kind
	{
		FIRE,
		WATER,
		LEAF,

		MAX,
	};
}

class MagicBase : public Component
{
public:
	MagicBase() {}
	virtual ~MagicBase()override {}

	virtual void Init()override {}
	virtual void Uninit()override {}
	virtual void Update()override {}

	inline void SetType(const MagicType::Kind type) { m_magicType = type; }

protected:
	virtual const bool CollideUpdate() { return false; }

protected:
	MagicType::Kind m_magicType;
	std::weak_ptr<Transform> m_pTransform;
	std::weak_ptr<Collider> m_pCollider;

};
