#include "Transform.h"


void Transform::Init()
{
	localpos = 0;
	localrot = 0;
	localscale = 1;
	m_mMatrix = DirectX::XMMatrixIdentity();
}

void Transform::Uninit()
{
}

void Transform::Update()
{
}

const DirectX::XMMATRIX Transform::GetWorldMatrix(const Vector3& vScale, const Vector3& vAngle, const Vector3& vPos)
{
	DirectX::XMMATRIX localMatrix = MyMath::ConvertMatrix(localscale * vScale, localrot + vAngle, localpos + vPos);
	return m_mMatrix * localMatrix;
}
