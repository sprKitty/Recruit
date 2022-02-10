#include "Player.h"
#include <App/Component/Object.h>
#include <App/Component/Transform.h>
#include <App/Component/Magic/MagicBullet.h>
#include <App/Component/Renderer/BillBoardRenderer.h>
#include <System/Clocker.h>
#include <System/Input.h>
#include <MyMath.h>
#include <App/FactoryMethod.h>


const char* g_pPlayerAnimPath[Player_State::MAX] =
{
	"Assets/csv/playerwait.csv",
	"Assets/csv/playerwalk.csv",
	"Assets/csv/playerattack.csv",
	"Assets/csv/playerwait.csv",
	"Assets/csv/playerwait.csv",
};

void Player::Init()
{
	m_pTransform = m_pOwner.lock()->GetComponent<Transform>();
	m_pTransform.lock()->localpos.y = 0.4f;
	m_vDestination = 0;
	m_vMove = 0;
	m_teleportInfo.isBefore = m_teleportInfo.isAfter = false;
	m_teleportInfo.fTime = m_teleportInfo.fMaxTime = 1.f;
	m_teleportInfo.vPos = Vector3(0.f, 0.4f, 0.f);
	m_teleportInfo.isEnable = false;
	m_isMove = false;
	m_isAttack = false;
	m_Direction = Chara_Direction::DOWN;
	m_isChangeDestination = false;
	m_state = Player_State::WAIT;

	m_pCharacterFade.reset(new Character_Fade);
	m_pCharacterFade->Init();
	m_pCharacterFade->inOutSpeed.set(0.5f);

	m_pTeleportUIFade.reset(new MagicUI_Fade);
	m_pTeleportUIFade->Init();
	m_pTeleportUIFade->inOutSpeed.set(m_teleportInfo.fMaxTime);

	std::weak_ptr<Object> pObject;
	pObject = FactoryMethod::GetInstance().CreateMagicUI();
	m_pLookTeleport = pObject.lock()->GetComponent<Renderer2D>();
	if (!m_pLookTeleport.expired())
	{
		m_pLookTeleport.lock()->m_Image.SetTexture("teleport.png");
		Renderer2D::RectTransform rect;
		rect.pos = Vector3(-SCREEN_WIDTH * 0.0f, SCREEN_HEIGHT * 0.4f, 8);
		rect.scale = Vector2(SCREEN_WIDTH * 0.1f, SCREEN_HEIGHT * 0.1f);
		m_pLookTeleport.lock()->SetRectTransform(rect);
		m_pLookTeleport.lock()->SetFadeAnimation(m_pTeleportUIFade);
	}

	pObject = FactoryMethod::GetInstance().CreateMagicUI();
	m_pLookTeleportBG = pObject.lock()->GetComponent<Renderer2D>();
	if (!m_pLookTeleportBG.expired())
	{
		m_pLookTeleportBG.lock()->m_Image.SetTexture("teleport.png");
		Renderer2D::RectTransform rect;
		rect.pos = Vector3(-SCREEN_WIDTH * 0.0f, SCREEN_HEIGHT * 0.4f, 9);
		rect.scale = Vector2(SCREEN_WIDTH * 0.1f, SCREEN_HEIGHT * 0.1f);
		m_pLookTeleportBG.lock()->SetRectTransform(rect);
		m_pLookTeleportBG.lock()->m_Image.m_vMultiplyColor = Vector4(0.5f, 0.5f, 0.5f, 0.5f);
	}

	for (int i = 0; i < Player_State::MAX; ++i)
	{
		std::shared_ptr<TexAnimation> pImage(new TexAnimation());
		pImage->LoadData(g_pPlayerAnimPath[i]);
		m_pTexAnimList.emplace_back(pImage);
	}

	std::weak_ptr<Player> pThis = std::dynamic_pointer_cast<Player>(shared_from_this());
	if (!pThis.expired())
	{
		for (int i = 0; i < Player_State::MAX; ++i)
		{
			State<Player>::PTR pState(new State<Player>());
			m_pStateList.emplace_back(std::move(pState));
		}

		m_pStateList[Player_State::WAIT]->AddActionFunc(pThis, &Player::UpdateTeleportTimer);
		m_pStateList[Player_State::WAIT]->AddActionFunc(pThis, &Player::SetDestination);
		m_pStateList[Player_State::WAIT]->AddActionFunc(pThis, &Player::CollisionMove);
		m_pStateList[Player_State::WAIT]->AddActionFunc(pThis, &Player::CalcDestination);
		m_pStateList[Player_State::WAIT]->AddActionFunc(pThis, &Player::DestinationCollision);
		m_pStateList[Player_State::WAIT]->SetTransitionFunc(pThis, &Player::WaitStateChange);

		m_pStateList[Player_State::WALK]->AddActionFunc(pThis, &Player::UpdateTeleportTimer);
		m_pStateList[Player_State::WALK]->AddActionFunc(pThis, &Player::SetDestination);
		m_pStateList[Player_State::WALK]->AddActionFunc(pThis, &Player::CollisionMove);
		m_pStateList[Player_State::WALK]->AddActionFunc(pThis, &Player::CalcDestination);
		m_pStateList[Player_State::WALK]->AddActionFunc(pThis, &Player::DestinationCollision);
		m_pStateList[Player_State::WALK]->AddActionFunc(pThis, &Player::Move);
		m_pStateList[Player_State::WALK]->SetTransitionFunc(pThis, &Player::WalkStateChange);

		m_pStateList[Player_State::ATTACK]->AddActionFunc(pThis, &Player::AttackAction);
		m_pStateList[Player_State::ATTACK]->SetTransitionFunc(pThis, &Player::AttackStateChange);

		m_pStateList[Player_State::TELEPORT]->AddActionFunc(pThis, &Player::BeforeTeleport);
		m_pStateList[Player_State::TELEPORT]->AddActionFunc(pThis, &Player::Teleporting);
		m_pStateList[Player_State::TELEPORT]->SetTransitionFunc(pThis, &Player::Teleported);
	}
}

void Player::Uninit()
{
	m_pCharacterFade->Uninit();
	m_pStateList.clear();
}

void Player::Update()
{
	Player_State::Kind oldState = m_state;
	m_vOldPos = m_pTransform.lock()->localpos;

	if (m_pRootMotion.expired())
	{
		m_pRootMotion = m_pOwner.lock()->GetComponent<RootMotion>();
	}
	SetNeedComponent();

	if (m_pStateList[m_state]->Action())
	{
		m_state = static_cast<Player_State::Kind>(m_pStateList[m_state]->Next());
	}

	if (m_state != oldState)
	{
		m_pTexAnimList[m_state]->Reset(m_Direction);
	}

	m_pTexAnimList[m_state]->Update(m_Direction);

	if (!m_pBBR.expired())
	{
		m_pBBR.lock()->SetMainTexAnimation(m_pTexAnimList[m_state]);
		m_pBBR.lock()->SetFadeAnimation(m_pCharacterFade);
	}

	if (!PTRNULLCHECK(m_pTeleportUIFade))
	{
		m_pTeleportUIFade->time.set(m_teleportInfo.fTime);
	}

}

const bool Player::DestinationCollision()
{
	Vector3 vPos = m_pTransform.lock()->localpos;
	Vector2 vDis(vPos.x - m_vDestination.x, vPos.z - m_vDestination.z);
	float fLength = vDis.Length();
	if (fLength < OneSecMoveSpeed * 0.05f)
	{
		m_vMove = 0;
	}
	
	return true;
}

const bool Player::AttackAction()
{
	Vector3 vPos = m_pTransform.lock()->localpos;
	if (m_isAttack)
	{
		float fRad = MyMath::Radian(vPos.x, vPos.z, m_vMousePos.x, m_vMousePos.z);
		m_Direction = CalcDirection4(DirectX::XMConvertToDegrees(fRad));
		Object::WORKER_OBJ pMagicObject = FactoryMethod::GetInstance().CreatePlayerMagic();

		if (!pMagicObject.expired())
		{
			std::weak_ptr<MagicBullet> pMB = pMagicObject.lock()->GetComponent<MagicBullet>();
			if (!pMB.expired())
			{
				Vector3 vDir = m_vMousePos - m_pTransform.lock()->localpos;
				vDir.y = 0.0f;
				vDir.Normalize();
				pMB.lock()->SetStartPos(vPos);
				pMB.lock()->SetType(MagicType::FIRE);
				pMB.lock()->SetDiretion(vDir);
			}
		}
		m_isAttack = false;
	}

	if (m_pTexAnimList[m_state]->IsFinish())
	{
		return true;
	}


	return false;
}

const bool Player::BeforeTeleport()
{
	if (!m_teleportInfo.isBefore)
	{
		m_pCharacterFade->StartFadeIn();
		m_teleportInfo.isBefore = !m_pCharacterFade->Update();

		return (m_teleportInfo.isBefore) ? true : false;
	}
	return true;
}

const bool Player::Teleporting()
{
	m_pTransform.lock()->localpos = m_teleportInfo.vPos;
	if (!m_teleportInfo.isAfter)
	{
		m_pCharacterFade->StartFadeOut();
		m_teleportInfo.isAfter = !m_pCharacterFade->Update();
		return (m_teleportInfo.isAfter) ? true : false;
	}
	return true;
}

const UINT8 Player::Teleported()
{
	m_teleportInfo.isEnable = false;
	m_teleportInfo.fTime = 0.f;
	m_teleportInfo.isBefore = m_teleportInfo.isAfter = false;
	return Player_State::WAIT;
}

const UINT8 Player::WaitStateChange()
{
	Vector3 vMove = m_vMove;
	vMove.Abs();

	if (m_isAttack)
	{
		return Player_State::ATTACK;
	}

	if (vMove.x < 0.001f
		&& vMove.z < 0.001f)
	{
		return Player_State::WAIT;
	}
	return Player_State::WALK;
}

const UINT8 Player::WalkStateChange()
{
	Vector3 vMove = m_vMove;
	vMove.Abs();
	
	if (m_isAttack)
	{
		return Player_State::ATTACK;
	}

	if (vMove.x < 0.001f
		&& vMove.z < 0.001f)
	{
		return Player_State::WAIT;
	}
	return Player_State::WALK;
}

const UINT8 Player::AttackStateChange()
{
	Vector3 vPos = m_pTransform.lock()->localpos;
	Vector3 vMove = m_vMove;
	vMove.Abs();
	float fRad = MyMath::Radian(vPos.x, vPos.z, vPos.x + m_vMove.x, vPos.z + m_vMove.z);
	m_Direction = CalcDirection4(DirectX::XMConvertToDegrees(fRad));
	if (vMove.x < 0.001f
		&& vMove.z < 0.001f)
	{
		return Player_State::WAIT;
	}

	return Player_State::WALK;
}

const bool Player::Move()
{
	const float t = Clocker::GetInstance().DeltaTime();
	m_pTransform.lock()->localpos.x += m_vMove.x * t;
	m_pTransform.lock()->localpos.z += m_vMove.z * t;

	return true;
}

void Player::SetMousePos(const Vector3 & vPos)
{
	m_vMousePos = vPos;
}

const bool Player::UpdateTeleportTimer()
{
	m_teleportInfo.fTime += Clocker::GetInstance().DeltaTime();

	return true;
}

const bool Player::SetDestination()
{
	if (m_isChangeDestination)
	{
		m_vDestination.x = m_vMousePos.x;
		m_vDestination.z = m_vMousePos.z;
		m_vKeepDestination = m_vDestination;
		m_vMove = 0.f;
	}
	return true;
}

const bool Player::CollisionMove()
{
	if (m_pRootMotion.expired())return true;

	std::weak_ptr<Collider> pCollider = m_pOwner.lock()->GetComponent<Collider>();
	if (pCollider.expired())return true;

	Collider::HitInfo info = m_pCollider.lock()->GetHitInfo(CollisionType::AABB);
	bool isReCalc = false; // �Čv�Z����
	if (!m_pRootMotion.lock()->IsRootListEmpty())
	{
		m_isChangeDestination = true;
		if (!info.isFlg)	// �������Ă��Ȃ���������s�@�������Ă�����ēx�ŒZ�I��H���v�Z
		{
			m_vDestination = m_pRootMotion.lock()->CalcGuide(m_pTransform.lock()->localpos);
			return true;
		}
		else
		{
			isReCalc = true;
		}
	}

	if (!info.isFlg)return true; // �������Ă��Ȃ�������X�L�b�v
	if (info.pObj.expired())return true; // �I�u�W�F�N�g��񂪐ݒ肳��Ă��Ȃ�������X�L�b�v
	ObjectType::Kind type = info.pObj.lock()->GetType();

	if (type != ObjectType::STAGE)return true; // �֌W���Ȃ��I�u�W�F�N�g�Ȃ̂ŃX�L�b�v


	if (m_pRootMotion.lock()->IsRootListEmpty()
		|| isReCalc)
	{
		m_pTransform.lock()->localpos = m_vOldPos;
		m_pRootMotion.lock()->CalcRoot(m_vKeepDestination, m_pTransform.lock()->localpos, true);
	}
	m_vDestination = m_pRootMotion.lock()->CalcGuide(m_pTransform.lock()->localpos);
	m_isChangeDestination = true;

	return true;
}

const bool Player::CalcDestination()
{
	if (m_isChangeDestination) // �ړI�n�Čv�Z�t���O�������Ă��邩
	{ 
		Vector3 vPos = m_pTransform.lock()->localpos;

		// �ړ����������߈ړ����x�����肷��
		m_vMove.x = m_vDestination.x - vPos.x; 
		m_vMove.z = m_vDestination.z - vPos.z;
		m_vMove.Normalize();
		m_vMove *= OneSecMoveSpeed;
		
		// �ړ������̊p�x�����߃A�j���[�V�����ɓK������
		float fRad = MyMath::Radian(vPos.x, vPos.z, vPos.x + m_vMove.x, vPos.z + m_vMove.z);
		fRad = std::isnan(fRad) ? DirectX::XMConvertToRadians(-90.0f) : fRad;
		m_Direction = CalcDirection4(DirectX::XMConvertToDegrees(fRad));

		// �ړI�n�̌v�Z���I�����̂�false�ɂ���
		m_isChangeDestination = false;
	}
	return true;
}

void Player::EnableChangeDestination()
{
	if (m_state == Player_State::WAIT
		|| m_state == Player_State::WALK)
	{
		m_isChangeDestination = true;
		if (!m_pRootMotion.expired())
		{
			m_pRootMotion.lock()->ClearRootList();
		}
	}
}

void Player::EnableAttack()
{
	if (m_state == Player_State::WAIT
		|| m_state == Player_State::WALK)
	{
		m_isAttack = true;
	}
}

void Player::EnableHeal()
{
}

void Player::EnableTeleport()
{
	// �U������
	if (m_isAttack)return;
	
	// �e���|�[�g����
	if (m_teleportInfo.isEnable)return;
	
	// �e���|�[�g�̃N�[���^�C�����I����Ă��邩
	if (m_teleportInfo.fTime >= m_teleportInfo.fMaxTime)
	{
		m_state = Player_State::TELEPORT;
		m_vMove = 0.f;
		m_vDestination = m_pTransform.lock()->localpos;
		m_teleportInfo.fTime = 0.f;
		m_teleportInfo.vPos = m_vMousePos;
		m_teleportInfo.vPos.y = 0.4f;
		m_teleportInfo.isEnable = true;

		// ���[�g�T���R���|�[�l���g�����邩
		if (!m_pRootMotion.expired()) m_pRootMotion.lock()->ClearRootList();
	}
}
