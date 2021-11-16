#include "Collider.h"
#include "Object.h"
#include <System/Geometory.h>
#include <Shader/ShaderBuffer.h>
#include <System/Collision.h>

Collider::Collider()
	:m_isActive(true)
	, m_isHit(false)
	, m_isOutSideWorld(false)
	, m_isParentWorld(true)
{
	m_Matrix = XMMatrixIdentity();
	XMStoreFloat4x4(&m_World, m_Matrix);
}


Collider::~Collider()
{
}


void Collider::Init()
{
	COLLISION->m_pColliderList.push_back(this);
	m_World = ConvertWorld();
}

void Collider::Uninit()
{
	COLLISION->Delete(this);
}


void Collider::Update()
{
	m_World = ConvertWorld();
}


void Collider::Draw()
{
	if (m_isHit)
		return;
	XMFLOAT4X4 world = XMFLOAT4X4A(
		m_World._11, m_World._21, m_World._31, m_World._41,
		m_World._12, m_World._22, m_World._32, m_World._42,
		m_World._13, m_World._23, m_World._33, m_World._43,
		m_World._14, m_World._24, m_World._34, m_World._44
	);
	SHADER->SetWorld4X4(world);
	SHADER->SetColor(XMFLOAT4(1, 0, 0, 0.6f));
	DrawCube();
}


XMFLOAT4X4 Collider::ConvertWorld()
{
	XMFLOAT4X4 world;
	XMMATRIX mtx = MyMath::ConvertMatrix(m_EditCollider.scale, m_EditCollider.rot, m_EditCollider.pos);
	if (!m_isOutSideWorld)
	{
		XMMATRIX mtxT = m_pOwner->GetWorldMatrix();
		mtx *= mtxT;
		if (m_pOwner->GetParent())
		{
			if (m_isParentWorld)
			{
				XMMATRIX mtxP = m_pOwner->GetParent()->GetWorld();
				mtx *= mtxP;
			}
		}
	}
	else
	{
		mtx *= m_Matrix;
	}
	m_wMatrix = mtx;
	XMStoreFloat4x4(&world, mtx);
	return world;
}