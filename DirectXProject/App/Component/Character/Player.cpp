#include "Player.h"
#include <App/Component/Object.h>
#include <App/Component/Image.h>
#include <App/Component/Renderer/BillBoardRenderer.h>
#include <System/Clocker.h>
#include <System/Input.h>

void Player::Init()
{
	m_vDestination = 0;
	m_vMove = 0;
	for (int i = 0; i < static_cast<int>(Player::State::MAX); ++i)
	{
		std::shared_ptr<Image> pImage(new Image());
		m_pImageList.push_back(pImage);
	}

	m_pImageList[static_cast<int>(Player::State::WAIT)]->SetPath("Assets/samp.png");
	m_pImageList[static_cast<int>(Player::State::WALK)]->SetPath("Assets/samp.png");
	m_pImageList[static_cast<int>(Player::State::WALK)]->m_vTiling = Vector2(0.166f, 0.25f);
	m_pImageList[static_cast<int>(Player::State::ATTACK)]->SetPath("Assets/samp.png");
	m_pImageList[static_cast<int>(Player::State::SKILL1)]->SetPath("Assets/samp.png");
	m_pImageList[static_cast<int>(Player::State::SKILL2)]->SetPath("Assets/samp.png");
}

void Player::Uninit()
{
}

void Player::Update()
{
	m_Transform = m_pOwner.lock()->GetTransform();
	DestinationCollision();
	Move();

	if (!m_pBBR.expired())
	{
		m_pBBR.lock()->SetMainImage(m_pImageList[static_cast<int>(Player::State::WALK)]);
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
	}
}
