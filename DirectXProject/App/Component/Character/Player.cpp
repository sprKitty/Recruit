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
	//"Assets/csv/playerwalk.csv",
	//"Assets/csv/playerwalk.csv",
};

void Player::Init()
{
	m_pTransform = m_pOwner.lock()->GetComponent<Transform>();
	m_pTransform.lock()->localpos = { 1,0.5f,1 };
	m_vDestination = 0;
	m_vMove = 0;
	m_isMove = false;
	m_isAttack = false;
	m_Direction = Chara_Direction::DOWN;
	m_isChangeDestination = false;
	m_state = Player_State::WAIT;
	for (int i = 0; i < Player_State::MAX; ++i)
	{
		std::shared_ptr<TexAnimation> pImage(new TexAnimation());
		pImage->LoadData(g_pPlayerAnimPath[i]);
		m_pTexAnimList.push_back(pImage);
	}

	std::weak_ptr<Player> pThis = std::dynamic_pointer_cast<Player>(weak_from_this().lock());
	if (!pThis.expired())
	{
		for (int i = 0; i < Player_State::MAX; ++i)
		{
			State<Player>::PTR pState(new State<Player>());
			m_pStateList.push_back(std::move(pState));
		}

		m_pStateList[Player_State::WAIT]->AddActionFunc(pThis, &Player::CalcDestination);
		m_pStateList[Player_State::WAIT]->AddActionFunc(pThis, &Player::DestinationCollision);
		m_pStateList[Player_State::WAIT]->SetTransitionFunc(pThis, &Player::WaitStateChange);

		m_pStateList[Player_State::WALK]->AddActionFunc(pThis, &Player::CalcDestination);
		m_pStateList[Player_State::WALK]->AddActionFunc(pThis, &Player::DestinationCollision);
		m_pStateList[Player_State::WALK]->AddActionFunc(pThis, &Player::Move);
		m_pStateList[Player_State::WALK]->SetTransitionFunc(pThis, &Player::WalkStateChange);

		m_pStateList[Player_State::ATTACK]->AddActionFunc(pThis, &Player::AttackAction);
		m_pStateList[Player_State::ATTACK]->SetTransitionFunc(pThis, &Player::AttackStateChange);

	}
}

void Player::Uninit()
{
	m_pStateList.clear();
}

void Player::Update()
{
	Player_State::Kind oldState = m_state;

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
		m_pBBR.lock()->SetMainImage(m_pTexAnimList[m_state]);
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
				//pMB.lock()->m_pOwner.lock()->Update();
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

const int Player::WaitStateChange()
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

const int Player::WalkStateChange()
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

const int Player::AttackStateChange()
{
	Vector3 vPos = m_pTransform.lock()->localpos;
	Vector3 vMove = m_vMove;
	vMove.Abs();
	float fRad = MyMath::Radian(vPos.x, vPos.z, vPos.x + m_vMove.x, vPos.z + m_vMove.z);
	m_Direction = CalcDirection8(DirectX::XMConvertToDegrees(fRad));
	if (vMove.x < 0.001f
		&& vMove.z < 0.001f)
	{
		return Player_State::WAIT;
	}

	return Player_State::WALK;
}

const bool Player::Move()
{
	m_pTransform.lock()->localpos.x += m_vMove.x * Clocker::GetInstance().GetFrameTime();
	m_pTransform.lock()->localpos.z += m_vMove.z * Clocker::GetInstance().GetFrameTime();

	return true;
}

void Player::SetMousePos(const Vector3 & vPos)
{
	m_vMousePos = vPos;
}

const bool Player::CalcDestination()
{
	if (m_isChangeDestination)
	{ 
		Vector3 vPos = m_pTransform.lock()->localpos;
		m_vDestination.x = m_vMousePos.x;
		m_vDestination.z = m_vMousePos.z;
		m_vMove.x = m_vDestination.x - vPos.x;
		m_vMove.z = m_vDestination.z - vPos.z;
		m_vMove.Normalize();
		m_vMove *= OneSecMoveSpeed;
		float fRad = MyMath::Radian(vPos.x, vPos.z, vPos.x + m_vMove.x, vPos.z + m_vMove.z);
		m_Direction = CalcDirection8(DirectX::XMConvertToDegrees(fRad));
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
