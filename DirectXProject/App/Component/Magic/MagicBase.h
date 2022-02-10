#pragma once
#include <App/Component/Component.h>
#include <System/Clocker.h>
#include <System/ClassDesign/Property.h>
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
	MagicBase()
	: m_fCoolTime(0.f)
	, m_fTime(0.f){}
	virtual ~MagicBase()override {}

	virtual void Init()override {}
	virtual void Uninit()override {}
	virtual void Update()override {}

	inline void SetType(const MagicType::Kind type) { m_magicType = type; }
	inline void UpdateCoolTime() { m_fTime += Clocker::GetInstance().DeltaTime(); }
	inline bool IsCoolTime() { return m_fTime < m_fCoolTime; }

protected:
	virtual const bool CollideUpdate() { return false; }

protected:
	MagicType::Kind m_magicType;
	std::weak_ptr<Transform> m_pTransform;
	std::weak_ptr<Collider> m_pCollider;
	
	float m_fTime;
	float m_fCoolTime; // クールタイム
};
