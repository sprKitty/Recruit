#include <App/ViewPoint/LateCamera.h>
#include <App/Component/Transform.h>
#include <System/Input.h>
#include <System/Clocker.h>
#include <Shader/ShaderBuffer.h>
#include <Defines.h>
#include <System/DirectX.h>
#include <math.h>


void LateCamera::Init()
{
	m_vLatePos = CameraInitPos;
	m_vLatePos = CameraInitLook;
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
}


void LateCamera::Uninit()
{
}


void LateCamera::Update()
{
	float mouseMoveX = static_cast<float>(GetMouseMoveX());
	float mouseMoveY = static_cast<float>(GetMouseMoveY());

	if (!targetTransform.get().expired())
	{
		Vector3 vDistance = position.get() - look.get();
		look.set(targetTransform.get().lock()->localpos + CameraInitLook);
		position.set(look.get() + vDistance);
		Vector3 moveLook = look.get() - m_vLateLook;
		Vector3 movePos = position.get() - m_vLatePos;
		moveLook *= Clocker::GetInstance().DeltaTime() * 2.0f;
		movePos *= Clocker::GetInstance().DeltaTime() * 2.0f;
		m_vLateLook = moveLook + m_vLateLook;
		m_vLatePos = movePos + m_vLatePos;
	}

	DirectX::XMVECTOR vPos, vLook;
	vPos = DirectX::XMLoadFloat3(&m_vLatePos.Convert());
	vLook = DirectX::XMLoadFloat3(&m_vLateLook.Convert());
	DirectX::XMVECTOR vFront = DirectX::XMVectorSubtract(vLook, vPos);
	DirectX::XMVECTOR vUp = DirectX::XMLoadFloat3(&Vector3(up.get()).Convert());

	DirectX::XMVECTOR vSide;
	float focus = 0.0f;
	DirectX::XMStoreFloat(&focus, DirectX::XMVector3Length(vFront));
	vFront = DirectX::XMVector3Normalize(vFront);
	vUp = DirectX::XMVector3Normalize(vUp);
	vSide = DirectX::XMVector3Normalize(DirectX::XMVector3Cross(vUp, vFront));
	vUp = DirectX::XMVector3Normalize(DirectX::XMVector3Cross(vFront, vSide));

	// ÉXÉNÉçÅ[Éã
	int wheel = GetMouseWheel();
	if (wheel != 0)
	{
		vPos = DirectX::XMVectorAdd(vPos, DirectX::XMVectorScale(vFront, focus * wheel * 0.1f));
	}

	DirectX::XMFLOAT3 value;
	DirectX::XMStoreFloat3(&value, vPos);
	m_vLatePos = value;
	DirectX::XMStoreFloat3(&value, vLook);
	m_vLateLook = value;
	DirectX::XMStoreFloat3(&value, vUp);
	up.set(value);
	DirectX::XMStoreFloat3(&value, vSide);
	side.set(value);
	DirectX::XMStoreFloat3(&value, vFront);
	front.set(value);


	CalcView();
	CalcProjection();
	CalcWorldMatrix();
	CreateViewFrustum();
	UpdateViewFrustum();
}