#include <App/Light.h>
#include <Shader/ShaderBuffer.h>
#include <math.h>
#include <System/Input.h>
#include <Defines.h>
#include <MyMath.h>


const Vector3 LightInitPos(40, 55, -60);
//const DirectX::XMFLOAT4 LightInitPos(400, 550,-600, 0);
const Vector3 LightInitLook(0, 0, 0);

const float FOV = 45.0f;

Light::Light()
{
	color = Property<Vector4>(&m_vColor);
	position = Property<Vector3>(&m_vPos);
	look = Property<Vector3>(&m_vLook);
	vpSize = Property<Vector2>(&m_vScreenSize);
	parallelScale = Property<Vector2>(&m_vParallelScale);
	nearclip = Property<float>(&m_fNearClip);
	farclip = Property<float>(&m_fFarClip);
	fov = Property<float>(&m_fFov);
}


Light::~Light()
{

}


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
	m_fNearClip = 10.0f;
	m_fFarClip = 1000.0f;
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
		DirectX::XMVectorSet(m_vPos.x, m_vPos.y, m_vPos.z, 0),
		DirectX::XMVectorSet(m_vLook.x, m_vLook.y, m_vLook.z, 0),
		DirectX::XMVectorSet(0, 1, 0, 0)
	);
}


void Light::CalcProjection()
{
	if (m_isPerspective)
	{
		m_mProjection = DirectX::XMMatrixPerspectiveFovLH(
			DirectX::XMConvertToRadians(m_fFov), m_vScreenSize.x / m_vScreenSize.y, m_fNearClip, m_fFarClip
		);
	}
	else
	{
		Vector2 halfSize = m_vScreenSize / 2 / m_vParallelScale;
		m_mProjection = DirectX::XMMatrixOrthographicOffCenterLH(
			-halfSize.x, halfSize.x, -halfSize.y, halfSize.y, m_fNearClip, m_fFarClip
		);
	}
}

void Light::Bind3D()
{
	CalcProjection();
	ShaderResource::LightInfo info;
	info.pos = { m_vPos.x, m_vPos.y, m_vPos.z, 0.0f };
	info.color = m_vColor.Convert();
	info.direction = { m_vFront.x,m_vFront.y,m_vFront.z,0.0f };
	ShaderBuffer::GetInstance().SetLightVP(m_mView, m_mProjection);
	ShaderBuffer::GetInstance().SetLightInfo(info);
}