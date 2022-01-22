  #pragma once
#include "Component.h"
#include <MyMath.h>
#include <App/Component/Object.h>


class Mesh;
class Transform;
class Object;

namespace CollisionType
{
	enum Kind
	{
		NONE,
		AABB,
		OBB,
		BC,
		RAY,
		MESH,

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


	/*
	* @brief 当たり判定の処理を切り替える
	* @param [ObjectType::Kind] typeO 相手のオブジェクトのタイプ
	* @param [ObjectType::Kind] typeO 当たり判定のタイプ
	*/
	inline void SetCollisionType(const ObjectType::Kind typeO, const CollisionType::Kind typeC = CollisionType::NONE)
	{
		if (typeO < m_typeList.size())
		{
			m_typeList[typeO] = typeC;
		}
	}

	inline void EnableMouseCollision()
	{
		m_isMouse = true;
	}

	inline void DisableMouseCollision()
	{
		m_isMouse = false;
	}

	inline const bool IsMouseCollision()const
	{
		return m_isMouse;
	}

	inline void EnableHitType(const CollisionType::Kind type)
	{
		m_HitInfoList[type].isFlg = true;
	}

	inline void SetHitObject(const CollisionType::Kind type, const std::weak_ptr<Object>& pObj)
	{
		m_HitInfoList[type].pObj = pObj;
	}

	inline const CollisionType::Kind GetCollisionType(const ObjectType::Kind type)
	{
		if (type < m_typeList.size())
		{
			return m_typeList[type];
		}
		return CollisionType::NONE;
	}

	inline const HitInfo& GetHitInfo(const CollisionType::Kind type)
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

	inline const Vector3& GetScaleDeviation()const
	{
		return m_vScaleDeviation;
	}

	inline const Vector3& GetAngleDeviation()const
	{
		return m_vAngleDeviation;
	}

	inline const Vector3& GetPosDeviation()const
	{
		return m_vPosDeviation;
	}

private:
	std::vector<CollisionType::Kind> m_typeList;
	std::vector<HitInfo> m_HitInfoList;
	std::weak_ptr<Mesh> m_pMesh;
	std::weak_ptr<Transform> m_pTransform;
	Vector3 m_vPosDeviation;
	Vector3 m_vAngleDeviation;
	Vector3 m_vScaleDeviation;
	bool m_isMouse;
};