#include <App/ViewPoint/Camera.h>
#include <App/Component/Transform.h>
#include <System/Input.h>
#include <System/Clocker.h>
#include <Shader/ShaderBuffer.h>
#include <Defines.h>
#include <System/DirectX.h>
#include <math.h>

const Vector3 Camera::CameraInitPos(0, 20, -25);
const Vector3 Camera::CameraInitLook(0, 0, 2);

const float Camera::FOV = 30.0f;

void Camera::Init()
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
}


void Camera::Uninit()
{
}


void Camera::Update()
{
	float mouseMoveX = static_cast<float>(GetMouseMoveX());
	float mouseMoveY = static_cast<float>(GetMouseMoveY());

	if (!targetTransform.get().expired())
	{
		Vector3 vDistance = position.get() - look.get();
		look.set(targetTransform.get().lock()->localpos + CameraInitLook);
		position.set(look.get() + vDistance);
	}

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

	// ÉXÉNÉçÅ[Éã
	int wheel = GetMouseWheel();
	if (wheel != 0)
	{
		vPos = DirectX::XMVectorAdd(vPos, DirectX::XMVectorScale(vFront, focus * wheel * 0.1f));
	}

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
	CreateViewFrustum();
	UpdateViewFrustum();
}

void Camera::Bind3D(const std::weak_ptr<ShaderBuffer> pBuf, const int nBufferNum)
{
	Vector3 vPos = position.get();
	Vector3 vFront = front.get();

	ShaderResource::CameraInfo cam;
	cam.pos = { vPos.x, vPos.y, vPos.z, 0.0f };
	cam.dir = { vFront.x, vFront.y, vFront.z, 0.0f };
	pBuf.lock()->SetCameraVP(view.get(), projection.get(), nBufferNum);
	pBuf.lock()->SetCameraInfo(cam);
	DirectX11::GetInstance().SetCulling(CullingMode::CULL_BACK);
}

void Camera::Bind2D(const std::weak_ptr<ShaderBuffer> pBuf)
{
	Vector3 vPos = position.get();
	Vector2 vp = vpSize.get();

	DirectX::XMMATRIX mView, mProj;
	ShaderResource::CameraInfo cam;
	cam.pos = { vPos.x, vPos.y, vPos.z, 0.0f };
	cam.vp = { vp.x,vp.y,0,0 };
	mView = DirectX::XMMatrixIdentity();
	mProj = DirectX::XMMatrixOrthographicOffCenterLH(0, vp.x, vp.y, 0, nearclip.get(), farclip.get());
	pBuf.lock()->SetCameraVP(mView, mProj, 0);
	pBuf.lock()->SetCameraInfo(cam);
	DirectX11::GetInstance().SetCulling(CullingMode::CULL_NONE);
}