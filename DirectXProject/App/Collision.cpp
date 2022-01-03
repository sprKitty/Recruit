#include "Collision.h"
#include <MyMath.h>
#include <App/Component/Mesh.h>
#include <App/Component/Transform.h>
#include <App/Camera.h>
#include <System/Mouse.h>
#include <System/DebugLog.h>
#include <Shader/ShaderBuffer.h>

void Collision::Initialize()
{
#ifdef _DEBUG
	m_pCube.reset(new Mesh());
	m_pCube->Set("cube");
	m_pSpheare.reset(new Mesh());
	m_pSpheare->Set("sphere");
#endif // _DEBUG
	Vector2 first(150.0f, 150.0f);
	int nCnt = 0;
	++nCnt;
	for (int i = 0; i < CHIELD_NUM; ++i)
	{
		m_pNode[i] = new NODE();
	}
	m_pNode[0]->min = { -first.x,-first.y };
	m_pNode[0]->max = { 0,0 };
	m_pNode[1]->min = { 0,-first.y };
	m_pNode[1]->max = { first.x,0 };
	m_pNode[2]->min = { -first.x,0 };
	m_pNode[2]->max = { 0,first.y };
	m_pNode[3]->min = { 0,0 };
	m_pNode[3]->max = { first.x,first.y };
	CreateNode(nCnt, m_pNode[0], m_pNode[0]->min, m_pNode[0]->max);
	CreateNode(nCnt, m_pNode[1], m_pNode[1]->min, m_pNode[1]->max);
	CreateNode(nCnt, m_pNode[2], m_pNode[2]->min, m_pNode[2]->max);
	CreateNode(nCnt, m_pNode[3], m_pNode[3]->min, m_pNode[3]->max);
}

void Collision::Finalize()
{
	for (int i = 0; i < CHIELD_NUM; ++i)
	{
		FinalizeNode(0, m_pNode[i]);
	}
}

void Collision::Update()
{
	SetQuadTree();
	for (int i = 0; i < CHIELD_NUM; ++i)
	{
		std::vector<NODE*> pNodeList;
		pNodeList.emplace_back(m_pNode[i]);
		Excute(pNodeList);
		SameListJudge(m_pNode[i]->pList);
		ClearNodeColliderList(0, m_pNode[i]);
	}

	for (ColliderPtrList::iterator itrA = m_pColliderList.begin(); itrA != m_pColliderList.end();)
	{
		if (itrA->lock()->m_pOwner.lock()->IsActive())
		{
			if (itrA->lock()->IsMouseCollision())
			{
				MouseMesh(itrA);
			}
			//for (ColliderPtrList::iterator itrB = itrA; itrB != m_pColliderList.end();)
			//{
			//	++itrB;
			//	if (itrB == m_pColliderList.end())
			//	{
			//		break;
			//	}
			//	if (itrB->lock()->m_pOwner.lock()->IsActive())
			//	{
			//		CollisionType::Kind typeA = itrA->lock()->GetCollisionType(itrB->lock()->m_pOwner.lock()->GetType());
			//		CollisionType::Kind typeB = itrB->lock()->GetCollisionType(itrA->lock()->m_pOwner.lock()->GetType());
			//		switch (typeA)
			//		{
			//		case CollisionType::AABB:
			//			if (typeB == CollisionType::AABB)
			//			{
			//				AABB(itrA, itrB);
			//			}
			//			break;
			//		case CollisionType::OBB:
			//			if (typeB == CollisionType::OBB)
			//			{
			//				OBB(itrA, itrB);
			//			}
			//			break;
			//		case CollisionType::BC:
			//			if (typeB == CollisionType::BC)
			//			{
			//				BC(itrA, itrB);
			//			}
			//			break;
			//		case CollisionType::RAY:
			//			if (typeB == CollisionType::MESH)
			//			{
			//				RayMesh(itrA, itrB);
			//			}
			//			break;
			//		case CollisionType::MESH:
			//			if (typeB == CollisionType::RAY)
			//			{
			//				RayMesh(itrB, itrA);
			//			}
			//			break;
			//		case CollisionType::NONE:
			//		case CollisionType::MAX:
			//		default:
			//			continue;
			//		}
			//	}
			//}
		}
		++itrA;
		if (itrA == m_pColliderList.end())
		{
			break;
		}
	}
}

void Collision::Draw(const std::weak_ptr<ShaderBuffer> pBuf)
{
#ifdef _DEBUG
	for (const auto& itr : m_pColliderList)
	{
		if (itr.expired())continue;
		if (!itr.lock()->m_pOwner.lock()->IsActive())continue;
		std::weak_ptr<Transform> pTrans = itr.lock()->GetTransform();
		if (pTrans.expired())continue;

		DirectX::XMMATRIX mtx = pTrans.lock()->GetWorldMatrix(itr.lock()->GetScaleDeviation(), itr.lock()->GetAngleDeviation(), itr.lock()->GetPosDeviation());
		//DirectX::XMVECTOR s, q, p;
		//DirectX::XMMatrixDecompose(&s, &q, &p, mtx);
		//DirectX::XMFLOAT3 vScale, vPos;
		//DirectX::XMStoreFloat3(&vScale, s);
		//DirectX::XMStoreFloat3(&vPos, p);
		//mtx = MyMath::ConvertMatrix(Vector3().Convert(vScale), 0, Vector3().Convert(vPos));
		pBuf.lock()->SetWorld(mtx);
		pBuf.lock()->BindVS(VS_TYPE::NORMAL);
		pBuf.lock()->BindPS(PS_TYPE::COLOR);
		for (int i = 0; i < ObjectType::MAX; ++i)
		{
			switch (itr.lock()->GetCollisionType(static_cast<ObjectType::Kind>(i)))
			{
			case CollisionType::AABB:
			case CollisionType::OBB:
				m_pCube->Bind();
				break;
			case CollisionType::BC:
				m_pSpheare->Bind();
				break;

			default:
				break;
			}
		}

	}
#endif // _DEBUG
}

void Collision::AddCollider(const std::weak_ptr<Component>& pComponent)
{
	std::weak_ptr<Collider> pRenderer = std::dynamic_pointer_cast<Collider>(pComponent.lock());
	m_pColliderList.emplace_back(pRenderer);
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

void Collision::DifferentListJudge(ColliderPtrList & pListA, ColliderPtrList & pListB)
{
	if (pListA.empty())return;
	if (pListB.empty())return;
	for (auto itrA = pListA.begin(); itrA != pListA.end();)
	{
		for (auto itrB = pListB.begin(); itrB != pListB.end();)
		{
			CollisionType::Kind typeA = itrA->lock()->GetCollisionType(itrB->lock()->m_pOwner.lock()->GetType());
			CollisionType::Kind typeB = itrB->lock()->GetCollisionType(itrA->lock()->m_pOwner.lock()->GetType());

			switch (typeA)
			{
			case CollisionType::AABB:
				if (typeB == CollisionType::AABB)
				{
					AABB(itrA, itrB);
				}
				break;
			case CollisionType::OBB:
				if (typeB == CollisionType::OBB)
				{
					OBB(itrA, itrB);
				}
				break;
			case CollisionType::BC:
				if (typeB == CollisionType::BC)
				{
					BC(itrA, itrB);
				}
				break;
			case CollisionType::NONE:
			case CollisionType::MAX:
			default:
				break;
			}
			++itrB;
			if (itrB == pListB.end())
			{
				break;
			}
		}
		++itrA;
		if (itrA == pListA.end())
		{
			break;
		}
	}

	SameListJudge(pListB);
}

void Collision::SameListJudge(ColliderPtrList & pList)
{
	if (pList.empty())return;

	for (auto itrA = pList.begin(); itrA != pList.end();)
	{
		for (auto itrB = itrA; itrB != pList.end();)
		{
			++itrB;
			if (itrB == pList.end())
			{
				break;
			}
			CollisionType::Kind typeA = itrA->lock()->GetCollisionType(itrB->lock()->m_pOwner.lock()->GetType());
			CollisionType::Kind typeB = itrB->lock()->GetCollisionType(itrA->lock()->m_pOwner.lock()->GetType());

			switch (typeA)
			{
			case CollisionType::AABB:
				if (typeB == CollisionType::AABB)
				{
					AABB(itrA, itrB);
				}
				break;
			case CollisionType::OBB:
				if (typeB == CollisionType::OBB)
				{
					OBB(itrA, itrB);
				}
				break;
			case CollisionType::BC:
				if (typeB == CollisionType::BC)
				{
					BC(itrA, itrB);
				}
				break;
			case CollisionType::NONE:
			case CollisionType::MAX:
			default:
				break;
			}
		}
		++itrA;
		if (itrA == pList.end())
		{
			break;
		}
	}
}

void Collision::AABB(const ColliderPtrList::iterator& itrA, const ColliderPtrList::iterator& itrB)
{
	std::weak_ptr<Transform> pTransA = itrA->lock()->GetTransform();
	std::weak_ptr<Transform> pTransB = itrB->lock()->GetTransform();
	if ((pTransA.expired()) || (pTransB.expired()))return;
	DirectX::XMFLOAT4X4 wA, wB;
	DirectX::XMStoreFloat4x4(&wA, pTransA.lock()->GetWorldMatrix(itrA->lock()->GetScaleDeviation(), itrA->lock()->GetAngleDeviation(), itrA->lock()->GetPosDeviation()));
	DirectX::XMStoreFloat4x4(&wB, pTransB.lock()->GetWorldMatrix(itrB->lock()->GetScaleDeviation(), itrB->lock()->GetAngleDeviation(), itrB->lock()->GetPosDeviation()));
	Vector3 vPosA, vPosB, vScaleA, vScaleB;
	vPosA = { wA._41,wA._42,wA._43 };
	vPosB = { wB._41,wB._42,wB._43 };
	vScaleA = { wA._11,wA._22,wA._33 };
	vScaleB = { wB._11,wB._22,wB._33 };

	Vector3 vDistance = vPosB - vPosA;
	vDistance.Abs();

	if (vDistance.x < (vScaleA.x + vScaleB.x) * 0.5f
		&& vDistance.y < (vScaleA.y + vScaleB.y) * 0.5f
		&& vDistance.z < (vScaleA.z + vScaleB.z) * 0.5f)
	{
		itrA->lock()->EnableHitType(CollisionType::AABB);
		itrA->lock()->SetHitObject(CollisionType::AABB, itrB->lock()->m_pOwner);
		itrB->lock()->EnableHitType(CollisionType::AABB);
		itrB->lock()->SetHitObject(CollisionType::AABB, itrA->lock()->m_pOwner);
	}
}

void Collision::OBB(const ColliderPtrList::iterator& itrA, const ColliderPtrList::iterator& itrB)
{
	std::weak_ptr<Transform> pTransA = itrA->lock()->GetTransform();
	std::weak_ptr<Transform> pTransB = itrB->lock()->GetTransform();
	if ((pTransA.expired()) || (pTransB.expired()))return;
	DirectX::XMFLOAT4X4 wA, wB;
	DirectX::XMStoreFloat4x4(&wA, pTransA.lock()->GetWorldMatrix(itrA->lock()->GetScaleDeviation(), itrA->lock()->GetAngleDeviation(), itrA->lock()->GetPosDeviation()));
	DirectX::XMStoreFloat4x4(&wB, pTransB.lock()->GetWorldMatrix(itrB->lock()->GetScaleDeviation(), itrB->lock()->GetAngleDeviation(), itrB->lock()->GetPosDeviation()));
	DirectX::XMFLOAT3 posA, posB;
	posA = DirectX::XMFLOAT3(wA._41, wA._42, wA._43);
	posB = DirectX::XMFLOAT3(wB._41, wB._42, wB._43);
	
	DirectX::XMVECTOR vD;
	vD = DirectX::XMVectorSet(posB.x - posA.x, posB.y - posA.y, posB.z - posA.z, 0.0f);
	
	DirectX::XMVECTOR vN[6];
	vN[0] = DirectX::XMVectorSet(wA._11, wA._12, wA._13, 0.0f);
	vN[1] = DirectX::XMVectorSet(wA._21, wA._22, wA._23, 0.0f);
	vN[2] = DirectX::XMVectorSet(wA._31, wA._32, wA._33, 0.0f);
	vN[3] = DirectX::XMVectorSet(wB._11, wB._12, wB._13, 0.0f);
	vN[4] = DirectX::XMVectorSet(wB._21, wB._22, wB._23, 0.0f);
	vN[5] = DirectX::XMVectorSet(wB._31, wB._32, wB._33, 0.0f);

	DirectX::XMVECTOR vL[6];
	vL[0] = DirectX::XMVectorSet(wA._11 * 0.5f, wA._12 * 0.5f, wA._13 * 0.5f, 0.0f);
	vL[1] = DirectX::XMVectorSet(wA._21 * 0.5f, wA._22 * 0.5f, wA._23 * 0.5f, 0.0f);
	vL[2] = DirectX::XMVectorSet(wA._31 * 0.5f, wA._32 * 0.5f, wA._33 * 0.5f, 0.0f);
	vL[3] = DirectX::XMVectorSet(wB._11 * 0.5f, wB._12 * 0.5f, wB._13 * 0.5f, 0.0f);
	vL[4] = DirectX::XMVectorSet(wB._21 * 0.5f, wB._22 * 0.5f, wB._23 * 0.5f, 0.0f);
	vL[5] = DirectX::XMVectorSet(wB._31 * 0.5f, wB._32 * 0.5f, wB._33 * 0.5f, 0.0f);

	float fL, f, fD;
	for (int i = 0; i < 6; ++i) {
		DirectX::XMVECTOR& vS = vN[i];
		fL = 0.0f;
		for (int j = 0; j < 6; ++j) {
			DirectX::XMStoreFloat(&f, DirectX::XMVector3Dot(vS, vL[j]));
			fL += fabsf(f);
		}
		DirectX::XMStoreFloat(&fD, DirectX::XMVector3Dot(vS, vD));
		fD = fabsf(fD);
		if (fL < fD) return;// 当たっていない
	}

	DirectX::XMVECTOR vS;
	for (int i = 0; i < 3; ++i) {
		for (int j = 3; j < 6; ++j) {
			vS = DirectX::XMVector3Normalize(DirectX::XMVector3Cross(vN[i], vN[j]));
			fL = 0.0f;
			for (int k = 0; k < 6; ++k) {
				DirectX::XMStoreFloat(&f, DirectX::XMVector3Dot(vS, vL[k]));
				fL += fabsf(f);
			}
			DirectX::XMStoreFloat(&fD, DirectX::XMVector3Dot(vS, vD));
			fD = fabsf(fD);
			if (fL < fD) return;// 当たっていない
		}
	}
	//DebugLog::GetInstance().FreeError("OBBの当たり判定が発生");
	itrA->lock()->EnableHitType(CollisionType::OBB);
	itrA->lock()->SetHitObject(CollisionType::OBB, itrB->lock()->m_pOwner);
	itrB->lock()->EnableHitType(CollisionType::OBB);
	itrB->lock()->SetHitObject(CollisionType::OBB, itrA->lock()->m_pOwner);
}
 
void Collision::BC(const ColliderPtrList::iterator& itrA, const ColliderPtrList::iterator& itrB)
{
	std::weak_ptr<Transform> pTransA = itrA->lock()->GetTransform();
	std::weak_ptr<Transform> pTransB = itrB->lock()->GetTransform();
	if ((pTransA.expired()) || (pTransB.expired()))return;
	DirectX::XMFLOAT4X4 wA, wB;
	DirectX::XMStoreFloat4x4(&wA, pTransA.lock()->GetWorldMatrix(itrA->lock()->GetScaleDeviation(), itrA->lock()->GetAngleDeviation(), itrA->lock()->GetPosDeviation()));
	DirectX::XMStoreFloat4x4(&wB, pTransB.lock()->GetWorldMatrix(itrB->lock()->GetScaleDeviation(), itrB->lock()->GetAngleDeviation(), itrB->lock()->GetPosDeviation()));
	Vector3 vPosA, vPosB;
	vPosA = { wA._41,wA._42,wA._43 };
	vPosB = { wB._41,wB._42,wB._43 };
	float radiusAB = wA._33 + wB._33;

	Vector3 vDistance = vPosB - vPosA;
	float fLength = vDistance.Length();

	if (fLength < radiusAB)
	{
		//DebugLog::GetInstance().FreeError("球の当たり判定が発生");
		itrA->lock()->EnableHitType(CollisionType::BC);
		itrA->lock()->SetHitObject(CollisionType::BC, itrB->lock()->m_pOwner);
		itrB->lock()->EnableHitType(CollisionType::BC);
		itrB->lock()->SetHitObject(CollisionType::BC, itrA->lock()->m_pOwner);
	}
}

void Collision::RayMesh(const ColliderPtrList::iterator& itrRay, const ColliderPtrList::iterator& itrMesh)
{
	//DebugLog::GetInstance().FreeError("Rayとメッシュの当たり判定が発生");
}

void Collision::MouseMesh(const ColliderPtrList::iterator& itrMesh)
{
	std::weak_ptr<Mesh> pMesh = itrMesh->lock()->GetMesh();
	std::weak_ptr<Transform> pTransform = itrMesh->lock()->GetTransform();
	if ((pMesh.expired()) || (pTransform.expired()))return;
	if (m_pMouse.expired())return;
	if (m_pMouse.lock()->GetCamera().expired())return;

	Vector3 vCameraN = m_pMouse.lock()->GetCamera().lock()->front.get();
	vCameraN.Normalize();
	Vector3 vMousePos = m_pMouse.lock()->GetWorldPos();
	vCameraN *= -1;
	vMousePos += (vCameraN * 1000.0f);
	vCameraN *= -1;

	Mesh::SurfaceList surfaceList = pMesh.lock()->GetSurfaceList();
	for (auto itr : surfaceList)
	{
		DirectX::XMMATRIX mtx0 = MyMath::ConvertMatrix(1, 0, itr.vPos0) * pTransform.lock()->GetWorldMatrix();
		DirectX::XMMATRIX mtx1 = MyMath::ConvertMatrix(1, 0, itr.vPos1) * pTransform.lock()->GetWorldMatrix();
		DirectX::XMMATRIX mtx2 = MyMath::ConvertMatrix(1, 0, itr.vPos2) * pTransform.lock()->GetWorldMatrix();
		DirectX::XMFLOAT4X4 w0, wA, wB;
		DirectX::XMStoreFloat4x4(&w0, mtx0);
		DirectX::XMStoreFloat4x4(&wA, mtx1);
		DirectX::XMStoreFloat4x4(&wB, mtx2);
		Vector3 V0(w0._41, w0._42, w0._43);
		Vector3 V1(wA._41, wA._42, wA._43);
		Vector3 V2(wB._41, wB._42, wB._43);
		Vector3 V0V1 = V1 - V0;
		Vector3 V1V2 = V2 - V1;
		Vector3 V2V0 = V0 - V2;

		Vector3 N;
		N.Cross(V0V1, V1V2);
		N.Normalize();

		float base = N.Dot(vCameraN);
		if (base == 0.0f)continue;

		Vector3 P0V0 = V0 - vMousePos;
		float dot = N.Dot(P0V0);
		float t = dot / base;
		if (t < 0.0f)continue;
		Vector3 X = vMousePos + (vCameraN * t);
		Vector3 vValue;

		Vector3 V0X(X - V0);
		dot = N.Dot(vValue.Cross(V0V1, V0X));
		if (dot < 0.0f)continue;

		Vector3 V1X(X - V1);
		dot = N.Dot(vValue.Cross(V1V2, V1X));
		if (dot < 0.0f)continue;

		Vector3 V2X(X - V2);
		dot = N.Dot(vValue.Cross(V2V0, V2X));
		if (dot < 0.0f)continue;
		//DebugLog::GetInstance().FreeError("マウスとMeshの当たり判定が発生");
		m_pMouse.lock()->SetHitType(itrMesh->lock()->m_pOwner.lock()->GetType());
	}
}

void Collision::Excute(std::vector<NODE*>& pParentList)
{
	for (int i = 0; i < CHIELD_NUM; ++i)
	{
		std::vector<NODE*> pList = pParentList;
		NODE* pNode = pList[pParentList.size() - 1];
		if (pNode->pChiled[i])
		{
			pList.emplace_back(pNode->pChiled[i]);
			for (int j = 0; j < pParentList.size(); ++j)
			{
				DifferentListJudge(pList[j]->pList, pNode->pChiled[i]->pList);
			}
			Excute(pList);
		}
	}
}

void Collision::CreateNode(int nCnt, NODE* pNode, const Vector2 & vMin, const Vector2 & vMax)
{
	++nCnt;
	Vector2 vCenter = vMin + vMax;
	vCenter *= 0.5f;
	for (int i = 0; i < CHIELD_NUM; ++i)
	{
		pNode->pChiled[i] = new NODE();
		pNode->pChiled[i]->pParent = pNode;
		if (nCnt >= SERCH_NUM)
		{
			pNode->pChiled[i]->m_type = Collision::NODE::LAST;
		}
	}
	pNode->pChiled[0]->min = {    vMin.x,   vMin.y };
	pNode->pChiled[0]->max = { vCenter.x,vCenter.y };
	pNode->pChiled[1]->min = { vCenter.x,   vMin.y };
	pNode->pChiled[1]->max = {    vMax.x,vCenter.y };
	pNode->pChiled[2]->min = {    vMin.x,vCenter.y };
	pNode->pChiled[2]->max = { vCenter.x,   vMax.y };
	pNode->pChiled[3]->min = { vCenter.x,vCenter.y };
	pNode->pChiled[3]->max = {    vMax.x,   vMax.y };
	if (nCnt < SERCH_NUM)
	{
		CreateNode(nCnt, pNode->pChiled[0], pNode->pChiled[0]->min, pNode->pChiled[0]->max);
		CreateNode(nCnt, pNode->pChiled[1], pNode->pChiled[1]->min, pNode->pChiled[1]->max);
		CreateNode(nCnt, pNode->pChiled[2], pNode->pChiled[2]->min, pNode->pChiled[2]->max);
		CreateNode(nCnt, pNode->pChiled[3], pNode->pChiled[3]->min, pNode->pChiled[3]->max);
	}
}

Collision::NODE * Collision::SecondSerchNode(const Vector2 & vPos)
{
	for (int i = 0; i < CHIELD_NUM; ++i)
	{
		if (vPos.x >= m_pNode[i]->min.x
			&& vPos.x < m_pNode[i]->max.x
			&& vPos.y >= m_pNode[i]->min.y
			&& vPos.y < m_pNode[i]->max.y)
		{
			return m_pNode[i];
		}
	}
	return nullptr;
}

Collision::NODE * Collision::FirstSerchNode(const std::weak_ptr<Collider>& pCollider, NODE * pNode, const Vector2 & vPos)
{
	if (pNode->m_type >= Collision::NODE::LAST)
	{
		return pNode;
	}
	else
	{
		if (vPos.x >= pNode->pChiled[0]->min.x
			&& vPos.x < pNode->pChiled[0]->max.x
			&& vPos.y >= pNode->pChiled[0]->min.y
			&& vPos.y < pNode->pChiled[0]->max.y)
		{
			return FirstSerchNode(pCollider, pNode->pChiled[0], vPos);
		}

		if (vPos.x >= pNode->pChiled[1]->min.x
			&& vPos.x < pNode->pChiled[1]->max.x
			&& vPos.y >= pNode->pChiled[1]->min.y
			&& vPos.y < pNode->pChiled[1]->max.y)
		{
			return FirstSerchNode(pCollider, pNode->pChiled[1], vPos);
		}

		if (vPos.x >= pNode->pChiled[2]->min.x
			&& vPos.x < pNode->pChiled[2]->max.x
			&& vPos.y >= pNode->pChiled[2]->min.y
			&& vPos.y < pNode->pChiled[2]->max.y)
		{
			return FirstSerchNode(pCollider, pNode->pChiled[2], vPos);
		}

		if (vPos.x >= pNode->pChiled[3]->min.x
			&& vPos.x < pNode->pChiled[3]->max.x
			&& vPos.y >= pNode->pChiled[3]->min.y
			&& vPos.y < pNode->pChiled[3]->max.y)
		{
			return FirstSerchNode(pCollider, pNode->pChiled[3], vPos);
		}
	}
	return nullptr;
}


const bool Collision::ReSerchNode(const NODE* pNode, const Vector2& vPos)
{
	if (vPos.x >= pNode->min.x
		&& vPos.x < pNode->max.x
		&& vPos.y >= pNode->min.y
		&& vPos.y < pNode->max.y)
	{
		return true;
	}
	return false;
}

void Collision::FinalizeNode(int nCnt, NODE * pNode)
{
	++nCnt;
	if (nCnt >= SERCH_NUM)
	{
		delete pNode;
		pNode = nullptr;
	}
	else
	{
		for (int i = 0; i < CHIELD_NUM; ++i)
		{
			FinalizeNode(nCnt, pNode->pChiled[i]);
		}
		delete pNode;
		pNode = nullptr;
	}
}

void Collision::ClearNodeColliderList(int nCnt, NODE * pNode)
{
	++nCnt;
	if (nCnt >= SERCH_NUM)
	{
		pNode->pList.clear();
	}
	else
	{
		for (int i = 0; i < CHIELD_NUM; ++i)
		{
			ClearNodeColliderList(nCnt, pNode->pChiled[i]);
		}
		pNode->pList.clear();
	}
}

void Collision::SetQuadTree()
{
	for (int i = 0; i < m_pColliderList.size(); ++i)
	{
		if (m_pColliderList[i].expired())continue;
		if (m_pColliderList[i].lock()->m_pOwner.expired())continue;
		if (!m_pColliderList[i].lock()->m_pOwner.lock()->IsActive())continue;
		std::weak_ptr<Transform> pTrans = m_pColliderList[i].lock()->GetTransform();
		if (pTrans.expired())continue;
		DirectX::XMFLOAT4X4 w;
		DirectX::XMStoreFloat4x4(&w, pTrans.lock()->GetWorldMatrix(m_pColliderList[i].lock()->GetScaleDeviation(), m_pColliderList[i].lock()->GetAngleDeviation(), m_pColliderList[i].lock()->GetPosDeviation()));
		Vector2 vPos[CHIELD_NUM] =
		{
			{ w._41 - w._11,w._43 + w._33 },
			{ w._41 + w._11,w._43 + w._33 },
			{ w._41 - w._11,w._43 - w._33 },
			{ w._41 + w._11,w._43 - w._33 },
		};
		NODE* pNode;

		if (vPos[0].x >= m_pNode[0]->min.x
			&& vPos[0].x < m_pNode[0]->max.x
			&& vPos[0].y >= m_pNode[0]->min.y
			&& vPos[0].y < m_pNode[0]->max.y)
		{
			pNode = FirstSerchNode(m_pColliderList[i], m_pNode[0], vPos[0]);
		}
		else if (vPos[0].x >= m_pNode[1]->min.x
			&& vPos[0].x < m_pNode[1]->max.x
			&& vPos[0].y >= m_pNode[1]->min.y
			&& vPos[0].y < m_pNode[1]->max.y)
		{
			pNode = FirstSerchNode(m_pColliderList[i], m_pNode[1], vPos[0]);
		}
		else if (vPos[0].x >= m_pNode[2]->min.x
			&& vPos[0].x < m_pNode[2]->max.x
			&& vPos[0].y >= m_pNode[2]->min.y
			&& vPos[0].y < m_pNode[2]->max.y)
		{
			pNode = FirstSerchNode(m_pColliderList[i], m_pNode[2], vPos[0]);
		}
		else if (vPos[0].x >= m_pNode[3]->min.x
			&& vPos[0].x < m_pNode[3]->max.x
			&& vPos[0].y >= m_pNode[3]->min.y
			&& vPos[0].y < m_pNode[3]->max.y)
		{
			pNode = FirstSerchNode(m_pColliderList[i], m_pNode[3], vPos[0]);
		}

		bool isNextSerch = false;
		if (pNode)
		{
			for (int j = 1; j < CHIELD_NUM;)
			{
				if (ReSerchNode(pNode, vPos[j]))
				{
					++j;
				}
				else
				{
					if (pNode->pParent)
					{
						pNode = pNode->pParent;
					}
					else
					{
						isNextSerch = true;
						break;
					}
				}
			}
		}
		if (!isNextSerch)
		{
			pNode->pList.emplace_back(m_pColliderList[i]);
		}
		else
		{
			std::vector<NODE*> pNodeList;
			for (int j = 0; j < CHIELD_NUM; ++j)
			{
				NODE* pSecNode = SecondSerchNode(vPos[j]);
				bool isSkip = false;
				for (const auto& itr : pNodeList)
				{
					if (pSecNode == itr)
					{
						isSkip = true;
						continue;
					}
				}
				if (isSkip)continue;
				pNodeList.emplace_back(pSecNode);
			}
			for (const auto& itr : pNodeList)
			{
				itr->pList.emplace_back(m_pColliderList[i]);
			}
		}
	}
}