#include "Player.h"
#include "Object.h"
#include "Collider.h"
#include "Land.h"
#include <System/Input.h>

Player::Player()
{
}

Player::~Player()
{
}

void Player::Init()
{
	m_pOwner->AddComponent<Collider>();
	m_pOwner->GetComponent<Collider>()->m_EditCollider.pos = Vector3(0, 10, 1);
	m_pOwner->GetComponent<Collider>()->m_EditCollider.scale = Vector3(10, 20, 6);
}

void Player::Uninit()
{
}

void Player::Update()
{
	//m_pOwner->AddressTransform().move.y -= 0.01f;
	//m_pOwner->AddressTransform().move.z -= 0.1f;
	if (IsKeyPress('W'))
		m_pOwner->AddressTransform().move.z -= 0.3f;
	if (IsKeyPress('A'))
		m_pOwner->AddressTransform().move.x += 0.3f;
	if (IsKeyPress('S'))
		m_pOwner->AddressTransform().move.z += 0.3f;
	if (IsKeyPress('D'))
		m_pOwner->AddressTransform().move.x -= 0.3f;
	//if (IsKeyPress(VK_LEFT))
	//	m_pOwner->AddressTransform().rot.x++;
	//if (IsKeyPress(VK_UP))
	//	m_pOwner->AddressTransform().rot.y++;
	//if (IsKeyPress(VK_RIGHT))
	//	m_pOwner->AddressTransform().rot.z++;
	if (IsKeyTrigger('R'))
		m_pOwner->AddressTransform().rot = 0;

	m_pOwner->AddressTransform().pos += m_pOwner->AddressTransform().move;
	//float length = MyMath::Length(m_pOwner->AddressTransform().move);
	//Vector3 dir = m_pOwner->GetTransform().move / length;

	XMFLOAT3 X;
	bool isLand = false;
	isLand = Land::m_pLand->Collision(Vector3(m_pOwner->GetTransform().pos.x, m_pOwner->GetTransform().pos.y + 1000, m_pOwner->GetTransform().pos.z), Vector3(0, -1.0f, 0), &X, nullptr);
	if (isLand)
		m_pOwner->AddressTransform().pos = MyMath::XM3ConvertV3(X);
}

void Player::Draw()
{
}
