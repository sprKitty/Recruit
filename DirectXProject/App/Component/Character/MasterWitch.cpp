#include "MasterWitch.h"
#include <App/Component/Transform.h>
#include <App/Component/Object.h>
#include <App/Component/Renderer/BillBoardRenderer.h>
#include <App/Component/Event/Event.h>
#include <App/Component/Magic/MagicBullet.h>
#include <App/FactoryMethod.h>
#include <System/Clocker.h>
#include <MyMath.h>


float MasterWitch::WAIT_TIME = 1.0f;

const char* g_pWitchAnimPath[] =
{
	"Assets/csv/witchwait.csv",
	"Assets/csv/witchattack.csv",
};

void MasterWitch::Init()
{
	m_pTransform = m_pOwner.lock()->GetComponent<Transform>();
	m_pTransform.lock()->localscale = 2;
	m_Direction = Chara_Direction::DOWN;
	m_masterState = Witch_State::Master::WAIT;
	m_bossState = Witch_State::Boss::WAIT;
	m_state = Witch_State::MASTER;
	for (int i = 0; i < Witch_State::MAX; ++i)
	{
		std::shared_ptr<TexAnimation> pImage(new TexAnimation());
		pImage->LoadData(g_pWitchAnimPath[i]);
		m_pTexAnimList.push_back(pImage);
	}
	std::weak_ptr<MasterWitch> pMW = std::dynamic_pointer_cast<MasterWitch>(weak_from_this().lock());
	if (!pMW.expired())
	{
		for (int i = 0; i < Witch_State::Master::MAX; ++i)
		{
			m_pMasterStateList.push_back(std::move(std::make_unique<State<MasterWitch> >()));
		}
		for (int i = 0; i < Witch_State::Boss::MAX; ++i)
		{
			m_pBossStateList.push_back(std::move(std::make_unique<State<MasterWitch> >()));
		}
		
		m_pMasterStateList[Witch_State::Master::WAIT]->AddActionFunc(pMW, &MasterWitch::CalcTarget);
		m_pMasterStateList[Witch_State::Master::WAIT]->SetTransitionFunc(pMW, &MasterWitch::MasterFromBoss);

		m_pBossStateList[Witch_State::Boss::WAIT]->AddActionFunc(pMW, &MasterWitch::CalcTarget);
		m_pBossStateList[Witch_State::Boss::WAIT]->AddActionFunc(pMW, &MasterWitch::Wait);
		m_pBossStateList[Witch_State::Boss::WAIT]->SetTransitionFunc(pMW, &MasterWitch::ChangeBossState);

		m_pBossStateList[Witch_State::Boss::ATTACK1]->AddActionFunc(pMW, &MasterWitch::Attack1);
		m_pBossStateList[Witch_State::Boss::ATTACK1]->SetTransitionFunc(pMW, &MasterWitch::ChangeBossState);
	}
	else
	{
		DebugLog::GetInstance().FreeError("魔女のステートが生成できていません。");
	}
	m_fWaitTime = 0.0f;
}

void MasterWitch::Uninit()
{
	m_pMasterStateList.clear();
	m_pBossStateList.clear();
	m_pTexAnimList.clear();
}

void MasterWitch::Update()
{
	switch (m_state)
	{
	case Witch_State::MASTER:
		if (m_pMasterStateList[m_masterState]->Action())
		{
			m_masterState = static_cast<Witch_State::Master::Kind>(m_pMasterStateList[m_masterState]->Next());
			
			if (m_masterState == Witch_State::Master::MAX)
			{
				m_state = Witch_State::BOSS;
			}
		}

		m_pTexAnimList[m_masterState]->Update(m_Direction);
		if (!m_pBBR.expired())
		{
			m_pBBR.lock()->SetMainImage(m_pTexAnimList[m_masterState]);
		}
		break;
	
	case Witch_State::BOSS:
		if (m_pBossStateList[m_bossState]->Action())
		{
			m_bossState = static_cast<Witch_State::Boss::Kind>(m_pBossStateList[m_bossState]->Next());
		
			if (m_bossState == Witch_State::Boss::MAX)
			{
				m_state = Witch_State::MASTER;
			}
		}

		m_pTexAnimList[m_bossState]->Update(m_Direction);
		if (!m_pBBR.expired())
		{
			m_pBBR.lock()->SetMainImage(m_pTexAnimList[m_bossState]);
		}
		break;
	
	case Witch_State::MAX:
	default:
		break;
	}
}

const bool MasterWitch::CalcTarget()
{
	if (m_pTarget.expired())
	{
		DebugLog::GetInstance().FreeError("MasterWitchクラスでターゲットが設定されていません。");
		return true;
	}
	std::weak_ptr<Transform> pTargetT = m_pTarget.lock()->GetComponent<Transform>();
	Vector3 vTargetPos = pTargetT.lock()->localpos;
	Vector3 vPos = m_pTransform.lock()->localpos;

	switch (m_state)
	{
	case Witch_State::MASTER:
	{
		Vector2 vDistance = Vector2(vPos.x - vTargetPos.x, vPos.z - vTargetPos.z);
		float fLength = vDistance.Length();

		if (fLength > 5)
		{
			m_Direction = Chara_Direction::DOWN;
		}
		else
		{
			float fRad = MyMath::Radian(vPos.x, vPos.z, vTargetPos.x, vTargetPos.z);
			m_Direction = CalcDirection8(DEG(fRad));
		}
		return true;
	}

	case Witch_State::BOSS:
	{
		float fRad = MyMath::Radian(vPos.x, vPos.z, vTargetPos.x, vTargetPos.z);
		m_Direction = CalcDirection4(DEG(fRad));
		return true;
	}

	case Witch_State::MAX:
	default:
		return false;
	}


	return true;
}

const bool MasterWitch::Wait()
{
	if (m_fWaitTime >= WAIT_TIME)
	{
		m_fWaitTime = 0;
		return true;
	}
	m_fWaitTime += Clocker::GetInstance().GetFrameTime();
	return false;
}

const bool MasterWitch::Attack1()
{
	if (m_pTexAnimList[m_bossState]->IsSheetUpdate(VectorInt2(0, 2)))
	{
		std::weak_ptr<Transform> pTargetTrans = m_pTarget.lock()->GetComponent<Transform>();
		Vector3 vTargetDir = pTargetTrans.lock()->localpos - m_pTransform.lock()->localpos;
		vTargetDir.y = 0.0f;
		const int nAttackNum = 3;
		const Vector3 vAttackDirections[nAttackNum] =
		{
			Vector3(0.9f, 1.0f, 1.1f), Vector3(1.0f), Vector3(1.1f, 1.0f, 0.9f),
		};
		Vector3 vAttackDir;
		std::weak_ptr<Object> pObj;
		std::weak_ptr<MagicBullet> pMB;
		for (int i = 0; i < nAttackNum; ++i)
		{
			vAttackDir = vTargetDir * vAttackDirections[i];
			vAttackDir.Normalize();
			pObj = FactoryMethod::GetInstance().CreateBossWitchMagic();
			if (pObj.expired())continue;
			pMB = pObj.lock()->GetComponent<MagicBullet>();
			if (pMB.expired())continue;
			pMB.lock()->SetStartPos(m_pTransform.lock()->localpos);
			pMB.lock()->SetType(MagicType::FIRE);
			pMB.lock()->SetDiretion(vAttackDir);
			pObj.lock()->Update();
		}
	}

	if (m_pTexAnimList[m_bossState]->IsFinish())
	{
		return true;
	}

	return false;
}

const int MasterWitch::MasterFromBoss()
{
	std::weak_ptr<Event> pEvent = m_pOwner.lock()->GetComponent<Event>();
	if (!pEvent.expired())
	{
		if (pEvent.lock()->IsFinishAll())
		{
			return Witch_State::BOSS;
		}
		else
		{
			return Witch_State::MASTER;
		}
	}
	else
	{
		DebugLog::GetInstance().FreeError("イベントが読み取れませんでした!");
		return Witch_State::MASTER;
	}

}

const int MasterWitch::ChangeBossState()
{
	return Witch_State::Boss::ATTACK1;
}

const int MasterWitch::ResetBossState()
{
	return Witch_State::Boss::WAIT;
}