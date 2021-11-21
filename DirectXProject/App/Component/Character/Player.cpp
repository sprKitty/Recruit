#include "Player.h"
#include <App/Component/Object.h>
#include <App/Component/Renderer/BillBoardRenderer.h>
#include <System/Clocker.h>
#include <System/Input.h>
#include <MyMath.h>

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
	m_Transform = m_pOwner.lock()->GetTransform();
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

	m_pOwner.lock()->SetTransform(m_Transform);
}

const bool Player::DestinationCollision()
{
	Vector2 vDis(m_Transform.pos.x - m_vDestination.x, m_Transform.pos.z - m_vDestination.y);
	float fLength = vDis.Length();
	if (fLength < OneSecMoveSpeed * 0.05f)
	{
		m_vMove = 0;
	}
	
	return true;
}

const bool Player::AttackAction()
{
	if (m_isAttack)
	{
		float fRad = MyMath::Radian(m_Transform.pos.x, m_Transform.pos.z, m_vMousePos.x, m_vMousePos.z);
		m_Direction = CalcDirection4(DEG(fRad));
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
	Vector2 vMove = m_vMove;
	vMove.Abs();

	if (m_isAttack)
	{
		return Player_State::ATTACK;
	}

	if (vMove.x < 0.001f
		&& vMove.y < 0.001f)
	{
		return Player_State::WAIT;
	}
	return Player_State::WALK;
}

const int Player::WalkStateChange()
{
	Vector2 vMove = m_vMove;
	vMove.Abs();
	
	if (m_isAttack)
	{
		return Player_State::ATTACK;
	}

	if (vMove.x < 0.001f
		&& vMove.y < 0.001f)
	{
		return Player_State::WAIT;
	}
	return Player_State::WALK;
}

const int Player::AttackStateChange()
{
	Vector2 vMove = m_vMove;
	vMove.Abs();
	float fRad = MyMath::Radian(m_Transform.pos.x, m_Transform.pos.z, m_Transform.pos.x + m_vMove.x, m_Transform.pos.z + m_vMove.y);
	m_Direction = CalcDirection8(DEG(fRad));
	if (vMove.x < 0.001f
		&& vMove.y < 0.001f)
	{
		return Player_State::WAIT;
	}

	return Player_State::WALK;
}

const bool Player::Move()
{
	m_Transform.move.x = m_vMove.x * static_cast<float>(Clocker::GetInstance().GetFrameTime());
	m_Transform.move.z = m_vMove.y * static_cast<float>(Clocker::GetInstance().GetFrameTime());
	m_Transform.pos += m_Transform.move;
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
		m_vDestination.x = m_vMousePos.x;
		m_vDestination.y = m_vMousePos.z;
		m_vMove.x = m_vDestination.x - m_Transform.pos.x;
		m_vMove.y = m_vDestination.y - m_Transform.pos.z;
		m_vMove.Normalize();
		m_vMove *= OneSecMoveSpeed;
		float fRad = MyMath::Radian(m_Transform.pos.x, m_Transform.pos.z, m_Transform.pos.x + m_vMove.x, m_Transform.pos.z + m_vMove.y);
		m_Direction = CalcDirection8(DEG(fRad));
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
