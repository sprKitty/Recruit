#include "MagicBall.h"
#include <App/Component/Transform.h>
#include <App/Component/Object.h>
#include <App/Component/Collider.h>
#include <App/Component/Magic/MagicBullet.h>
#include <App/FactoryMethod.h>
#include <System/Clocker.h>
#include <System/DebugLog.h>


float MagicBall::MOVE_SPEED = 1.0f;

void MagicBall::Init()
{
	m_pTransform = m_pOwner.lock()->GetComponent<Transform>();
	m_shot.fDefault = m_shot.fFluctuation = m_shot.fMaxTime = m_shot.fTime = 0.0f;
	m_shot.nNum = m_shot.nMaxNum = 0;
	m_move.fTime = 0.0f;
	int size = 12;
	for (int i = 0; i < size; ++i)
	{
		std::weak_ptr<Object> pObj = FactoryMethod::GetInstance().CreateBossWitchMagicBullet();
		if (pObj.expired())continue;
		m_pMagicBulletList.emplace_back(pObj.lock()->GetComponent<MagicBullet>());
	}

	std::weak_ptr<MagicBall> pMB = std::dynamic_pointer_cast<MagicBall>(weak_from_this().lock());
	if (!pMB.expired())
	{
		m_pState.reset(new State<MagicBall>());
		m_pState->AddActionFunc(pMB, &MagicBall::MoveShotPos);
		m_pState->AddActionFunc(pMB, &MagicBall::PlayShot);
	}

}

void MagicBall::Reset()
{
}

void MagicBall::Uninit()
{
	for (const auto& itr : m_pMagicBulletList)
	{
		if (itr.expired())continue;
		if (itr.lock()->m_pOwner.expired())continue;
		itr.lock()->m_pOwner.lock()->Uninit();
	}
}

void MagicBall::Update()
{
	if (m_pState->Action())
	{
		m_move.fTime = 0.0f;
		m_shot.fTime = 0.0f;
		m_shot.nNum = 0;
		m_pOwner.lock()->DisableActive();
		return;
	}
}

const bool MagicBall::MoveShotPos()
{
	if (m_move.fTime >= MOVE_SPEED)return true;

	m_move.fTime += Clocker::GetInstance().DeltaTime();
	float fLerpCnt = m_move.fTime / MOVE_SPEED;
	if (fLerpCnt >= 1.0f)fLerpCnt = 1.0f;
	Vector3 vPos;
	vPos.Lerp(m_move.vStart, m_move.vEnd, MyMath::EaseInOutCubic(fLerpCnt));
	vPos += m_move.vStart;
	vPos.y = 0.4f;
	m_pTransform.lock()->localpos = vPos;
	return false;
}

const bool MagicBall::PlayShot()
{
	if (m_shot.nNum >= m_shot.nMaxNum)return true;

	m_shot.fTime += Clocker::GetInstance().DeltaTime();

	if (m_shot.fTime >= m_shot.fMaxTime)
	{
		float fCenter = static_cast<float>(rand() % 360);
		fCenter -= 180.0f;
		fCenter = DirectX::XMConvertToRadians(fCenter);
		const int nAttackNum = 3;
		const float vAttackRad[nAttackNum] =
		{
			DirectX::XMConvertToRadians(15.0f),
			DirectX::XMConvertToRadians(0.0f),
			DirectX::XMConvertToRadians(-15.0f),
		};
		int num = 0;
		Vector3 vAttackDir;
		for (const auto& itr : m_pMagicBulletList)
		{
			if (num >= nAttackNum)break;
			if (itr.expired())continue;
			if (!itr.lock()->m_pOwner.lock()->IsActive())
			{
				vAttackDir = Vector3(cosf(fCenter + vAttackRad[num]), 0.0f, sinf(fCenter + vAttackRad[num]));
				vAttackDir.Normalize();
				itr.lock()->Reset();
				itr.lock()->m_pOwner.lock()->EnableActive();
				itr.lock()->SetStartPos(m_pTransform.lock()->localpos);
				itr.lock()->SetDiretion(vAttackDir);
				++num;
			}
		}

		m_shot.fTime -= m_shot.fMaxTime;
		++m_shot.nNum;
		NextShot();
	}
	return false;
}

void MagicBall::SetShotData(const float fTime, const float fFluctuation)
{
	m_shot.fDefault = fTime;
	m_shot.fFluctuation = fFluctuation;
	NextShot();
}

void MagicBall::SetMoveShotPos(const Vector3& nowPos, const Vector3& shotPos)
{
	m_move.vStart = nowPos;
	m_move.vEnd = shotPos;
}

void MagicBall::NextShot()
{
	int time = static_cast<int>(60.0f * m_shot.fFluctuation * 2.0f);
	time = rand() % time;
	m_shot.fMaxTime = time / 60.0f;
	m_shot.fMaxTime -= (m_shot.fTime * 0.5f);
	m_shot.fMaxTime += m_shot.fDefault;
}
