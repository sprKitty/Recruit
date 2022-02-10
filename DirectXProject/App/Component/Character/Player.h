#pragma once
#include "Character.h"
#include <System/ClassDesign/StateBase.h>
#include <App/Fade/Character_Fade.h>
#include <App/Fade/MagicUI_Fade.h>

namespace Player_State
{
	enum Kind
	{
		WAIT,
		WALK,
		ATTACK,
		HEAL,
		TELEPORT,

		MAX,
	};
}

class RootMotion;
class Renderer2D;

class Player : public Character
{
private:
	struct TeleportInfo
	{
		bool isBefore;
		bool isAfter;
		float fTime; // クールタイム
		float fMaxTime; // クールタイム
		Vector3 vPos; // テレポートする座標
		bool isEnable;
	};

public:
	Player() {}
	~Player()override {}

	void Init() override;
	void Uninit() override;
	void Update() override;

	void SetMousePos(const Vector3& vPos);
	void EnableChangeDestination();
	void EnableAttack();
	void EnableHeal();
	void EnableTeleport();

protected:
	const bool Move()override;

private:
	const bool UpdateTeleportTimer();
	const bool SetDestination();
	const bool CollisionMove();
	const bool CalcDestination();
	const bool DestinationCollision();
	const bool AttackAction();
	const bool BeforeTeleport();
	const bool Teleporting();

	const UINT8 WaitStateChange();
	const UINT8 WalkStateChange();
	const UINT8 AttackStateChange();
	const UINT8 Teleported();


private:
	const float OneSecMoveSpeed = 3.0f;

	std::shared_ptr<Character_Fade> m_pCharacterFade;
	std::shared_ptr<MagicUI_Fade> m_pTeleportUIFade;
	std::weak_ptr<Object> m_pTalkOccurObj;
	std::weak_ptr<Renderer2D> m_pLookTeleportBG;
	std::weak_ptr<Renderer2D> m_pLookTeleport;
	std::weak_ptr<RootMotion> m_pRootMotion;
	State<Player>::PTRLIST m_pStateList;
	TeleportInfo m_teleportInfo;
	Player_State::Kind m_state;
	Vector3 m_vMousePos;
	Vector3 m_vDestination;
	Vector3 m_vKeepDestination;
	Vector3 m_vMove;
	Vector3 m_vOldPos;
	bool m_isMove;
	bool m_isAttack;
	bool m_isChangeDestination;
};