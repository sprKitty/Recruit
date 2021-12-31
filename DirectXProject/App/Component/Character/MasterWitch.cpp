#include "MasterWitch.h"
#include <App/Component/Transform.h>
#include <App/Component/Renderer/BillBoardRenderer.h>
#include <App/Component/Event/Event.h>
#include <App/Component/Magic/MagicBall.h>
#include <App/Component/Magic/MagicBullet.h>
#include <App/Component/Magic/MagicRazer.h>
#include <App/Component/Collider.h>
#include <App/FactoryMethod.h>
#include <System/Clocker.h>
#include <MyMath.h>


float MasterWitch::WAIT_TIME = 1.5;
float MasterWitch::CHARGE_RAZER_TIME = 2.0f;
float MasterWitch::INIT_RAZER_LENGTH = 0.5f;
float MasterWitch::MAX_RAZER_LENGTH = 10.0f;

const char* g_pWitchAnimPath[Witch_State::Boss::MAX] =
{
	"Assets/csv/witchwait.csv",
	"Assets/csv/witchattack.csv",
	"Assets/csv/witchwait.csv",
	"Assets/csv/witchwait.csv",
	//"Assets/csv/witchwait.csv",
};

void MasterWitch::Init()
{
	m_pTransform = m_pOwner.lock()->GetComponent<Transform>();
	m_pTransform.lock()->localpos = { 0.0f,0.5f,1.0f };
	m_pTransform.lock()->localscale = 1;
	m_Direction = Chara_Direction::DOWN;
	m_masterState = Witch_State::Master::WAIT;
	m_bossState = Witch_State::Boss::WAIT;
	m_state = Witch_State::MASTER;
	m_fWaitTime = 0.0f;
	for (int i = 0; i < Witch_State::Boss::MAX; ++i)
	{
		std::shared_ptr<TexAnimation> pTexAnim(new TexAnimation());
		pTexAnim->LoadData(g_pWitchAnimPath[i]);
		m_pTexAnimList.push_back(pTexAnim);
	}

	std::weak_ptr<Object> pObj;
	m_pAttackList1.resize(15);
	for (int i = 0; i < 15; i++)
	{
		pObj = FactoryMethod::GetInstance().CreateBossWitchMagicBullet();
		if (pObj.expired())continue;
		m_pAttackList1[i] = pObj.lock()->GetComponent<MagicBullet>();
		if (m_pAttackList1[i].expired())continue;
		m_pAttackList1[i].lock()->SetType(MagicType::FIRE);
		m_pAttackObject.emplace_back(pObj);
	}

	m_rotatebullet.nRate = 10;
	m_rotatebullet.nIntervalNum = static_cast<int>(360.0f / m_rotatebullet.nRate);
	m_rotatebullet.nLineNum = 2;
	m_rotatebullet.fLineAngle = 360.0f / m_rotatebullet.nLineNum;
	m_pAttackList2.resize(m_rotatebullet.nLineNum * m_rotatebullet.nIntervalNum);
	for (int i = 0; i < m_pAttackList2.size(); i++)
	{
		pObj = FactoryMethod::GetInstance().CreateBossWitchMagicBullet();
		if (pObj.expired())continue;
		m_pAttackList2[i] = pObj.lock()->GetComponent<MagicBullet>();
		if (m_pAttackList2[i].expired())continue;
		m_pAttackList2[i].lock()->SetType(MagicType::FIRE);
		m_pAttackObject.emplace_back(pObj);
	}
	
	m_pAttackList3.resize(4);
	MagicBall::MOVE_SPEED = 1.2f;
	for (int i = 0; i < m_pAttackList3.size(); ++i)
	{
		pObj = FactoryMethod::GetInstance().CreateBossWitchMagicBall();
		if (pObj.expired())continue;
		m_pAttackList3[i] = pObj.lock()->GetComponent<MagicBall>();
		m_pAttackList3[i].lock()->SetShotNum(10);
		m_pAttackList3[i].lock()->SetShotData(0.6f, 0.1f);
		m_pAttackList3[i].lock()->SetType(MagicType::FIRE);
		m_pAttackObject.emplace_back(pObj);
	}

	m_razer.num = 8;
	m_razer.fIntervalAngle = 360.0f / m_razer.num;
	m_pRotateRazerList.resize(m_razer.num);
	for (int i = 0; i < m_razer.num; ++i)
	{
		pObj = FactoryMethod::GetInstance().CreateBossWitchRazer();
		if (pObj.expired())continue;
		m_pRotateRazerList[i] = pObj.lock()->GetComponent<MagicRazer>();
		if (m_pRotateRazerList[i].expired())continue;
		m_pRotateRazerList[i].lock()->SetType(MagicType::FIRE);
		m_pRotateRazerList[i].lock()->SetLength(10.0f);
		m_pAttackObject.emplace_back(pObj);
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

		m_pBossStateList[Witch_State::Boss::ATTACK1]->AddActionFunc(pMW, &MasterWitch::CalcTarget);
		m_pBossStateList[Witch_State::Boss::ATTACK1]->AddActionFunc(pMW, &MasterWitch::Attack1);
		m_pBossStateList[Witch_State::Boss::ATTACK1]->SetTransitionFunc(pMW, &MasterWitch::ChangeBossState);

		m_pBossStateList[Witch_State::Boss::ATTACK2]->AddActionFunc(pMW, &MasterWitch::CalcTarget);
		m_pBossStateList[Witch_State::Boss::ATTACK2]->AddActionFunc(pMW, &MasterWitch::Attack2);
		m_pBossStateList[Witch_State::Boss::ATTACK2]->SetTransitionFunc(pMW, &MasterWitch::ChangeBossState);

		m_pBossStateList[Witch_State::Boss::ATTACK3]->AddActionFunc(pMW, &MasterWitch::CalcTarget);
		m_pBossStateList[Witch_State::Boss::ATTACK3]->AddActionFunc(pMW, &MasterWitch::Attack3);
		m_pBossStateList[Witch_State::Boss::ATTACK3]->SetTransitionFunc(pMW, &MasterWitch::ChangeBossState);

		//m_pBossStateList[Witch_State::Boss::ROTATERAZER]->AddActionFunc(pMW, &MasterWitch::ChargeRotateRazer);
		//m_pBossStateList[Witch_State::Boss::ROTATERAZER]->AddActionFunc(pMW, &MasterWitch::IrradiationRazer);
		//m_pBossStateList[Witch_State::Boss::ROTATERAZER]->AddActionFunc(pMW, &MasterWitch::AttackRotateRazer);
		//m_pBossStateList[Witch_State::Boss::ROTATERAZER]->AddActionFunc(pMW, &MasterWitch::VanishRazer);
		//m_pBossStateList[Witch_State::Boss::ROTATERAZER]->SetTransitionFunc(pMW, &MasterWitch::ChangeBossState);
	}
	else
	{
		DebugLog::GetInstance().FreeError("魔女のステートが生成できていません。");
	}
}

void MasterWitch::Uninit()
{
	for (const auto& itr : m_pAttackObject)
	{
		if (itr.expired())continue;
		itr.lock()->DisableActive();
		itr.lock()->EnableDelete();
	}

	m_pMasterStateList.clear();
	m_pBossStateList.clear();
	m_pTexAnimList.clear();
}

void MasterWitch::Update()
{
	SetNeedComponent();

	if (HitUpdate())
	{
		m_pOwner.lock()->EnableDelete();
		return;
	}

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

const bool MasterWitch::HitUpdate()
{
	Collider::HitInfo hitInfoBC = m_pCollider.lock()->IsHitInfo(CollisionType::BC);

	if (hitInfoBC.isFlg)
	{
		if (!hitInfoBC.pObj.expired())
		{
			if (hitInfoBC.pObj.lock()->GetType() == ObjectType::PLAYERATTACK)
			{
				return true;
			}
		}
	}

	return false;
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
			m_Direction = CalcDirection8(DirectX::XMConvertToDegrees(fRad));
		}
		return true;
	}

	case Witch_State::BOSS:
	{
		float fRad = MyMath::Radian(vPos.x, vPos.z, vTargetPos.x, vTargetPos.z);
		m_Direction = CalcDirection4(DirectX::XMConvertToDegrees(fRad));
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
	if (m_fWaitTime >= WAIT_TIME)return true;

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
		const float vAttackRad[nAttackNum] =
		{
			DirectX::XMConvertToRadians(30.0f),
			DirectX::XMConvertToRadians(15.0f),
			DirectX::XMConvertToRadians(0.0f),
			DirectX::XMConvertToRadians(-15.0f),
			DirectX::XMConvertToRadians(-30.0f),
		};
		int num = 0;
		Vector3 vAttackDir;
		for (auto itr : m_pAttackList1)
		{
			if (num >= nAttackNum)break;
			if (itr.expired())continue;
			if (!itr.lock()->m_pOwner.lock()->IsActive())
			{
				vAttackDir = Vector3(cosf(fRad + vAttackRad[num]), 0.0f, sinf(fRad + vAttackRad[num]));
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

const bool MasterWitch::Attack2()
{
	if (m_rotatebullet.num >= m_rotatebullet.nIntervalNum)
	{
		return true;
	}

	m_rotatebullet.fMainLineNowAngle -= DirectX::XMConvertToDegrees(Clocker::GetInstance().GetFrameTime());
	float fDis = fabsf(m_rotatebullet.fMainLineNowAngle) - fabsf(m_rotatebullet.fMainLineOldAngle);
	float flg = fabsf(fDis) - m_rotatebullet.nRate;
	if (flg >= 0 
		|| m_rotatebullet.fMainLineNowAngle <= 0)
	{
		if (m_rotatebullet.fMainLineNowAngle <= 0)
		{
			m_rotatebullet.fMainLineNowAngle += (fDis + m_rotatebullet.nRate);
		}
		else
		{
			m_rotatebullet.fMainLineNowAngle -= (fDis + m_rotatebullet.nRate);
		}
		int i = 0;
		Vector3 vDirction;
		float rad;
		Vector3 vPos = m_pTransform.lock()->localpos;
		for (const auto& itr : m_pAttackList2)
		{
			if (i > m_rotatebullet.nLineNum)break;
			if (itr.expired())continue;
			if (itr.lock()->m_pOwner.lock()->IsActive())continue;
			rad = DirectX::XMConvertToRadians(m_rotatebullet.fMainLineNowAngle) + DirectX::XMConvertToRadians(m_rotatebullet.fLineAngle * i);
			vDirction = Vector3(cosf(rad), 0.0f, sinf(rad));
			vDirction.Normalize();
			itr.lock()->m_pOwner.lock()->EnableActive();
			itr.lock()->Reset();
			itr.lock()->SetStartPos(vPos);
			itr.lock()->SetDiretion(vDirction);
			++i;
		}
		m_rotatebullet.fMainLineOldAngle = m_rotatebullet.fMainLineNowAngle;
		++m_rotatebullet.num;
	}

	return false;
}

const bool MasterWitch::Attack3()
{
	for (const auto itr : m_pAttackList3)
	{
		if (itr.lock()->m_pOwner.lock()->IsActive())return true;
	}

	const Vector3 vPosList[] =
	{
		Vector3(-6.0f, 0.5f, 6.0f),
		Vector3(6.0f, 0.5f, 6.0f),
		Vector3(-6.0f, 0.5f, -6.0f),
		Vector3(6.0f, 0.5f, -6.0f),
	};
	int i = 0;
	for (const auto itr : m_pAttackList3)
	{
		itr.lock()->SetMoveShotPos(m_pTransform.lock()->localpos, vPosList[i]);
		itr.lock()->m_pOwner.lock()->EnableActive();
		++i;
	}

	return false;
}

const bool MasterWitch::IrradiationRazer()
{
	if (m_razer.fIrradiationTime >= 1.0f)return true;

	m_razer.fIrradiationTime += Clocker::GetInstance().GetFrameTime() / 0.3f;
	if (m_razer.fIrradiationTime > 1.0f)
	{
		m_razer.fIrradiationTime = 1.0f;
	}
	m_razer.fNowLength = INIT_RAZER_LENGTH + MyMath::Lerp(m_razer.fIrradiationTime * m_razer.fIrradiationTime * m_razer.fIrradiationTime, INIT_RAZER_LENGTH, MAX_RAZER_LENGTH);
	Vector3 vDir;
	int i = 0;
	for (const auto& itr : m_pRotateRazerList)
	{
		if (itr.expired())continue;
		vDir = Vector3(cosf(DirectX::XMConvertToRadians(m_razer.fMainAngle) + DirectX::XMConvertToRadians(m_razer.fIntervalAngle * i)), 0.0f, sinf(DirectX::XMConvertToRadians(m_razer.fMainAngle) + DirectX::XMConvertToRadians(m_razer.fIntervalAngle * i)));
		vDir.Normalize();
		itr.lock()->m_pOwner.lock()->EnableActive();
		itr.lock()->SetLength(m_razer.fNowLength);
		itr.lock()->SetPos(m_pTransform.lock()->localpos + vDir * (m_razer.fNowLength * 0.5f + 1.0f));
		++i;
	}

	return true;
}

const bool MasterWitch::VanishRazer()
{
	if (m_razer.fVanishTime >= 1.0f)return true;

	m_razer.fVanishTime += Clocker::GetInstance().GetFrameTime() / 0.3f;
	if (m_razer.fVanishTime > 1.0f)
	{
		m_razer.fVanishTime = 1.0f;
	}
	m_razer.fNowLength = INIT_RAZER_LENGTH + MyMath::Lerp(1 - (m_razer.fVanishTime * m_razer.fVanishTime * m_razer.fVanishTime), INIT_RAZER_LENGTH, MAX_RAZER_LENGTH);
	Vector3 vDir;
	int i = 0;
	for (const auto& itr : m_pRotateRazerList)
	{
		if (itr.expired())continue;
		vDir = Vector3(cosf(DirectX::XMConvertToRadians(m_razer.fMainAngle) + DirectX::XMConvertToRadians(m_razer.fIntervalAngle * i)), 0.0f, sinf(DirectX::XMConvertToRadians(m_razer.fMainAngle) + DirectX::XMConvertToRadians(m_razer.fIntervalAngle * i)));
		vDir.Normalize();
		itr.lock()->m_pOwner.lock()->EnableActive();
		itr.lock()->SetLength(m_razer.fNowLength);
		itr.lock()->SetPos(m_pTransform.lock()->localpos + vDir * ((MAX_RAZER_LENGTH * 2.0f - m_razer.fNowLength) * 0.5f + 1.0f));
		++i;
	}

	return false;
}

const bool MasterWitch::ChargeRotateRazer()
{
	if (m_razer.fChargeTime >= CHARGE_RAZER_TIME)return true;

	m_razer.fChargeTime += Clocker::GetInstance().GetFrameTime();	
	Vector3 vDir;
	int i = 0;
	for (const auto& itr : m_pRotateRazerList)
	{
		if (itr.expired())continue;
		vDir = Vector3(cosf(DirectX::XMConvertToRadians(m_razer.fMainAngle) + DirectX::XMConvertToRadians(m_razer.fIntervalAngle * i)), 0.0f, sinf(DirectX::XMConvertToRadians(m_razer.fMainAngle) + DirectX::XMConvertToRadians(m_razer.fIntervalAngle * i)));
		vDir.Normalize();
		itr.lock()->m_pOwner.lock()->EnableActive();
		itr.lock()->SetDirection(vDir);
		itr.lock()->SetLength(INIT_RAZER_LENGTH);
		itr.lock()->SetPos(m_pTransform.lock()->localpos + vDir * (INIT_RAZER_LENGTH * 0.5f + 1.0f));
		++i;
	}

	return false;
}

const bool MasterWitch::AttackRotateRazer()
{
	if (m_razer.fMainAngle <= 0.0f)return true;

	m_razer.fMainAngle -= (DirectX::XMConvertToDegrees(Clocker::GetInstance().GetFrameTime()) / 0.5f);
	Vector3 vDir;
	int i = 0;
	for (const auto& itr : m_pRotateRazerList)
	{
		if (itr.expired())continue;
		vDir = Vector3(cosf(DirectX::XMConvertToRadians(m_razer.fMainAngle) + DirectX::XMConvertToRadians(m_razer.fIntervalAngle * i)), 0.0f, sinf(DirectX::XMConvertToRadians(m_razer.fMainAngle) + DirectX::XMConvertToRadians(m_razer.fIntervalAngle * i)));
		vDir.Normalize();
		itr.lock()->m_pOwner.lock()->EnableActive();
		itr.lock()->SetDirection(vDir);
		itr.lock()->SetLength(m_razer.fNowLength);
		itr.lock()->SetPos(m_pTransform.lock()->localpos + vDir * (m_razer.fNowLength * 0.5f + 1.0f));
		++i;
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
	if (m_bossState == Witch_State::Boss::WAIT)
	{
		m_fWaitTime = 0.0f;
		int type = rand() % (Witch_State::Boss::MAX - 1);
		++type;
		return type;
	}
	else
	{
		m_razer.fChargeTime = 0.0f;
		m_razer.fMainAngle = 360.0f;
		m_razer.fIrradiationTime = 0.0f;
		m_razer.fNowLength = INIT_RAZER_LENGTH;
		m_razer.fVanishTime = 0.0f;
		m_rotatebullet.fMainLineNowAngle = 360.0f;
		m_rotatebullet.fMainLineOldAngle = 360.0f;
		m_rotatebullet.num = 0;
		return Witch_State::Boss::WAIT;
	}
	return Witch_State::Boss::WAIT;
}

const int MasterWitch::ResetBossState()
{
	return Witch_State::Boss::WAIT;
}