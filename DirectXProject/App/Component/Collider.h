#pragma once
#include "Component.h"
#include <MyMath.h>

class Mesh;
class Transform;
class Object;

namespace Collision_Type
{
	enum Kind
	{
		AABB,
		OBB,
		BC,
		RAY,
		MESH,
		MOUSE,

		MAX,
	};
};

class Collider : public Component
{
public:
	struct HitInfo
	{
		bool isFlg;
		std::weak_ptr<Object> pObj;
	};

public:
	Collider() {}
	~Collider()override {}

	void Init()override;
	void Uninit()override;
	void Update()override;

	inline void EnableCollisionType(const Collision_Type::Kind type)
	{
		if (type < m_typeList.size())
		{
			m_typeList[type] = true;
		}
	}

	inline void DisableCollisionType(const Collision_Type::Kind type)
	{
		if (type < m_typeList.size())
		{
			m_typeList[type] = false;
		}
	}

	inline void EnableHitType(const Collision_Type::Kind type)
	{
		m_HitInfoList[type].isFlg = true;
	}

	inline void SetHitObject(const Collision_Type::Kind type, const std::weak_ptr<Object>& pObj)
	{
		m_HitInfoList[type].pObj = pObj;
	}

	inline const bool IsCollisionType(const Collision_Type::Kind type)
	{
		if (type < m_typeList.size())
		{
			return m_typeList[type];
		}
		return false;
	}

	inline const HitInfo& IsHitInfo(const Collision_Type::Kind type)
	{
		return m_HitInfoList[type];
	}

	inline const std::weak_ptr<Mesh>& GetMesh()
	{
		return m_pMesh;
	}

	const std::weak_ptr<Transform>& GetTransform()
	{
		return m_pTransform;
	}

	inline void SetPosDeviation(const Vector3& vPos)
	{
		m_vPosDeviation = vPos;
	}

	inline void SetAngleDeviation(const Vector3& vAngle)
	{
		m_vAngleDeviation = vAngle;
	}

	inline void SetScaleDeviation(const Vector3& vScale)
	{
		m_vScaleDeviation = vScale;
	}

	inline const DirectX::XMMATRIX GetDeviationMatrix()const
	{
		return MyMath::ConvertMatrix(m_vScaleDeviation, m_vAngleDeviation, m_vPosDeviation);
	}

private:
	std::vector<bool> m_typeList;
	std::vector<HitInfo> m_HitInfoList;
	std::weak_ptr<Mesh> m_pMesh;
	std::weak_ptr<Transform> m_pTransform;
	Vector3 m_vPosDeviation;
	Vector3 m_vAngleDeviation;
	Vector3 m_vScaleDeviation;
};