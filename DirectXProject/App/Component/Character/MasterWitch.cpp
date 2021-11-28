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
	std::weak_ptr<Object> pObj;
	m_pAttackList.resize(15);
	for (int i = 0; i < 15; i++)
	{
		pObj = FactoryMethod::GetInstance().CreateBossWitchMagic();
		if (pObj.expired())continue;
		m_pAttackList[i] = pObj.lock()->GetComponent<MagicBullet>();
		if (m_pAttackList[i].expired())continue;
		m_pAttackList[i].lock()->SetType(MagicType::FIRE);
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
		m_pBossStateList[Witch_State::Boss::ATTACK1]->AddActionFunc(pMW, &MasterWitch::CalcTarget);
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
		float fRad = MyMath::Radian(m_pTransform.lock()->localpos.x, m_pTransform.lock()->localpos.z, pTargetTrans.lock()->localpos.x, pTargetTrans.lock()->localpos.z);
		Vector3 vTargetDir = pTargetTrans.lock()->localpos - m_pTransform.lock()->localpos;
		vTargetDir.y = 0.0f;
		const int nAttackNum = 5;
		const float vAttackDeg[nAttackNum] =
		{
			RAD(20.0), RAD(10.0f), 0.0f, RAD(-10.0f), RAD(-20.0f)
		};
		int num = 0;
		Vector3 vAttackDir;
		for (auto itr : m_pAttackList)
		{
			if (num >= nAttackNum)break;
			if (itr.expired())continue;
			if (!itr.lock()->m_pOwner.lock()->IsActive())
			{
				vAttackDir = Vector3(cosf(fRad + vAttackDeg[num]), 0.0f, sinf(fRad + vAttackDeg[num]));
				vAttackDir.Normalize();
				itr.lock()->Reset();
				itr.lock()->m_pOwner.lock()->EnableActive();
				itr.lock()->SetStartPos(m_pTransform.lock()->localpos);
				itr.lock()->SetDiretion(vAttackDir);
				++num;
			}
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