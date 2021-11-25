#include "Collider.h"
#include "Object.h"
#include <App/Component/Mesh.h>
#include <App/Collision.h>
#include <Shader/ShaderBuffer.h>
#include <App/Component/Transform.h>
#include <System/DebugLog.h>


void Collider::Init()
{
	m_pTransform = m_pOwner.lock()->GetComponent<Transform>();
	Collision::GetInstance().AddCollider(weak_from_this());
	m_typeList.resize(Collision_Type::MAX);
	m_HitInfoList.resize(Collision_Type::MAX);
	m_vPosDeviation = 0;
	m_vAngleDeviation = 0;
	m_vScaleDeviation = 1;
	for (auto itr : m_typeList)
	{
		itr = false;
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
			switch (i)
			{
			case Collision_Type::AABB:
				DebugLog::GetInstance().FreeError("AABB‚Ì“–‚½‚è”»’è‚ª”­¶");
				break;
			case Collision_Type::OBB:
				DebugLog::GetInstance().FreeError("OBB‚Ì“–‚½‚è”»’è‚ª”­¶");
				break;
			case Collision_Type::BC:
				DebugLog::GetInstance().FreeError("‹…‚Ì“–‚½‚è”»’è‚ª”­¶");
				break;
			case Collision_Type::RAY:
				DebugLog::GetInstance().FreeError("Ray‚Ì“–‚½‚è”»’è‚ª”­¶");
				break;
			case Collision_Type::MESH:
				DebugLog::GetInstance().FreeError("Mesh‚Ì“–‚½‚è”»’è‚ª”­¶");
				break;
			case Collision_Type::MOUSE:
				DebugLog::GetInstance().FreeError("ƒ}ƒEƒX‚Æ‚Ì“–‚½‚è”»’è‚ª”­¶");
				break;

			default:
				break;
			}
			m_HitInfoList[i].isFlg = false;
			m_HitInfoList[i].pObj.reset();
		}
	}
}