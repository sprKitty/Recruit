#include "RenderPipeline.h"
#include <App/Component/Object.h>
#include <Shader/ShaderBuffer.h>
#include <System/Geometory.h>


void RenderPipeline::Initialize()
{
}

void RenderPipeline::Finalize()
{
}

void RenderPipeline::ReleaseRenderer(const std::weak_ptr<Component>& pComponent)
{
	if(pComponent.expired())return;

	for (RendererPtrList::iterator itr = m_pDrawList.begin(); itr != m_pDrawList.end();)
	{
		if (itr->expired())continue;

		if (itr->lock() == pComponent.lock())
		{
			itr = m_pDrawList.erase(itr);
		}

		if (itr == m_pDrawList.end())break;

		++itr;
	}
}

void RenderPipeline::DrawShadowMap(const std::weak_ptr<Light> pLight)
{
	if (pLight.expired())
	{
		return;
	}
	else
	{
		pLight.lock()->BindRenderTarget();
		pLight.lock()->Bind3D(m_pShaderBuffer);
	}
	m_pShaderBuffer.lock()->BindVS(VS_TYPE::LIGHTDEPTH);
	m_pShaderBuffer.lock()->BindPS(PS_TYPE::LIGHTDEPTH);
	Call(WriteType::DEPTH_OF_SHADOW, DrawType::MAX);

}

void RenderPipeline::DrawCameraDepth(const std::weak_ptr<Camera> pCamera)
{
	if (pCamera.expired())
	{
		return;
	}
	else
	{
		pCamera.lock()->BindRenderTarget();
		pCamera.lock()->Bind3D(m_pShaderBuffer);
	}
	m_pShaderBuffer.lock()->BindVS(VS_TYPE::CAMERADEPTH);
	m_pShaderBuffer.lock()->BindPS(PS_TYPE::CAMERADEPTH);
	Call(WriteType::DEPTH_OF_FIELD, DrawType::MAX);
}

void RenderPipeline::PlayGaussianBlurX(const std::weak_ptr<Camera> pCamera, const std::weak_ptr<ViewPoint> pVP, const float fDeviation, const int number)
{
	if (pCamera.expired())return;
	if (pVP.expired())return;

	Vector2 vp = pCamera.lock()->vpSize.get();
	pCamera.lock()->BindRenderTarget();
	pCamera.lock()->Bind2D(m_pShaderBuffer);

	m_pShaderBuffer.lock()->SetTexture(pVP.lock()->GetRenderingTexture(number), ShaderResource::TEX_TYPE::DEPTH_OF_FIELD);
	m_pShaderBuffer.lock()->BindVS(VS_TYPE::GAUSSIANBLUR);
	m_pShaderBuffer.lock()->BindPS(PS_TYPE::GAUSSIANBLUR);
	ShaderResource::PostEffect post;
	float deviation = fDeviation;
	float tu = 1.5f / vp.x;
	float tv = 1.5f / vp.y;
	Vector2 dir = { 1,0 };
	post.blur[0].x = 0;
	post.blur[0].y = 0;
	post.blur[0].z = MyMath::GaussianDistribution(Vector2(0, 0), deviation);
	float totalweight = post.blur[0].z;
	for (int i = 1; i < 8; ++i)
	{
		post.blur[i].x = dir.x * i * tu;
		post.blur[i].y = dir.y * i * tv;
		post.blur[i].z = MyMath::GaussianDistribution(dir * i, deviation);
		totalweight += post.blur[i].z * 2.0f;
	}

	for (int i = 0; i < 8; ++i)
	{
		post.blur[i].z /= totalweight;
	}

	m_pShaderBuffer.lock()->SetPostEffectInfo(post);
	DirectX::XMMATRIX mtx = MyMath::ConvertMatrix(Vector3(vp.x, vp.y, 10), Vector3(0, 0, 0), Vector3(vp.x * 0.5f, vp.y * 0.5f, 0));
	m_pShaderBuffer.lock()->SetWorld(mtx);
	Geometory::GetInstance().DrawPolygon();
}

void RenderPipeline::PlayGaussianBlurY(const std::weak_ptr<Camera> pCamera, const std::weak_ptr<ViewPoint> pVP, const float fDeviation, const int number)
{
	if (pCamera.expired())return;
	if (pVP.expired())return;

	Vector2 vp = pCamera.lock()->vpSize.get();
	pCamera.lock()->BindRenderTarget();
	pCamera.lock()->Bind2D(m_pShaderBuffer);
	
	m_pShaderBuffer.lock()->SetTexture(pVP.lock()->GetRenderingTexture(number), ShaderResource::TEX_TYPE::DEPTH_OF_FIELD);
	m_pShaderBuffer.lock()->BindVS(VS_TYPE::GAUSSIANBLUR);
	m_pShaderBuffer.lock()->BindPS(PS_TYPE::GAUSSIANBLUR);
	ShaderResource::PostEffect post;
	float deviation = fDeviation;
	float tu = 1.5f / vp.x;
	float tv = 1.5f / vp.y;
	Vector2 dir = { 0,1 };
	post.blur[0].x = 0;
	post.blur[0].y = 0;
	post.blur[0].z = MyMath::GaussianDistribution(Vector2(0, 0), deviation);
	float totalweight = post.blur[0].z;
	for (int i = 1; i < 8; ++i)
	{
		post.blur[i].x = dir.x * i * tu;
		post.blur[i].y = dir.y * i * tv;
		post.blur[i].z = MyMath::GaussianDistribution(dir * i, deviation);
		totalweight += post.blur[i].z * 2.0f;
	}

	for (int i = 0; i < 8; ++i)
	{
		post.blur[i].z /= totalweight;
	}

	m_pShaderBuffer.lock()->SetPostEffectInfo(post);
	DirectX::XMMATRIX mtx = MyMath::ConvertMatrix(Vector3(vp.x, vp.y, 10), Vector3(0, 0, 0), Vector3(vp.x * 0.5f, vp.y * 0.5f, 0));
	m_pShaderBuffer.lock()->SetWorld(mtx);
	Geometory::GetInstance().DrawPolygon();
}

void RenderPipeline::PlayKawaseBloom(const std::weak_ptr<Camera> pCamera, KAWASE_BlOOM_VP pVPList)
{
	for (int i = 0; i < KAWASE_BlOOM_NUM; ++i)
	{
		if (pVPList[i].expired())return;
	}
	if (pCamera.expired())return;

	Vector2 vp = pCamera.lock()->vpSize.get();
	pCamera.lock()->BindRenderTarget();
	pCamera.lock()->Bind2D(m_pShaderBuffer);

	
	m_pShaderBuffer.lock()->BindVS(VS_TYPE::GAUSSIANBLUR);
	m_pShaderBuffer.lock()->BindPS(PS_TYPE::KAWASEBLOOM);
	DirectX11::GetInstance().SetBlendMode(BlendMode::BLEND_ADD);

	DirectX::XMMATRIX mtx = MyMath::ConvertMatrix(Vector3(vp.x, vp.y, 10), Vector3(0, 0, 0), Vector3(vp.x * 0.5f, vp.y * 0.5f, 0));
	m_pShaderBuffer.lock()->SetWorld(mtx);

	m_pShaderBuffer.lock()->SetTexture(pVPList[0].lock()->GetRenderingTexture(0), ShaderResource::TEX_TYPE::MAIN);
	m_pShaderBuffer.lock()->SetTexture(pVPList[1].lock()->GetRenderingTexture(0), ShaderResource::TEX_TYPE::KAWASEX_1);
	m_pShaderBuffer.lock()->SetTexture(pVPList[2].lock()->GetRenderingTexture(0), ShaderResource::TEX_TYPE::KAWASEY_1);
	m_pShaderBuffer.lock()->SetTexture(pVPList[3].lock()->GetRenderingTexture(0), ShaderResource::TEX_TYPE::KAWASEX_2);
	m_pShaderBuffer.lock()->SetTexture(pVPList[4].lock()->GetRenderingTexture(0), ShaderResource::TEX_TYPE::KAWASEY_2);
	Geometory::GetInstance().DrawPolygon();

	DirectX11::GetInstance().SetBlendMode(BlendMode::BLEND_ALPHA);
}

void RenderPipeline::PlayMixTexture(const std::weak_ptr<Camera> pCamera, const std::weak_ptr<ViewPoint> pVPMix1, const std::weak_ptr<ViewPoint> pVPMix2)
{
	if (pCamera.expired())return;
	if (pVPMix1.expired())return;
	if (pVPMix2.expired())return;

	Vector2 vp = pCamera.lock()->vpSize.get();
	pCamera.lock()->BindRenderTarget();
	pCamera.lock()->Bind2D(m_pShaderBuffer);
	DirectX11::GetInstance().SetBlendMode(BlendMode::BLEND_ADD);
	m_pShaderBuffer.lock()->SetTexture(pVPMix1.lock()->GetRenderingTexture(0), ShaderResource::TEX_TYPE::BLURX);
	m_pShaderBuffer.lock()->SetTexture(pVPMix2.lock()->GetRenderingTexture(0), ShaderResource::TEX_TYPE::BLURY);
	m_pShaderBuffer.lock()->BindVS(VS_TYPE::GAUSSIANBLUR);
	m_pShaderBuffer.lock()->BindPS(PS_TYPE::MIX);
	DirectX::XMMATRIX mtx = MyMath::ConvertMatrix(Vector3(vp.x, vp.y, 10), Vector3(0, 0, 0), Vector3(vp.x * 0.5f, vp.y * 0.5f, 0));
	m_pShaderBuffer.lock()->SetWorld(mtx);
	Geometory::GetInstance().DrawPolygon();
	DirectX11::GetInstance().SetBlendMode(BlendMode::BLEND_ALPHA);
}

void RenderPipeline::DrawDOFTexture(const std::weak_ptr<Camera> pCamera, const std::weak_ptr<ViewPoint> pVPDMain, const std::weak_ptr<ViewPoint> pVPDepth, const std::weak_ptr<ViewPoint> pVPBlur)
{
	if (pCamera.expired())return;
	if (pVPDMain.expired())return;
	if (pVPDepth.expired())return;
	if (pVPBlur.expired())return;

	Vector2 vp = pCamera.lock()->vpSize.get();
	pCamera.lock()->BindRenderTarget();
	pCamera.lock()->Bind2D(m_pShaderBuffer);
	m_pShaderBuffer.lock()->SetTexture(pVPDMain.lock()->GetRenderingTexture(0), ShaderResource::TEX_TYPE::MAIN);
	m_pShaderBuffer.lock()->SetTexture(pVPDepth.lock()->GetRenderingTexture(1), ShaderResource::TEX_TYPE::DEPTH_OF_FIELD);
	m_pShaderBuffer.lock()->SetTexture(pVPBlur.lock()->GetRenderingTexture(0), ShaderResource::TEX_TYPE::BLURX);
	m_pShaderBuffer.lock()->BindVS(VS_TYPE::GAUSSIANBLUR);
	m_pShaderBuffer.lock()->BindPS(PS_TYPE::DOF);
	DirectX::XMMATRIX mtx = MyMath::ConvertMatrix(Vector3(vp.x, vp.y, 10), Vector3(0, 0, 0), Vector3(vp.x * 0.5f, vp.y * 0.5f, 0));
	m_pShaderBuffer.lock()->SetWorld(mtx);
	Geometory::GetInstance().DrawPolygon();
}

void RenderPipeline::DrawWaterReflection(const std::weak_ptr<Camera> pCamera, const std::weak_ptr<Light> pLight, const std::weak_ptr<ViewPoint> pDOSVP)
{
	if (pCamera.expired())return;
	if (pLight.expired())return;
	if (pDOSVP.expired())return;

	pLight.lock()->Bind3D(m_pShaderBuffer);
	pCamera.lock()->BindRenderTarget();
	pCamera.lock()->Bind3D(m_pShaderBuffer);
	m_pShaderBuffer.lock()->SetTexture(pDOSVP.lock()->GetRenderingTexture(0), ShaderResource::TEX_TYPE::DEPTH_OF_SAHDOW);

	Call(WriteType::MAX, DrawType::WORLD_OF_TRIPLANAR);
	Call(WriteType::MAX, DrawType::WORLD_OF_CHARACTER);
}

void RenderPipeline::Draw(const std::weak_ptr<Camera> pCamera, const std::weak_ptr<Light> pLight, const std::weak_ptr<ViewPoint> pDOSVP, const std::weak_ptr<ViewPoint> pWaterRefVP)
{
	if (pCamera.expired())return;
	if (pLight.expired())return;
	if (pDOSVP.expired())return;
	if (pWaterRefVP.expired())return;

	pLight.lock()->Bind3D(m_pShaderBuffer);
	pCamera.lock()->BindRenderTarget();
	pWaterRefVP.lock()->Bind3D(m_pShaderBuffer, 1);
	pCamera.lock()->Bind3D(m_pShaderBuffer);
	m_pShaderBuffer.lock()->SetTexture(pDOSVP.lock()->GetRenderingTexture(0), ShaderResource::TEX_TYPE::DEPTH_OF_SAHDOW);
	m_pShaderBuffer.lock()->SetTexture(pWaterRefVP.lock()->GetRenderingTexture(0), ShaderResource::TEX_TYPE::WATER);

	Call(WriteType::MAX, DrawType::WORLD_OF_TRIPLANAR);
	Call(WriteType::MAX, DrawType::WORLD_OF_CHARACTER);
	Call(WriteType::MAX, DrawType::WORLD_OF_WATER);
}

void RenderPipeline::DrawEffect(const std::weak_ptr<Camera> pCamera)
{
	if (pCamera.expired())return;
	pCamera.lock()->BindRenderTarget();
	pCamera.lock()->Bind3D(m_pShaderBuffer);

	Call(WriteType::MAX, DrawType::WORLD_OF_EFFECT);
}

void RenderPipeline::AddRenderer(const std::weak_ptr<Component>& pComponent)
{
	std::weak_ptr<Renderer> pRenderer = std::dynamic_pointer_cast<Renderer>(pComponent.lock());
	m_pDrawList.push_back(pRenderer);
}

void RenderPipeline::Call(WriteType::kind typeW, DrawType::kind typeD)
{
	if (typeW == WriteType::MAX)
	{
		for (const auto& itr : m_pDrawList)
		{
			if (itr.lock()->m_pOwner.lock()->IsActive())
			{
				itr.lock()->Draw(m_pShaderBuffer, typeD);
			}
		}
	}
	else if(typeD == DrawType::MAX)
	{
		for (const auto& itr : m_pDrawList)
		{
			if (itr.lock()->m_pOwner.lock()->IsActive())
			{
				itr.lock()->Write(m_pShaderBuffer, typeW);
			}
		}
	}
}
