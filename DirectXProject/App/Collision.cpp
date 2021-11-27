#include "Collision.h"
#include <System/Mouse.h>
#include <MyMath.h>
#include <App/Component/Mesh.h>
#include <App/Component/Transform.h>
#include <App/Camera.h>
#include <Shader/ShaderBuffer.h>

void Collision::Initialize()
{
#ifdef _DEBUG
	m_pCube.reset(new Mesh());
	m_pCube->Set("cube");
#endif // _DEBUG

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

			if (i == Collision_Type::MOUSE)
			{
				MouseMesh(itrX);
			}

			for (ColliderPtrList::iterator itrY = itrX; itrY != m_pColliderList.end();)
			{
				++itrY;
				if (itrY == m_pColliderList.end())
				{
					break;
				}
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
					case Collision_Type::BC:
						if (static_cast<Collision_Type::Kind>(j)
							== Collision_Type::BC)
						{
							BC(itrX, itrY);
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
			}
		}
		++itrX;
		if (itrX == m_pColliderList.end())
		{
			break;
		}
	}
}

void Collision::Draw()
{
#ifdef _DEBUG
	for (const auto& itr : m_pColliderList)
	{
		if (itr.expired())continue;
		std::weak_ptr<Transform> pTrans = itr.lock()->GetTransform();
		if (pTrans.expired())continue;

		DirectX::XMMATRIX mtx = pTrans.lock()->GetWorldMatrix() * itr.lock()->GetDeviationMatrix();
		DirectX::XMVECTOR s, q, p;
		DirectX::XMMatrixDecompose(&s, &q, &p, mtx);
		DirectX::XMFLOAT3 vScale, vPos;
		DirectX::XMStoreFloat3(&vScale, s);
		DirectX::XMStoreFloat3(&vPos, p);
		mtx = MyMath::ConvertMatrix(Vector3().Convert(vScale), 0, Vector3().Convert(vPos));
		ShaderBuffer::GetInstance().SetWorld(mtx);
		ShaderBuffer::GetInstance().BindPS(PS_TYPE::COLOR);
		for (int i = 0; i < Collision_Type::MAX; ++i)
		{
			if (itr.lock()->IsCollisionType(static_cast<Collision_Type::Kind>(i)))
			{
				switch (i)
				{
				case Collision_Type::AABB:
					break;
				case Collision_Type::OBB:
					break;
				case Collision_Type::BC:
					m_pCube->Bind();
					break;
				default:
					break;
				}
			}
		}

	}
#endif // _DEBUG
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

void Collision::BC(ColliderPtrList::iterator itrA, ColliderPtrList::iterator itrB)
{
	std::weak_ptr<Mesh> pMeshA = itrA->lock()->GetMesh();
	std::weak_ptr<Mesh> pMeshB = itrB->lock()->GetMesh();
	if ((pMeshA.expired()) && (pMeshB.expired()))return;
	std::weak_ptr<Transform> pTransA = itrA->lock()->GetTransform();
	std::weak_ptr<Transform> pTransB = itrB->lock()->GetTransform();
	if ((pTransA.expired()) && (pTransB.expired()))return;
	DirectX::XMMATRIX mtxA = pTransA.lock()->GetWorldMatrix() * itrA->lock()->GetDeviationMatrix();
	DirectX::XMMATRIX mtxB = pTransB.lock()->GetWorldMatrix() * itrB->lock()->GetDeviationMatrix();
	DirectX::XMVECTOR sA, qA, pA, sB, qB, pB;
	DirectX::XMMatrixDecompose(&sA, &qA, &pA, mtxA);
	DirectX::XMMatrixDecompose(&sB, &qB, &pB, mtxB);
	DirectX::XMFLOAT3 vScaleA, vPosA, vScaleB, vPosB;
	DirectX::XMStoreFloat3(&vScaleA, sA);
	DirectX::XMStoreFloat3(&vScaleB, sB);
	DirectX::XMStoreFloat3(&vPosA, pA);
	DirectX::XMStoreFloat3(&vPosB, pB);
	Vector3 vDistance = Vector3().Convert(vPosB) - Vector3().Convert(vPosA);
	float fLength = vDistance.Length();
	if (fLength < (vScaleA.z + vScaleB.z) * 0.5f)
	{
		itrA->lock()->EnableHitType(Collision_Type::BC);
		itrA->lock()->SetHitObject(Collision_Type::BC, itrB->lock()->m_pOwner);
		itrB->lock()->EnableHitType(Collision_Type::BC);
		itrB->lock()->SetHitObject(Collision_Type::BC, itrA->lock()->m_pOwner);
	}
}

void Collision::RayMesh(ColliderPtrList::iterator itrRay, ColliderPtrList::iterator itrMesh)
{
}

void Collision::MouseMesh(ColliderPtrList::iterator itrMesh)
{
	std::weak_ptr<Mesh> pMesh = itrMesh->lock()->GetMesh();
	std::weak_ptr<Transform> pTransform = itrMesh->lock()->GetTransform();
	if ((pMesh.expired()) && (pTransform.expired()))return;
	if (m_pMouse.expired())return;
	if (m_pMouse.lock()->GetCamera().expired())return;

	Vector3 vCameraN = m_pMouse.lock()->GetCamera().lock()->GetNormal();
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
		DirectX::XMFLOAT4X4 w0, w1, w2;
		DirectX::XMStoreFloat4x4(&w0, mtx0);
		DirectX::XMStoreFloat4x4(&w1, mtx1);
		DirectX::XMStoreFloat4x4(&w2, mtx2);
		Vector3 V0(w0._41, w0._42, w0._43);
		Vector3 V1(w1._41, w1._42, w1._43);
		Vector3 V2(w2._41, w2._42, w2._43);
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
		itrMesh->lock()->EnableHitType(Collision_Type::MOUSE);
		m_pMouse.lock()->SetHitType(itrMesh->lock()->m_pOwner.lock()->GetType());
	}
}
