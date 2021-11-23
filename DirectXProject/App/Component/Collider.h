#pragma once
#include "Component.h"


class Mesh;

namespace Collision_Type
{
	enum Kind
	{
		AABB,
		OBB,
		RAY,
		MESH,

		MAX,
	};
};

class Collider : public Component
{
public:
	Collider() {}
	~Collider()override {}

	void Init()override;
	void Uninit()override;
	void Update()override;

	inline void EnableCollisionType(const Collision_Type::Kind type)
	{
		if (type >= m_typeList.size())
		{
			m_typeList[type] = true;
		}
	}

	inline void DisableCollisionType(const Collision_Type::Kind type)
	{
		if (type >= m_typeList.size())
		{
			m_typeList[type] = false;
		}
	}

	inline bool IsCollisionType(const Collision_Type::Kind type)
	{
		if (type >= m_typeList.size())
		{
			return m_typeList[type];
		}
		return false;
	}

	inline const std::weak_ptr<Mesh>& GetMesh()
	{
		return m_pMesh;
	}

private:
	std::vector<bool> m_typeList;
	std::weak_ptr<Mesh> m_pMesh;
};