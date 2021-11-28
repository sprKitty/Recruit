#pragma once
#include <App/Component/Character/Character.h>
#include<System/ClassDesign/StateBase.h>


class Object;
class MagicBullet;

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

	inline void SetTarget(const std::weak_ptr<Object>& pTarget)
	{
		m_pTarget = pTarget;
	}

private:
	const bool CalcTarget();
	const bool Wait();
	const bool Attack1();

	const int MasterFromBoss();
	const int ChangeBossState();
	const int ResetBossState();

private:
	static float WAIT_TIME;

	Witch_State::Master::Kind m_masterState;
	Witch_State::Boss::Kind m_bossState;
	Witch_State::Kind m_state;
	std::vector<std::weak_ptr<MagicBullet> > m_pAttackList;
	std::weak_ptr<Object> m_pTarget;
	State<MasterWitch>::PTRLIST m_pMasterStateList;
	State<MasterWitch>::PTRLIST m_pBossStateList;
	float m_fWaitTime;
};