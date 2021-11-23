#include "Transform.h"


void Transform::Init()
{
	localpos = 0;
	localrot = 0;
	localscale = 1;
	m_mMatrix = m_mWorldMatrix = DirectX::XMMatrixIdentity();
}

void Transform::Uninit()
{
}

void Transform::Update()
{
	DirectX::XMMATRIX localMatrix = MyMath::ConvertMatrix(localscale, localrot, localpos);
	m_mWorldMatrix = m_mMatrix * localMatrix;
}
