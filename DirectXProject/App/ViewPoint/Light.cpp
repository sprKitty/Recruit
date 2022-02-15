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
	position.set(LightInitPos);
	look.set(LightInitLook);
	up.set({ 0.0f,1.0f,0.0f });
	Vector3 vFront = LightInitLook - LightInitPos;
	vFront.Normalize();
	front.set(vFront);
	side.set({ 1.0f,0.0f,0.0f });
	vpSize.set(Vector2(SCREEN_WIDTH, SCREEN_HEIGHT));
	parallelScale.set(1.f);
	nearclip.set(1.f);
	farclip.set(500.f);
	fov.set(FOV);
	perspective.set(true);
	color.set(1.f);
	jaggiesReduction.set(0.f);
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
	//CreateViewFrustum();
	//UpdateViewFrustum();
}

void Light::Bind3D(const std::weak_ptr<ShaderBuffer> pBuf, const int nBufferNum)
{
	Vector3 vPos = position.get();
	Vector4 vColor = color.get();
	Vector3 vFront = front.get();

	ShaderResource::LightInfo info;
	info.pos = { vPos.x, vPos.y, vPos.z, jaggiesReduction.get() };
	info.color = vColor.Convert();
	info.direction = { vFront.x, vFront.y, vFront.z, 0.f };
	pBuf.lock()->SetLightVP(view.get(), projection.get(), nBufferNum);
	pBuf.lock()->SetLightInfo(info);
}