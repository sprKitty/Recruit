#pragma once
#include <App/Component/Character/Character.h>
#include <App/Component/Object.h>
#include<System/ClassDesign/StateBase.h>


class MagicBullet;
class MagicBall;
class MagicRazer;
class EventTrigger;

namespace Witch_State
{
	enum Kind
	{
		MASTER,
		BOSS,
		MAX,
	};

	namespace Master
	{
		enum Kind
		{
			WAIT = 0,
			MAX
		};
	};

	namespace Boss
	{
		enum Kind
		{
			WAIT = 0,
			ATTACK1 = 1,
			ATTACK2 = 2,
			ATTACK3 = 3,
			//ROTATERAZER = 4,
			MAX,
		};
	}
}

class MasterWitch : public Character
{
public:
	MasterWitch() {}
	~MasterWitch()override {}

	void Init()override;
	void Uninit()override;
	void Update()override;

	inline void SetTarget(const std::weak_ptr<Object>& pTarget) { m_pTarget = pTarget; }

private:
	struct RotateBullet
	{
		float fChargeTime = 0;
		int num = 0;
		int nRate = 0;
		int nIntervalNum = 0;
		int nLineNum = 0;
		float fLineAngle = 0;
		float fMainLineOldAngle = 360.0f;
		float fMainLineNowAngle = 360.0f;
	};

	struct Razer
	{
		int num = 0;
		float fNowLength = 0;
		float fIntervalAngle = 0;
		float fMainAngle = 360;
		float fChargeTime = 0;
		float fIrradiationTime = 0;
		float fVanishTime = 0.0f;
	};

private:
	const bool HitUpdate();
	
	const bool CalcTarget();

	const bool Wait();
	
	const bool Attack1();
	const bool Attack2();
	const bool Attack3();

	const bool IrradiationRazer();
	const bool VanishRazer();
	const bool ChargeRotateRazer();
	const bool AttackRotateRazer();

	const int MasterFromBoss();
	const int ChangeBossState();
	const int ResetBossState();

private:
	static float WAIT_TIME;
	static float CHARGE_RAZER_TIME;
	static float INIT_RAZER_LENGTH;
	static float MAX_RAZER_LENGTH;

	Witch_State::Master::Kind m_masterState;
	Witch_State::Boss::Kind m_bossState;
	Witch_State::Kind m_state;
	Object::WORKER_OBJECTLIST m_pAttackObject;
	std::vector<std::weak_ptr<MagicBullet> > m_pAttackList1;
	std::vector<std::weak_ptr<MagicBullet> > m_pAttackList2;
	std::vector<std::weak_ptr<MagicBall> > m_pAttackList3;
	std::vector<std::weak_ptr<MagicRazer>> m_pRotateRazerList;
	std::weak_ptr<Object> m_pTarget;
	State<MasterWitch>::PTRLIST m_pMasterStateList;
	State<MasterWitch>::PTRLIST m_pBossStateList;
	float m_fWaitTime;

	Razer m_razer;
	RotateBullet m_rotatebullet;
};