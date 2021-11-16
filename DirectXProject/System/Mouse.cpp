#include "Mouse.h"
#include <Defines.h>

void Mouse::Initialize()
{
}

void Mouse::Finalize()
{
}

void Mouse::Update()
{
}

void Mouse::SetExecuteFunc(const std::shared_ptr<DelegateBase<void, const Vector3&> > pFunc)
{
	m_pFunctionList.push_back(pFunc);
}

Vector3 Mouse::CalcScreenToWorld(float depthZ, DirectX::XMMATRIX mView, DirectX::XMMATRIX mProj)
{
	DirectX::XMFLOAT3 out;
	DirectX::XMFLOAT4X4 wVP;
	DirectX::XMMATRIX mInvView, mInvProj, viewport, InvViewport;
	mInvView = mView;
	mInvProj = mProj;
	mInvView = DirectX::XMMatrixInverse(nullptr, mView);
	mInvProj = DirectX::XMMatrixInverse(nullptr, mProj);
	DirectX::XMStoreFloat4x4(&wVP, DirectX::XMMatrixIdentity());
	wVP._11 = SCREEN_WIDTH * 0.5f;
	wVP._22 = -SCREEN_HEIGHT * 0.5f;
	wVP._41 = SCREEN_WIDTH * 0.5f;
	wVP._42 = SCREEN_HEIGHT * 0.5f;
	viewport = DirectX::XMLoadFloat4x4(&wVP);
	InvViewport = DirectX::XMMatrixInverse(nullptr, viewport);
	DirectX::XMMATRIX tmp = InvViewport * mInvProj * mInvView;
	DirectX::XMVECTOR vec = DirectX::XMVector3TransformCoord(DirectX::XMVectorSet(m_vScreenPos.x, m_vScreenPos.y, depthZ, 1), tmp);
	DirectX::XMStoreFloat3(&out, vec);
	return Vector3().Convert(out);
}

void Mouse::CalcScreentoXZ(DirectX::XMMATRIX mView, DirectX::XMMATRIX mProj)
{
	Vector3 vNear, vFar, vRay;
	vNear = CalcScreenToWorld(0.0f, mView, mProj);
	vFar = CalcScreenToWorld(1.0f, mView, mProj);
	vRay = vFar - vNear;
	vRay.Normalize();

	if (vRay.y <= 0)
	{
		float Lray = vRay.Dot(vRay, Vector3(0, 1, 0));
		vNear *= -1.0f;
		float LP0 = vNear.Dot(vNear, Vector3(0, 1, 0));
		vNear *= -1.0f;
		m_vWorldpos = vNear + vRay * (LP0 / Lray);
		for (const auto itr : m_pFunctionList)
		{
			itr->Execute(m_vWorldpos);		// é¿çs
		}
	}
	else
	{
		m_vWorldpos = vFar;
	}
}
