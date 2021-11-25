#include <App/Camera.h>
#include <System/Input.h>
#include <Shader/ShaderBuffer.h>
#include <Defines.h>
#include <System/DirectX.h>
#include <math.h>

const DirectX::XMFLOAT4 CameraInitPos(0, 15, -20, 0);
const DirectX::XMFLOAT4 CameraInitLook(0, 0, 0, 0);

Camera* Camera::m_pMain;

const float FOV = 45.0f;

Camera::Camera()
	: m_vPos(CameraInitPos)
	, m_vLook(CameraInitLook)
	, m_vUp(0, 1.0f, 0, 0)
	, m_fNearClip(1.0f)
	, m_fFarClip(300.0f)
	, m_isF1(true)
{
	m_vAngle = DirectX::XMFLOAT2(0, 0);
}


Camera::~Camera()
{

}


void Camera::Init(bool isMain)
{
	if (isMain)
		m_pMain = this;
	//{
	//	m_pMain = this;
	//	if (m_pLookObj)
	//	{
	//		m_vPos = DirectX::XMFLOAT4(m_pLookObj->GetTransform().pos.x, m_pLookObj->GetTransform().pos.y + 88, m_pLookObj->GetTransform().pos.z, 0);
	//		m_vLook = DirectX::XMFLOAT4(m_vPos.x, m_vPos.y, m_vPos.z - 5, 0);
	//	}
	//}
	//else
	//{
	//	m_vPos = DirectX::XMFLOAT4(0, 0, 0, 0);
	//	m_vLook = DirectX::XMFLOAT4(0, 0, 10, 0);
	//}
	//m_nZoom = 1;
	//m_fRotate = 0;
	m_vAngle = DirectX::XMFLOAT2(0, 0);
	Blur(true);
}


void Camera::Uninit()
{
}


void Camera::Update()
{
	float mouseMoveX = static_cast<float>(GetMouseMoveX());
	float mouseMoveY = static_cast<float>(GetMouseMoveY());

	DirectX::XMVECTOR vPos = DirectX::XMVectorSet(m_vPos.x, m_vPos.y, m_vPos.z, 0);
	DirectX::XMVECTOR vLook = DirectX::XMVectorSet(m_vLook.x, m_vLook.y, m_vLook.z, 0);
	DirectX::XMVECTOR vFront = DirectX::XMVectorSubtract(vLook, vPos);
	DirectX::XMVECTOR vUp = DirectX::XMVectorSet(m_vUp.x, m_vUp.y, m_vUp.z, 0);

	DirectX::XMVECTOR vSide;
	float focus = 0.0f;
	DirectX::XMStoreFloat(&focus, DirectX::XMVector3Length(vFront));
	vFront = DirectX::XMVector3Normalize(vFront);
	vUp = DirectX::XMVector3Normalize(vUp);
	vSide = DirectX::XMVector3Normalize(DirectX::XMVector3Cross(vUp, vFront));
	vUp = DirectX::XMVector3Normalize(DirectX::XMVector3Cross(vFront, vSide));

	// 左ボタンダブルクリック(初期化
	//if (IsMouseDoubleClick(VK_RBUTTON))
	//{
	//	m_vPos = CameraInitPos;
	//	m_vLook = CameraInitLook;
	//}
	//else if (mouseMoveX != 0 || mouseMoveY != 0)
	//{
	//	// 左ボタン(カメラ回り込み
	//	if (IsKeyPress(VK_RBUTTON))
	//	{
	//		// 回転量
	//		float angleX = 360.f * mouseMoveX / SCREEN_WIDTH;	// 画面一周で360度回転(画面サイズの半分で180度回転)
	//		float angleY = 180.f * mouseMoveY / SCREEN_HEIGHT;	// 画面一周で180度回転(画面サイズの半分で90度回転)

	//		// 回転量をもとに、各軸の回転行列を計算
	//		DirectX::XMVECTOR rotPos = DirectX::XMVectorSubtract(vPos, vLook);
	//		DirectX::XMMATRIX rotY = DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(angleX));
	//		DirectX::XMVECTOR rotAxis = DirectX::XMVector3TransformCoord(vSide, rotY);
	//		DirectX::XMMATRIX rotX = DirectX::XMMatrixRotationAxis(rotAxis, DirectX::XMConvertToRadians(angleY));

	//		// 注視点を原点として回転
	//		rotPos = DirectX::XMVector3TransformCoord(rotPos, rotY);
	//		rotPos = DirectX::XMVector3TransformCoord(rotPos, rotX);
	//		DirectX::XMStoreFloat4(&m_vPos, DirectX::XMVectorAdd(rotPos, vLook));
	//	}
	//	// 中ボタン(カメラ移動
	//	else if (IsKeyPress(VK_MBUTTON))
	//	{
	//		float rate = focus / m_fFarClip;
	//		float aspect = (float)SCREEN_WIDTH / SCREEN_HEIGHT;
	//		// 底辺A、高さBとする三角形について tanΘ = A / Bが成り立つ
	//		// 上記式をもとに割り出した遠景の移動量と、フォーカス位置 / 遠景 の比率から、カメラの移動量を求める
	//		float farMoveX = tanf(DirectX::XM_PI / 6.0f) * m_fFarClip * -mouseMoveX * aspect / (SCREEN_WIDTH * 0.5f);
	//		float farMoveY = tanf(DirectX::XM_PI / 6.0f) * m_fFarClip * mouseMoveY / (SCREEN_HEIGHT * 0.5f);
	//		// 姿勢行列をもとにカメラを移動
	//		DirectX::XMVECTOR vMove = DirectX::XMVectorAdd(DirectX::XMVectorScale(vSide, farMoveX * rate), DirectX::XMVectorScale(vUp, farMoveY * rate));
	//		DirectX::XMStoreFloat4(&m_vPos, DirectX::XMVectorAdd(vPos, vMove));
	//		DirectX::XMStoreFloat4(&m_vLook, DirectX::XMVectorAdd(vLook, vMove));
	//	}
	//}
	// スクロール
	int wheel = GetMouseWheel();
	if (wheel != 0)
	{
		DirectX::XMStoreFloat4(&m_vPos, DirectX::XMVectorAdd(vPos, DirectX::XMVectorScale(vFront, focus * wheel * 0.1f)));
	}
}

void Camera::RotateUpdate()
{
	Blur(true);

	DirectX::XMVECTOR vPos = DirectX::XMVectorSet(m_vPos.x, m_vPos.y, m_vPos.z, 0);
	DirectX::XMVECTOR vLook = DirectX::XMVectorSet(m_vLook.x, m_vLook.y, m_vLook.z, 0);
	DirectX::XMVECTOR vFront = DirectX::XMVectorSubtract(vLook, vPos);
	DirectX::XMVECTOR vUp = DirectX::XMVectorSet(m_vUp.x, m_vUp.y, m_vUp.z, 0);

	DirectX::XMVECTOR vSide;
	float focus = 0.0f;
	DirectX::XMStoreFloat(&focus, DirectX::XMVector3Length(vFront));
	vFront = DirectX::XMVector3Normalize(vFront);
	vUp = DirectX::XMVector3Normalize(vUp);
	vSide = DirectX::XMVector3Normalize(DirectX::XMVector3Cross(vUp, vFront));
	vUp = DirectX::XMVector3Normalize(DirectX::XMVector3Cross(vFront, vSide));

	// 回転量
	float angleX = 0.05f;
	float angleY = 0;

	// 回転量をもとに、各軸の回転行列を計算
	DirectX::XMVECTOR rotPos = DirectX::XMVectorSubtract(vPos, vLook);
	DirectX::XMMATRIX rotY = DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(angleX));
	DirectX::XMVECTOR rotAxis = DirectX::XMVector3TransformCoord(vSide, rotY);
	DirectX::XMMATRIX rotX = DirectX::XMMatrixRotationAxis(rotAxis, DirectX::XMConvertToRadians(angleY));

	// 注視点を原点として回転
	rotPos = DirectX::XMVector3TransformCoord(rotPos, rotY);
	rotPos = DirectX::XMVector3TransformCoord(rotPos, rotX);
	DirectX::XMStoreFloat4(&m_vPos, DirectX::XMVectorAdd(rotPos, vLook));
}


DirectX::XMMATRIX Camera::GetView(Vector3* pUp)
{
	if (pUp)
	{
		return
			DirectX::XMMatrixLookAtLH(
				DirectX::XMVectorSet(m_vPos.x, m_vPos.y, m_vPos.z, 0),
				DirectX::XMVectorSet(m_vLook.x, m_vLook.y, m_vLook.z, 0),
				DirectX::XMVectorSet(0, -1, 0, 0)
			);
	}
	return
		DirectX::XMMatrixLookAtLH(
			DirectX::XMVectorSet(m_vPos.x, m_vPos.y, m_vPos.z, 0),
			DirectX::XMVectorSet(m_vLook.x, m_vLook.y, m_vLook.z, 0),
			DirectX::XMVectorSet(m_vUp.x, m_vUp.y, m_vUp.z, 0)
		);
}


DirectX::XMMATRIX Camera::GetProj()
{
	return
		DirectX::XMMatrixPerspectiveFovLH(
			FOV * DirectX::XM_PI / 180.0f, (float)SCREEN_WIDTH / SCREEN_HEIGHT, m_fNearClip, m_fFarClip
		);
}

inline DirectX::XMMATRIX Camera::GetZoomProj()
{
	return
		DirectX::XMMatrixPerspectiveFovLH(
			FOV / m_nZoom * DirectX::XM_PI / 180.0f, (float)SCREEN_WIDTH / SCREEN_HEIGHT, m_fNearClip, m_fFarClip
		);
}

const Vector3 Camera::GetNormal()
{
	Vector3 N(m_vLook.x - m_vPos.x, m_vLook.y - m_vPos.y, m_vLook.z - m_vPos.z);
	N.Normalize();
	return N;
}


void Camera::Bind3D(bool zoom)
{
	ShaderResource::CameraInfo cam;
	cam.pos = m_vPos;
	ShaderBuffer::GetInstance().SetCameraVP(GetView(), GetProj());
	ShaderBuffer::GetInstance().SetCameraInfo(cam);
	SetCulling(CULL_BACK);
	//SetCulling(CULL_NONE);
}

void Camera::ReverseBind3D()
{
	Vector3 up(0, -1, 0);
	ShaderBuffer::GetInstance().SetCameraVP(GetView(&up), GetProj());
}

void Camera::Blur(bool flg)
{
	ShaderBuffer::GetInstance().SetBlur(flg);
}


void Camera::Bind2D()
{
	DirectX::XMMATRIX mView, mProj;
	mView = DirectX::XMMatrixIdentity();
	mProj = DirectX::XMMatrixOrthographicOffCenterLH(0, SCREEN_WIDTH, SCREEN_HEIGHT, (0), m_fNearClip, m_fFarClip);
	ShaderBuffer::GetInstance().SetCameraVP(mView,mProj);

	SetCulling(CULL_FRONT);
}


void Camera::UpVector(DirectX::XMFLOAT4 & up, DirectX::XMFLOAT4 & right)
{
	DirectX::XMMATRIX proj = GetProj();
	DirectX::XMMATRIX view = GetView();
	DirectX::XMMATRIX vp = view * proj;
	DirectX::XMMATRIX inv = DirectX::XMMatrixInverse(nullptr, vp);
	DirectX::XMFLOAT4X4 v;
	DirectX::XMStoreFloat4x4(&v, inv);
	up = DirectX::XMFLOAT4(v._31, v._32, v._33, v._34);
	right = DirectX::XMFLOAT4(v._11, v._12, v._13, v._14);
}

float Camera::GetViewAngle()
{
	return RAD(FOV);
}

float Camera::GetAspect()
{
	return (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT;
}
