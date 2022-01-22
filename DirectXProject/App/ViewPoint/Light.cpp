#include <App/ViewPoint/Light.h>
#include <Shader/ShaderBuffer.h>
#include <math.h>
#include <System/Input.h>
#include <Defines.h>
#include <MyMath.h>


const Vector3 LightInitPos(40, 55, -60);
const Vector3 LightInitLook(0, 0, 0);

const float FOV = 45.0f;

void Light::Init()
{
	m_vPos = LightInitPos;
	m_vLook = LightInitLook;
	m_vUp = { 0.0f,1.0f,0.0f };
	m_vFront = m_vLook - m_vPos;
	m_vFront.Normalize();
	m_vSide = { 1.0f,0.0f,0.0f };
	m_vScreenSize = Vector2(SCREEN_WIDTH, SCREEN_HEIGHT);
	m_vParallelScale = 1;
	m_fNearClip = 1.0f;
	m_fFarClip = 500.0f;
	m_fFov = FOV;
	m_isPerspective = true;
	m_vColor = 1;
	CalcView();
	CalcProjection();
}


void Light::Uninit()
{
}


void Light::Update()
{
	CalcView();
	CalcProjection();
	CalcWorldMatrix();
	CreateViewFrustum();
	UpdateViewFrustum();
}

void Light::Bind3D(const std::weak_ptr<ShaderBuffer> pBuf, const int nBufferNum)
{
	ShaderResource::LightInfo info;
	info.pos = { m_vPos.x, m_vPos.y, m_vPos.z, 0.0f };
	info.color = m_vColor.Convert();
	info.direction = { m_vFront.x,m_vFront.y,m_vFront.z,0.0f };
	pBuf.lock()->SetLightVP(m_mView, m_mProjection, nBufferNum);
	pBuf.lock()->SetLightInfo(info);
}