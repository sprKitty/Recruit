#include <App/Camera.h>
#include <App/Component/Transform.h>
#include <System/Input.h>
#include <System/Clocker.h>
#include <Shader/ShaderBuffer.h>
#include <Defines.h>
#include <System/DirectX.h>
#include <math.h>

const Vector3 CameraInitPos(0, 28, -21);
const Vector3 CameraInitLook(0, 0, 0);

const float FOV = 30.0f;

Camera::Camera()
{
}


Camera::~Camera()
{

}


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
	m_fFarClip = 100.0f;
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

	if (!m_pTargetTrans.expired())
	{
		Vector3 vDistance = m_vPos - m_vLook;
		m_vLook = m_pTargetTrans.lock()->localpos;
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

void Camera::SetTarget(const std::weak_ptr<Transform>& pTrans)
{
	m_pTargetTrans = pTrans;
}

void Camera::CalcView()
{
	if (m_isLate)
	{
		m_mView = DirectX::XMMatrixLookAtLH(
			DirectX::XMVectorSet(m_vLatePos.x, m_vLatePos.y, m_vLatePos.z, 0),
			DirectX::XMVectorSet(m_vLateLook.x, m_vLateLook.y, m_vLateLook.z, 0),
			DirectX::XMVectorSet(0, 1, 0, 0)
		);
	}
	else
	{
		m_mView = DirectX::XMMatrixLookAtLH(
			DirectX::XMVectorSet(m_vPos.x, m_vPos.y, m_vPos.z, 0),
			DirectX::XMVectorSet(m_vLook.x, m_vLook.y, m_vLook.z, 0),
			DirectX::XMVectorSet(0, 1, 0, 0)
		);
	}
}


void Camera::CalcProjection()
{
	m_mProjection = DirectX::XMMatrixPerspectiveFovLH(
		DirectX::XMConvertToRadians(m_fFov), m_vScreenSize.x / m_vScreenSize.y, m_fNearClip, m_fFarClip
	);
}

void Camera::Bind3D()
{
	ShaderResource::CameraInfo cam;
	cam.pos = { m_vPos.x, m_vPos.y, m_vPos.z, 0.0f };
	ShaderBuffer::GetInstance().SetCameraVP(m_mView, m_mProjection);
	ShaderBuffer::GetInstance().SetCameraInfo(cam);
	DirectX11::GetInstance().SetCulling(CullingMode::CULL_BACK);
}

void Camera::Bind2D()
{
	DirectX::XMMATRIX mView, mProj;
	mView = DirectX::XMMatrixIdentity();
	mProj = DirectX::XMMatrixOrthographicOffCenterLH(0, m_vScreenSize.x, m_vScreenSize.y, (0), m_fNearClip, m_fFarClip);
	ShaderBuffer::GetInstance().SetCameraVP(mView,mProj);
	DirectX11::GetInstance().SetCulling(CullingMode::CULL_NONE);
}