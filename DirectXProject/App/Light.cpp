#include <App/Light.h>
#include <Shader/ShaderBuffer.h>
#include <math.h>
#include <System/Input.h>
#include <Defines.h>
#include <MyMath.h>


const DirectX::XMFLOAT4 LightInitPos(400, 400,-450, 0);
const DirectX::XMFLOAT4 LightInitLook(0, 0, 0, 0);

const float FOV = 5.0f;

Light::Light()
{
}


Light::~Light()
{

}


void Light::Init()
{
	m_pos = LightInitPos;
	m_look = LightInitLook;
	m_up = DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 0.0f);
	Vector3 vDir = Vector3(m_look.x - m_pos.x, m_look.y - m_pos.y, m_look.z - m_pos.z).Normalize();
	m_direction = DirectX::XMFLOAT4(vDir.x, vDir.y, vDir.z, 0.0f);
	m_right = DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 0.0f);
	m_vScreenSize = Vector2(SCREEN_WIDTH, SCREEN_HEIGHT);
	m_fNearClip = 10.0f;
	m_fFarClip = 1000.0f;
	m_fFov = FOV;
	CalcView();
	CalcProjection();
}


void Light::Uninit()
{
}


void Light::Update()
{
	//DirectX::XMVECTOR vPos = DirectX::XMVectorSet(m_pos.x, m_pos.y, m_pos.z, 0);
	//DirectX::XMVECTOR vLook = DirectX::XMVectorSet(m_look.x, m_look.y, m_look.z, 0);
	//DirectX::XMVECTOR vFront = DirectX::XMVectorSubtract(vLook, vPos);
	//DirectX::XMVECTOR vUp = DirectX::XMVectorSet(m_up.x, m_up.y, m_up.z, 0);

	//DirectX::XMVECTOR vSide;
	//float focus = 0.0f;
	//DirectX::XMStoreFloat(&focus, DirectX::XMVector3Length(vFront));
	//vFront = DirectX::XMVector3Normalize(vFront);
	//vUp = DirectX::XMVector3Normalize(vUp);
	//vSide = DirectX::XMVector3Normalize(DirectX::XMVector3Cross(vUp, vFront));
	//vUp = DirectX::XMVector3Normalize(DirectX::XMVector3Cross(vFront, vSide));

	//DirectX::XMStoreFloat4(&m_pos, vPos);
	//DirectX::XMStoreFloat4(&m_look, vLook);
	//DirectX::XMStoreFloat4(&m_up, vUp);
	//DirectX::XMStoreFloat4(&m_right, vSide);
	//DirectX::XMStoreFloat4(&m_direction, vFront);
	//CalcView();
	//CalcProjection();
}

void Light::CalcView()
{
	m_mView = DirectX::XMMatrixLookAtLH(
		DirectX::XMVectorSet(m_pos.x, m_pos.y, m_pos.z, 0),
		DirectX::XMVectorSet(m_look.x, m_look.y, m_look.z, 0),
		DirectX::XMVectorSet(m_up.x, m_up.y, m_up.z, 0)
	);
}


void Light::CalcProjection()
{
	m_mProjection = DirectX::XMMatrixPerspectiveFovLH(
		DirectX::XMConvertToRadians(m_fFov), m_vScreenSize.x / m_vScreenSize.y, m_fNearClip, m_fFarClip
	);
}

void Light::Bind3D()
{
	CalcProjection();
	ShaderResource::LightInfo info;
	info.pos = m_pos;
	info.direction = m_direction;
	ShaderBuffer::GetInstance().SetLightVP(m_mView, m_mProjection);
	ShaderBuffer::GetInstance().SetLightInfo(info);
}


//int g_asd = 0;
//constexpr float NEARCLIP = 0.1f;
//constexpr float FARCLIP = 500;
//Light::Light()
//	:m_vPos(Vector4(0, 100, -50, 0))
//	, m_vLook(Vector4(0, 0, 0, 0))
//	, m_vColor(Vector4(1.0f, 0.6f, 0.0f, 1))
//	, m_vDirection(Vector4(0.0f, -0.7f, 0.8f, 0))
//	, m_fDistance(0)
//	, m_rot(0)
//	, m_vDecay(0)
//	, m_isSpot(false)
//	, m_isOn(false)
//	, m_vUp(Vector4(0, 1, 0, 0))
//	, m_vDebugValue(Vector4(3.0f, 0.0f, 0.0f, -0.0005f))
//	, m_fNearClip(NEARCLIP)
//	, m_fFarClip(FARCLIP)
//	, m_fUVOffset(0)
//{
//	m_fDistance = 0;
//
//}
//
//Light::~Light()
//{
//}
//
//void Light::Init(Vector3* pPos, Vector3* pDirection, Vector2* pNearFar, Vector2 debug, float uvoffset,float fov, float multi)
//{
//	if (pPos)
//		m_vPos = Vector4(pPos->x, pPos->y, pPos->z, 0);
//	m_rotation.x = m_vPos.x;
//	m_rotation.y = m_vPos.z;
//	if (pDirection)
//		m_vDirection = Vector4(pDirection->x, pDirection->y, pDirection->z, 0);
//	if (pNearFar)
//	{
//		m_fNearClip = pNearFar->x;
//		m_fFarClip = pNearFar->y;
//	}
//	m_vDebugValue.x = debug.x;
//	m_vDebugValue.y = debug.y;
//	m_fUVOffset = uvoffset;
//	m_fFOV = fov;
//	m_fMultipray = multi;
//}
//
//void Light::Uninit()
//{
//}
//
//void Light::Update()
//{
//	m_vPos.x = m_rotation.x * sinf(DirectX::XMConvertToRadians(g_asd)) + m_vLook.x;
//	m_vPos.z = m_rotation.y * cosf(DirectX::XMConvertToRadians(g_asd)) + m_vLook.z;
//
//	if (g_asd >= 360)
//		g_asd -= 360;
//	if (IsKeyPress('M'))
//		g_asd++;
//}
//
//void Light::Bind()
//{
//	ShaderResource::LightInfo light;
//	light.pos = DirectX::XMFLOAT4(m_vPos.x, m_vPos.y, m_vPos.z, 0);
//	light.decay = DirectX::XMFLOAT4(m_vDecay.x, m_vDecay.y, m_vDecay.z, 0);
//	light.direction = DirectX::XMFLOAT3(m_vDirection.x, m_vDirection.y, m_vDirection.z);
//	light.fSoft = m_fUVOffset;
//	light.color = DirectX::XMFLOAT4(m_vColor.x, m_vColor.y, m_vColor.z, m_vColor.w);
//	ShaderBuffer::GetInstance().SetParallelLightInfo(light);
//
//	ShaderBuffer::GetInstance().SetParallelLightVP(GetView(), GetProj());
//}
//
//void Light::UpdateValue(int num)
//{
//	ShaderResource::LightInfo light;
//	light.pos = DirectX::XMFLOAT4(m_vPos.x, m_vPos.y, m_vPos.z, 0);
//	light.decay = DirectX::XMFLOAT4(m_vDecay.x, m_vDecay.y, m_vDecay.z, 0);
//	light.direction = DirectX::XMFLOAT3(m_vDirection.x, m_vDirection.y, m_vDirection.z);
//	light.fSoft = m_fUVOffset;
//	light.color = DirectX::XMFLOAT4(m_vColor.x, m_vColor.y, m_vColor.z, m_vColor.w);
//	ShaderBuffer::GetInstance().SetParallelLightInfo(light);
//}
//
//DirectX::XMMATRIX Light::GetView()
//{
//	return
//		DirectX::XMMatrixLookAtLH(
//			DirectX::XMVectorSet(m_vPos.x, m_vPos.y, m_vPos.z, 0),
//			DirectX::XMVectorSet(m_vLook.x, m_vLook.y, m_vLook.z, 0),
//			DirectX::XMVectorSet(m_vUp.x, m_vUp.y, m_vUp.z, 0)
//		);
//}
//
//DirectX::XMMATRIX Light::GetProj()
//{
//	return
//		DirectX::XMMatrixPerspectiveFovLH(
//			m_fFOV * DirectX::XM_PI / 180.0f, (float)SCREEN_WIDTH * m_fMultipray / SCREEN_HEIGHT * m_fMultipray, m_fNearClip, m_fFarClip
//		);
//}
