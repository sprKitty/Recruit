#include "Collider.h"
#include <App/Component/Mesh.h>
#include <App/Collision.h>
#include <Shader/ShaderBuffer.h>
#include <App/Component/Transform.h>


void Collider::Init()
{
	m_pTransform = m_pOwner.lock()->GetComponent<Transform>();
	Collision::GetInstance().AddCollider(weak_from_this());
	m_typeList.resize(ObjectType::MAX);
	m_HitInfoList.resize(CollisionType::MAX);
	m_vPosDeviation = 0;
	m_vAngleDeviation = 0;
	m_vScaleDeviation = 1;
	m_isMouse = false;
	for (auto itr : m_typeList)
	{
		itr = CollisionType::NONE;
	}
	for (auto itr : m_HitInfoList)
	{
		itr.isFlg = false;
	}
}

void Collider::Uninit()
{
	Collision::GetInstance().ReleaseCollider(weak_from_this());
}

void Collider::Update()
{
	if (m_pMesh.expired())
	{
		m_pMesh = m_pOwner.lock()->GetComponent<Mesh>();
	}
	for (int i = 0; i < m_HitInfoList.size(); ++i)
	{
		if (m_HitInfoList[i].isFlg)
		{
			m_HitInfoList[i].isFlg = false;
			m_HitInfoList[i].pObj.reset();
		}
	}
}