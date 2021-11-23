#include "Transform.h"
#include <App/Component/Object.h>


void Transform::Init()
{
	localpos = 0;
	localrot = 0;
	localscale = 1;
	m_mMatrix = m_mWorldMatrix = DirectX::XMMatrixIdentity();
	//DirectX::XMStoreFloat4x4(&world, DirectX::XMMatrixIdentity());
}

void Transform::Uninit()
{
}

void Transform::Update()
{
	//m_mMatrix = DirectX::XMLoadFloat4x4(&world);
	DirectX::XMMATRIX localMatrix = MyMath::ConvertMatrix(localscale, localrot, localpos);
	m_mWorldMatrix = m_mMatrix * localMatrix;
}
