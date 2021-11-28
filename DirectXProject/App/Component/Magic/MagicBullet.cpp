#include "MagicBullet.h"
#include <App/Component/Transform.h>
#include <App/Component/Object.h>
#include<App/Component/Collider.h>
#include <System/Clocker.h>


float MagicBullet::MOVE_SPEED = 10.0f;
float MagicBullet::MAX_SURVIVETIME = 2.0f;

void MagicBullet::Init()
{
	m_pTransform = m_pOwner.lock()->GetComponent<Transform>();
	m_fSurviveTime = 0;
	m_vMoveDirection = 0;
}

void MagicBullet::Reset()
{
	m_fSurviveTime = 0;
	m_vMoveDirection = 0;
}

void MagicBullet::Uninit()
{
}

void MagicBullet::Update()
{
	if (m_pCollider.expired())
	{
		m_pCollider = m_pOwner.lock()->GetComponent<Collider>();
	}
	else
	{
		CollideUpdate();
	}

	m_fSurviveTime += static_cast<float>(Clocker::GetInstance().GetFrameTime());
	if (m_fSurviveTime >= MAX_SURVIVETIME)
	{
		m_pOwner.lock()->DisableActive();
	}

	if (!m_pTransform.expired())
	{
		m_pTransform.lock()->localpos += m_vMoveDirection * MOVE_SPEED * static_cast<float>(Clocker::GetInstance().GetFrameTime());
	}
}

void MagicBullet::SetStartPos(const Vector3 & vPos)
{
	if (m_pTransform.expired())return;
	m_pTransform.lock()->localpos = vPos;
}

void MagicBullet::CollideUpdate()
{
	Collider::HitInfo hitInfo = m_pCollider.lock()->IsHitInfo(Collision_Type::BC);
	if (hitInfo.isFlg)
	{
		if (!hitInfo.pObj.expired())
		{
			switch (m_pOwner.lock()->GetType())
			{
			case ObjectType::PLAYERATTACK:
				if (hitInfo.pObj.lock()->GetType() == ObjectType::BOSSWITCH)
				{
					m_pOwner.lock()->DisableActive();
				}
				break;

			case ObjectType::BOSSATTACK:
				if (hitInfo.pObj.lock()->GetType() == ObjectType::PLAYER)
				{
					m_pOwner.lock()->DisableActive();
				}
				break;

			default:
				break;
			}

		}
	}
}
