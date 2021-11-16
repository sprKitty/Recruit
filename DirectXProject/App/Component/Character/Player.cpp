#include "Player.h"
#include <App/Component/Object.h>
#include <App/Component/Image.h>
#include <App/Component/Renderer/BillBoardRenderer.h>
#include <App/TexAnimation.h>
#include <System/Clocker.h>
#include <System/Input.h>

void Player::Init()
{
	m_vDestination = 0;
	m_vMove = 0;
	for (int i = 0; i < Player_State::MAX; ++i)
	{
		std::shared_ptr<TexAnimation> pImage(new TexAnimation());
		m_pTexAnimList.push_back(pImage);
	}

	Vector2 vTiling = Vector2(0.167f, 0.25f);
	//m_pTexAnimList[Player_State::WAIT]->LoadData("Assets/samp.png");
	m_pTexAnimList[Player_State::WALK]->LoadData("Assets/csv/playerwalk.csv");
	//m_pTexAnimList[Player_State::ATTACK]->SetPath("Assets/samp.png");
	//m_pTexAnimList[Player_State::SKILL1]->SetPath("Assets/samp.png");
	//m_pTexAnimList[Player_State::SKILL2]->SetPath("Assets/samp.png");
	m_nAnimFrame = 0;
	m_Direction = Chara_Direction::DOWN;
}

void Player::Uninit()
{
}

void Player::Update()
{
	m_Transform = m_pOwner.lock()->GetTransform();
	DestinationCollision();
	Move();
	m_pTexAnimList[Player_State::WALK]->Update(m_Direction);
	if (!m_pBBR.expired())
	{
		m_pBBR.lock()->SetMainImage(m_pTexAnimList[Player_State::WALK]);
	}
}

void Player::DestinationCollision()
{
	Vector2 vDis(m_Transform.pos.x - m_vDestination.x, m_Transform.pos.z - m_vDestination.y);
	float fLength = vDis.Length();
	if (fLength < OneSecMoveSpeed * 0.05f)
	{
		m_vMove = 0;
	}
}

void Player::Move()
{
	m_Transform.move.x = m_vMove.x * static_cast<float>(Clocker::GetInstance().GetFrameTime());
	m_Transform.move.z = m_vMove.y * static_cast<float>(Clocker::GetInstance().GetFrameTime());
	m_Transform.pos += m_Transform.move;
	m_pOwner.lock()->SetTransform(m_Transform);
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
