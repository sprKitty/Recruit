#pragma once
#include <System/ClassDesign/Singleton.h>
#include <App/Component/Collider.h>
#include <memory>
#include <vector>


class Mouse;
class Camera;
class Mesh;
class ShaderBuffer;

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
	void MouseColUpdate();
	void Draw(const std::weak_ptr<ShaderBuffer> pBuf);


	void AddCollider(const std::weak_ptr<Component>& pComponent);

	void ReleaseCollider(const std::weak_ptr<Component>& pComponent);

	inline void SetMouse(const std::weak_ptr<Mouse> pMouse) { m_pMouse = pMouse; }

protected:
	Collision() {}
	~Collision()override {}

private:
	static const int CHIELD_NUM = 4;
	const int SERCH_NUM = 3;
	class NODE
	{
	public:
		enum Type
		{
			NORMAL,
			LAST,
		};

	public:
		NODE()
			:m_type(NORMAL)
			, pParent(nullptr)
		{
			for (int i = 0; i < CHIELD_NUM; ++i)
			{
				pChiled[i] = nullptr;
			}
		}
		virtual ~NODE() {}

		NODE* pParent;
		NODE* pChiled[CHIELD_NUM];
		Vector2 min;
		Vector2 max;
		Type m_type;
		std::vector<std::weak_ptr<Collider> > pList;

	};

private:
	void DifferentListJudge(ColliderPtrList& pListA, ColliderPtrList& pListB);
	void SameListJudge(ColliderPtrList& pList);
	void AABB(const ColliderPtrList::iterator& itrA, const ColliderPtrList::iterator& itrB);
	void OBB(const ColliderPtrList::iterator& itrA, const ColliderPtrList::iterator& itrB);
	void BC(const ColliderPtrList::iterator& itrA, const ColliderPtrList::iterator& itrB);
	void RayMesh(const ColliderPtrList::iterator& itrRay, const ColliderPtrList::iterator& itrMesh);
	void MouseMesh(const ColliderPtrList::iterator& itrMesh);

	void Excute(std::vector<NODE*>& pParentList);
	void CreateNode(int nCnt, NODE* pNode, const Vector2& vMin, const Vector2& vMax);
	NODE* SecondSerchNode(const Vector2& vPos);
	NODE* FirstSerchNode(const std::weak_ptr<Collider>& pCollider, NODE* pNode, const Vector2& vPos);
	const bool ReSerchNode(const NODE* pNode, const Vector2& vPos);
	void FinalizeNode(int nCnt, NODE* pNode);
	void ClearNodeColliderList(int nCnt, NODE* pNode);
	void SetQuadTree();

private:
	ColliderPtrList m_pColliderList;
	std::weak_ptr<Mouse> m_pMouse;
	std::shared_ptr<Mesh> m_pCube;
	std::shared_ptr<Mesh> m_pSpheare;
	NODE* m_pNode[CHIELD_NUM];
};