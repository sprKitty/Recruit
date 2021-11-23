#include "MasterWitch.h"
#include <Transform.h>
#include <App/Component/Object.h>
#include <App/Component/Renderer/BillBoardRenderer.h>
#include <App/Component/Event/Event.h>
#include <System/Clocker.h>
#include <MyMath.h>

const char* g_pWitchAnimPath[] =
{
	"Assets/csv/witchwait.csv",
	"Assets/csv/witchwait.csv",
};

void MasterWitch::Init()
{
	m_pTransform = m_pOwner.lock()->GetComponent<Transform>();
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
		m_pBossStateList[Witch_State::Boss::WAIT]->SetTransitionFunc(pMW, &MasterWitch::ChangeBossState);

		m_pBossStateList[Witch_State::Boss::ATTACK1]->SetTransitionFunc(pMW, &MasterWitch::ChangeBossState);
	}
	else
	{
		DebugLog::GetInstance().FreeError("魔女のステートが生成できていません。");
	}
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
		m_Direction = CalcDirection8(DEG(fRad));
		return true;
	}

	case Witch_State::MAX:
	default:
		return false;
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
	return Witch_State::Boss::WAIT;
}

const int MasterWitch::ResetBossState()
{
	return Witch_State::Boss::WAIT;
}