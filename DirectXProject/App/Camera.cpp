#include <App/Camera.h>
#include <App/Component/Transform.h>
#include <System/Input.h>
#include <System/Clocker.h>
#include <Shader/ShaderBuffer.h>
#include <Defines.h>
#include <System/DirectX.h>
#include <math.h>

const Vector3 CameraInitPos(0, 20, -25);
const Vector3 CameraInitLook(0, 0, 2);

const float FOV = 30.0f;

void Camera::Init()
{
	m_vPos = m_vLatePos = CameraInitPos;
	m_vLook = m_vLateLook = CameraInitLook;
	m_vUp = { 0.0f,1.0f,0.0f };
	m_vFront = m_vLook - m_vPos;
	m_vFront.Normalize();
	m_vSide = { 1.0f,0.0f,0.0f };
	m_vScreenSize = Vector2(SCREEN_WIDTH, SCREEN_HEIGHT);
	m_fNearClip = 1.0f;
	m_fFarClip = 80.0f;
	m_fFov = FOV;
	m_isLate = false;
	CalcView();
	CalcProjection();
}


void Camera::Uninit()
{
}


void Camera::Update()
{
	float mouseMoveX = static_cast<float>(GetMouseMoveX());
	float mouseMoveY = static_cast<float>(GetMouseMoveY());

	if (!m_pTargetTransform.expired())
	{
		Vector3 vDistance = m_vPos - m_vLook;
		m_vLook = m_pTargetTransform.lock()->localpos + CameraInitLook;
		m_vPos = m_vLook + vDistance;
		if (m_isLate)
		{
			Vector3 moveLook = m_vLook - m_vLateLook;
			Vector3 movePos = m_vPos - m_vLatePos;
			moveLook *= Clocker::GetInstance().GetFrameTime() * 2.0f;
			movePos *= Clocker::GetInstance().GetFrameTime() * 2.0f;
			m_vLateLook = moveLook + m_vLateLook;
			m_vLatePos = movePos + m_vLatePos;
		}
	}

	DirectX::XMVECTOR vPos, vLook;
	if (m_isLate)
	{
		vPos = DirectX::XMLoadFloat3(&m_vLatePos.Convert());
		vLook = DirectX::XMLoadFloat3(&m_vLateLook.Convert());
	}
	else
	{
		vPos = DirectX::XMLoadFloat3(&m_vPos.Convert());
		vLook = DirectX::XMLoadFloat3(&m_vLook.Convert());
	}
	DirectX::XMVECTOR vFront = DirectX::XMVectorSubtract(vLook, vPos);
	DirectX::XMVECTOR vUp = DirectX::XMLoadFloat3(&m_vUp.Convert());

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
	if (m_isLate)
	{
		DirectX::XMStoreFloat3(&value, vPos);
		m_vLatePos.Convert(value);
		DirectX::XMStoreFloat3(&value, vLook);
		m_vLateLook.Convert(value);
	}
	else
	{
		DirectX::XMStoreFloat3(&value, vPos);
		m_vPos.Convert(value);
		DirectX::XMStoreFloat3(&value, vLook);
		m_vLook.Convert(value);
	}
	DirectX::XMStoreFloat3(&value, vUp);
	m_vUp.Convert(value);
	DirectX::XMStoreFloat3(&value, vSide);
	m_vSide.Convert(value);
	DirectX::XMStoreFloat3(&value, vFront);
	m_vFront.Convert(value);


	CalcView();
	CalcProjection();
}

void Camera::Bind3D(const std::weak_ptr<ShaderBuffer> pBuf, const int nBufferNum)
{
	ShaderResource::CameraInfo cam;
	cam.pos = { m_vPos.x, m_vPos.y, m_vPos.z, 0.0f };
	pBuf.lock()->SetCameraVP(m_mView, m_mProjection, nBufferNum);
	pBuf.lock()->SetCameraInfo(cam);
	DirectX11::GetInstance().SetCulling(CullingMode::CULL_BACK);
}

void Camera::Bind2D(const std::weak_ptr<ShaderBuffer> pBuf)
{
	DirectX::XMMATRIX mView, mProj;
	ShaderResource::CameraInfo cam;
	cam.pos = { m_vPos.x, m_vPos.y, m_vPos.z, 0.0f };
	cam.vp = { m_vScreenSize.x,m_vScreenSize.y,0,0 };
	mView = DirectX::XMMatrixIdentity();
	mProj = DirectX::XMMatrixOrthographicOffCenterLH(0, m_vScreenSize.x, m_vScreenSize.y, 0, m_fNearClip, m_fFarClip);
	pBuf.lock()->SetCameraVP(mView, mProj, 0);
	pBuf.lock()->SetCameraInfo(cam);
	DirectX11::GetInstance().SetCulling(CullingMode::CULL_NONE);
}