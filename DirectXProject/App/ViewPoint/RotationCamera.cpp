#include "RotationCamera.h"
#include <App/Component/Transform.h>
#include <System/Input.h>
#include <System/Clocker.h>
#include <Shader/ShaderBuffer.h>
#include <Defines.h>
#include <System/DirectX.h>
#include <math.h>


void RotationCamera::Init()
{
	position.set(CameraInitPos);
	look.set(CameraInitLook);
	up.set({ 0.0f,1.0f,0.0f });
	Vector3 vFront = CameraInitLook - CameraInitPos;
	vFront.Normalize();
	front.set(vFront);
	side.set({ 1.0f,0.0f,0.0f });
	vpSize.set(Vector2(SCREEN_WIDTH, SCREEN_HEIGHT));
	nearclip.set(1.f);
	farclip.set(500.f);
	fov.set(FOV);
	CalcView();
	CalcProjection();
	CalcWorldMatrix();
	//CreateViewFrustum();
	//UpdateViewFrustum();
	m_fDisatance = 1.5f;
	m_fDegeree = 180.0f;
	m_vNormal = 0.0f;
}

void RotationCamera::Uninit()
{
}

void RotationCamera::Update()
{
	UpdateRotation();

	DirectX::XMVECTOR vPos, vLook;
	vPos = DirectX::XMLoadFloat3(&Vector3(position.get()).Convert());
	vLook = DirectX::XMLoadFloat3(&Vector3(look.get()).Convert());

	DirectX::XMVECTOR vFront = DirectX::XMVectorSubtract(vLook, vPos);
	DirectX::XMVECTOR vUp = DirectX::XMLoadFloat3(&Vector3(up.get()).Convert());

	DirectX::XMVECTOR vSide;
	float focus = 0.0f;
	DirectX::XMStoreFloat(&focus, DirectX::XMVector3Length(vFront));
	vFront = DirectX::XMVector3Normalize(vFront);
	vUp = DirectX::XMVector3Normalize(vUp);
	vSide = DirectX::XMVector3Normalize(DirectX::XMVector3Cross(vUp, vFront));
	vUp = DirectX::XMVector3Normalize(DirectX::XMVector3Cross(vFront, vSide));

	DirectX::XMFLOAT3 value;
	DirectX::XMStoreFloat3(&value, vPos);
	position.set(value);
	DirectX::XMStoreFloat3(&value, vLook);
	look.set(value);
	DirectX::XMStoreFloat3(&value, vUp);
	up.set(value);
	DirectX::XMStoreFloat3(&value, vSide);
	side.set(value);
	DirectX::XMStoreFloat3(&value, vFront);
	front.set(value);

	CalcView();
	CalcProjection();
	CalcWorldMatrix();
	//CreateViewFrustum();
	//UpdateViewFrustum();
}

void RotationCamera::UpdateRotation()
{
	m_fDegeree -= 0.1f;
	if (m_fDegeree > 180.0f)
	{
		m_fDegeree = -180.0f;
	}
	else if (m_fDegeree < -180.0f)
	{
		m_fDegeree = 180.0f;
	}

	m_vNormal = { sinf(DirectX::XMConvertToRadians(m_fDegeree)),0.6f,cosf(DirectX::XMConvertToRadians(m_fDegeree)) };
	position.set(look.get() + m_vNormal * m_fDisatance);
}
