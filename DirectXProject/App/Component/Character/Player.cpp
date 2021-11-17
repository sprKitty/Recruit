#include "Player.h"
#include <App/Component/Object.h>
#include <App/Component/Renderer/BillBoardRenderer.h>
#include <System/Clocker.h>
#include <System/Input.h>
#include <MyMath.h>

const char* g_pPlayerAnimPath[Player_State::MAX] =
{
	"Assets/csv/playerwalk.csv",
	"Assets/csv/playerwalk.csv",
	"Assets/csv/playerwalk.csv",
	"Assets/csv/playerwalk.csv",
	"Assets/csv/playerwalk.csv",
};

void Player::Init()
{
	m_vDestination = 0;
	m_vMove = 0;
	for (int i = 0; i < Player_State::MAX; ++i)
	{
		std::shared_ptr<TexAnimation> pImage(new TexAnimation());
		pImage->LoadData(g_pPlayerAnimPath[i]);
		m_pTexAnimList.push_back(pImage);
	}

	m_Direction = Chara_Direction::DOWN;
	m_isMove = false;
}

void Player::Uninit()
{
}

void Player::Update()
{
	m_Transform = m_pOwner.lock()->GetTransform();
	DestinationCollision();
	Move();
	if (!m_isMove)
	{
		m_pTexAnimList[Player_State::WALK]->Reset(m_Direction);
	}
	else
	{
		m_pTexAnimList[Player_State::WALK]->Update(m_Direction);
	}
	if (!m_pBBR.expired())
	{
		m_pBBR.lock()->SetMainImage(m_pTexAnimList[Player_State::WALK]);
	}
	m_pOwner.lock()->SetTransform(m_Transform);
}

void Player::DestinationCollision()
{
	Vector2 vDis(m_Transform.pos.x - m_vDestination.x, m_Transform.pos.z - m_vDestination.y);
	float fLength = vDis.Length();
	if (fLength < OneSecMoveSpeed * 0.05f)
	{
		m_vMove = 0;
		m_isMove = false;
	}
	else
	{
		m_isMove = true;
	}
}

void Player::Move()
{
	m_Transform.move.x = m_vMove.x * static_cast<float>(Clocker::GetInstance().GetFrameTime());
	m_Transform.move.z = m_vMove.y * static_cast<float>(Clocker::GetInstance().GetFrameTime());
	m_Transform.pos += m_Transform.move;
}

void Player::CalcDestination(const Vector3 & vPos)
{
	if (IsKeyPress(VK_LBUTTON))
	{ 
		m_vDestination.x = vPos.x;
		m_vDestination.y = vPos.z;
		m_vMove.x = m_vDestination.x - m_Transform.pos.x;
		m_vMove.y = m_vDestination.y - m_Transform.pos.z;
		m_vMove.Normalize();
		m_vMove *= OneSecMoveSpeed;
		float fRad = MyMath::Radian(m_Transform.pos.x, m_Transform.pos.y, m_Transform.pos.x + m_vMove.x, m_Transform.pos.y + m_vMove.y);
		m_Direction = CalcDirection(DEG(fRad));
	}
}
