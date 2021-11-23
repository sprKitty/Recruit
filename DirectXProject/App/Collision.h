#pragma once
#include <System/ClassDesign/Singleton.h>
#include <memory>
#include <vector>
#include <App/Component/Collider.h>


class Collision : public Singleton<Collision>
{
public:
	friend class Singleton<Collision>;

private:
	using ColliderPtrList = std::vector<std::weak_ptr<Collider> >;

public:
	void Initialize()override;
	void Finalize()override;
	void Update();

	void AddCollider(const std::weak_ptr<Component>& pComponent);

	void ReleaseCollider(const std::weak_ptr<Component>& pComponent);

protected:
	Collision() {}
	~Collision()override {}

private:
	void AABB(ColliderPtrList::iterator itrA, ColliderPtrList::iterator itrB);
	void OBB(ColliderPtrList::iterator itrA, ColliderPtrList::iterator itrB);
	void RayMesh(ColliderPtrList::iterator itrRay, ColliderPtrList::iterator itrMesh);

private:
	ColliderPtrList m_pColliderList;
};