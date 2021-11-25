#pragma once
#include <System/ClassDesign/Singleton.h>
#include <memory>
#include <vector>
#include <App/Component/Collider.h>


class Mouse;
class Camera;

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

	inline void SetMouse(const std::weak_ptr<Mouse> pMouse)
	{
		m_pMouse = pMouse;
	}

protected:
	Collision() {}
	~Collision()override {}

private:
	void AABB(ColliderPtrList::iterator itrA, ColliderPtrList::iterator itrB);
	void OBB(ColliderPtrList::iterator itrA, ColliderPtrList::iterator itrB);
	void RayMesh(ColliderPtrList::iterator itrRay, ColliderPtrList::iterator itrMesh);
	void MouseMesh(ColliderPtrList::iterator itrMesh);

private:
	ColliderPtrList m_pColliderList;
	std::weak_ptr<Mouse> m_pMouse;
};