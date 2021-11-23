#include "Collision.h"


void Collision::Initialize()
{
}

void Collision::Finalize()
{
}

void Collision::Update()
{
	for (ColliderPtrList::iterator itrX = m_pColliderList.begin(); itrX != m_pColliderList.end();)
	{
		for (int i = 0; i < Collision_Type::MAX; ++i)
		{
			if (!itrX->lock()->IsCollisionType(static_cast<Collision_Type::Kind>(i)))continue;

			for (ColliderPtrList::iterator itrY = itrX; itrY != m_pColliderList.end();)
			{
				for (int j = 0; j < Collision_Type::MAX; ++j)
				{
					if (!itrY->lock()->IsCollisionType(static_cast<Collision_Type::Kind>(j)))continue;

					switch (static_cast<Collision_Type::Kind>(i))
					{
					case Collision_Type::AABB:
						if (static_cast<Collision_Type::Kind>(j)
							== Collision_Type::AABB)
						{
							AABB(itrX, itrY);
						}
						break;
					case Collision_Type::OBB:
						if (static_cast<Collision_Type::Kind>(j)
							== Collision_Type::OBB)
						{
							OBB(itrX, itrY);
						}
						break;
					case Collision_Type::RAY:
						if (static_cast<Collision_Type::Kind>(j)
							== Collision_Type::MESH)
						{
							RayMesh(itrX, itrY);
						}
						break;
					case Collision_Type::MESH:
						if (static_cast<Collision_Type::Kind>(j)
							== Collision_Type::RAY)
						{
							RayMesh(itrY, itrX);
						}
						break;
					default:
						break;
					}

				}


				if (itrY == m_pColliderList.end())
				{
					break;
				}
				++itrY;
			}
		}
		if (itrX == m_pColliderList.end())
		{
			break;
		}
		++itrX;
	}
}

void Collision::AddCollider(const std::weak_ptr<Component>& pComponent)
{
	std::weak_ptr<Collider> pRenderer = std::dynamic_pointer_cast<Collider>(pComponent.lock());
	m_pColliderList.push_back(pRenderer);
}

void Collision::ReleaseCollider(const std::weak_ptr<Component>& pComponent)
{
	if (pComponent.expired())return;

	for (ColliderPtrList::iterator itr = m_pColliderList.begin(); itr != m_pColliderList.end();)
	{
		if (itr->expired())continue;

		if (itr->lock() == pComponent.lock())
		{
			itr = m_pColliderList.erase(itr);
		}

		if (itr == m_pColliderList.end())break;

		++itr;
	}
}

void Collision::AABB(ColliderPtrList::iterator itrA, ColliderPtrList::iterator itrB)
{
}

void Collision::OBB(ColliderPtrList::iterator itrA, ColliderPtrList::iterator itrB)
{
}

void Collision::RayMesh(ColliderPtrList::iterator itrRay, ColliderPtrList::iterator itrMesh)
{
}
