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
	m_vPos = CameraInitPos;
	m_vLook = CameraInitLook;
	m_vUp = { 0.0f,1.0f,0.0f };
	m_vFront = m_vLook - m_vPos;
	m_vFront.Normalize();
	m_vSide = { 1.0f,0.0f,0.0f };
	m_vScreenSize = Vector2(SCREEN_WIDTH, SCREEN_HEIGHT);
	m_fNearClip = 1.0f;
	m_fFarClip = 500.0f;
	m_fFov = FOV;
	CalcView();
	CalcProjection();
	CalcWorldMatrix();
	CreateViewFrustum();
	UpdateViewFrustum();
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
	vPos = DirectX::XMLoadFloat3(&m_vPos.Convert());
	vLook = DirectX::XMLoadFloat3(&m_vLook.Convert());

	DirectX::XMVECTOR vFront = DirectX::XMVectorSubtract(vLook, vPos);
	DirectX::XMVECTOR vUp = DirectX::XMLoadFloat3(&m_vUp.Convert());

	DirectX::XMVECTOR vSide;
	float focus = 0.0f;
	DirectX::XMStoreFloat(&focus, DirectX::XMVector3Length(vFront));
	vFront = DirectX::XMVector3Normalize(vFront);
	vUp = DirectX::XMVector3Normalize(vUp);
	vSide = DirectX::XMVector3Normalize(DirectX::XMVector3Cross(vUp, vFront));
	vUp = DirectX::XMVector3Normalize(DirectX::XMVector3Cross(vFront, vSide));

	DirectX::XMFLOAT3 value;
	DirectX::XMStoreFloat3(&value, vPos);
	m_vPos.Convert(value);
	DirectX::XMStoreFloat3(&value, vLook);
	m_vLook.Convert(value);
	DirectX::XMStoreFloat3(&value, vUp);
	m_vUp.Convert(value);
	DirectX::XMStoreFloat3(&value, vSide);
	m_vSide.Convert(value);
	DirectX::XMStoreFloat3(&value, vFront);
	m_vFront.Convert(value);

	CalcView();
	CalcProjection();
	CalcWorldMatrix();
	CreateViewFrustum();
	UpdateViewFrustum();
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
	m_vPos = m_vLook + m_vNormal * m_fDisatance;
}
