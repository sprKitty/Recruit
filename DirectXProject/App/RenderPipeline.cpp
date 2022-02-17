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
	CallWrite(WriteStep::LIGHT_DEPTH, pLight);
}

void RenderPipeline::DrawWaterReflection(const std::weak_ptr<Camera> pCamera, const std::weak_ptr<Light> pLight, const std::weak_ptr<ViewPoint> pDOSVP)
{
	if (pCamera.expired())return;
	if (pLight.expired())return;
	if (pDOSVP.expired())return;

	pLight.lock()->Bind3D(m_pShaderBuffer);
	pCamera.lock()->BindRenderTarget();
	pCamera.lock()->Bind3D(m_pShaderBuffer);
	m_pShaderBuffer.lock()->SetTexturePS(pDOSVP.lock()->GetRenderingTexture(0), ShaderResource::TEX_TYPE::DEPTH_OF_SAHDOW);

	CallDraw(DrawStep::WATERRELECTION, pCamera);
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
	m_pShaderBuffer.lock()->SetTexturePS(pDOSVP.lock()->GetRenderingTexture(0), ShaderResource::TEX_TYPE::DEPTH_OF_SAHDOW);
	m_pShaderBuffer.lock()->SetTexturePS(pWaterRefVP.lock()->GetRenderingTexture(0), ShaderResource::TEX_TYPE::WATER);

	CallDraw(DrawStep::WORLD, pCamera);
	CallDraw(DrawStep::WATER, pCamera);
}

void RenderPipeline::PlayGaussianBlurX(const std::weak_ptr<Camera> pCamera, const std::weak_ptr<ViewPoint> pVP, const float fDeviation, const int number)
{
	if (pCamera.expired())return;
	if (pVP.expired())return;

	Vector2 vp = pCamera.lock()->vpSize.get();
	pCamera.lock()->BindRenderTarget();
	pCamera.lock()->Bind2D(m_pShaderBuffer);

	m_pShaderBuffer.lock()->SetTexturePS(pVP.lock()->GetRenderingTexture(number), ShaderResource::TEX_TYPE::EFFECT);
	m_pShaderBuffer.lock()->BindVS(VS_TYPE::GAUSSIANBLUR);
	m_pShaderBuffer.lock()->BindPS(PS_TYPE::GAUSSIANBLUR);
	ShaderResource::PostEffect post;
	float deviation = fDeviation;
	float tu = 1.0f / vp.x;
	float tv = 1.0f / vp.y;
	Vector2 dir = { 1,0 };
	post.kawaseBloom[0].x = 0;
	post.kawaseBloom[0].y = 0;
	post.kawaseBloom[0].z = MyMath::GaussianDistribution(Vector2(0, 0), deviation);
	float totalweight = post.kawaseBloom[0].z;
	for (int i = 1; i < 8; ++i)
	{
		post.kawaseBloom[i].x = dir.x * i * tu;
		post.kawaseBloom[i].y = dir.y * i * tv;
		post.kawaseBloom[i].z = MyMath::GaussianDistribution(dir * static_cast<float>(i), deviation);
		totalweight += post.kawaseBloom[i].z * 2.0f;
	}

	for (int i = 0; i < 8; ++i)
	{
		post.kawaseBloom[i].z /= totalweight;
	}

	m_pShaderBuffer.lock()->SetKawaseBloom(post.kawaseBloom);
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
	
	m_pShaderBuffer.lock()->SetTexturePS(pVP.lock()->GetRenderingTexture(number), ShaderResource::TEX_TYPE::EFFECT);
	m_pShaderBuffer.lock()->BindVS(VS_TYPE::GAUSSIANBLUR);
	m_pShaderBuffer.lock()->BindPS(PS_TYPE::GAUSSIANBLUR);
	ShaderResource::PostEffect post;
	float deviation = fDeviation;
	float tu = 1.0f / vp.x;
	float tv = 1.0f / vp.y;
	Vector2 dir = { 0,1 };
	post.kawaseBloom[0].x = 0;
	post.kawaseBloom[0].y = 0;
	post.kawaseBloom[0].z = MyMath::GaussianDistribution(Vector2(0, 0), deviation);
	float totalweight = post.kawaseBloom[0].z;
	for (int i = 1; i < 8; ++i)
	{
		post.kawaseBloom[i].x = dir.x * i * tu;
		post.kawaseBloom[i].y = dir.y * i * tv;
		post.kawaseBloom[i].z = MyMath::GaussianDistribution(dir * i, deviation);
		totalweight += post.kawaseBloom[i].z * 2.0f;
	}

	for (int i = 0; i < 8; ++i)
	{
		post.kawaseBloom[i].z /= totalweight;
	}

	m_pShaderBuffer.lock()->SetKawaseBloom(post.kawaseBloom);
	DirectX::XMMATRIX mtx = MyMath::ConvertMatrix(Vector3(vp.x, vp.y, 10), Vector3(0, 0, 0), Vector3(vp.x * 0.5f, vp.y * 0.5f, 0));
	m_pShaderBuffer.lock()->SetWorld(mtx);
	Geometory::GetInstance().DrawPolygon();
}

void RenderPipeline::PlayKawaseBloom(const std::weak_ptr<Camera> pCamera, KAWASE_BlOOM_VP pVPList)
{
	for (int i = 0; i < KAWASE_BlOOM_NUM; ++i)
	{
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

	if (!pVPList[0].expired())
	{
		m_pShaderBuffer.lock()->SetTexturePS(pVPList[0].lock()->GetRenderingTexture(1), ShaderResource::TEX_TYPE::MAIN);
	}
	if (!pVPList[1].expired())
	{
		m_pShaderBuffer.lock()->SetTexturePS(pVPList[1].lock()->GetRenderingTexture(0), ShaderResource::TEX_TYPE::KAWASEX_1);
	}
	if (!pVPList[2].expired())
	{
		m_pShaderBuffer.lock()->SetTexturePS(pVPList[2].lock()->GetRenderingTexture(0), ShaderResource::TEX_TYPE::KAWASEY_1);
	}
	if (!pVPList[3].expired())
	{
		m_pShaderBuffer.lock()->SetTexturePS(pVPList[3].lock()->GetRenderingTexture(0), ShaderResource::TEX_TYPE::KAWASEX_2);
	}
	if (!pVPList[4].expired())
	{
		m_pShaderBuffer.lock()->SetTexturePS(pVPList[4].lock()->GetRenderingTexture(0), ShaderResource::TEX_TYPE::KAWASEY_2);
	}
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
	m_pShaderBuffer.lock()->SetTexturePS(pVPDMain.lock()->GetRenderingTexture(0), ShaderResource::TEX_TYPE::MAIN);
	m_pShaderBuffer.lock()->SetTexturePS(pVPDepth.lock()->GetRenderingTexture(1), ShaderResource::TEX_TYPE::DEPTH_OF_FIELD);
	m_pShaderBuffer.lock()->SetTexturePS(pVPBlur.lock()->GetRenderingTexture(0), ShaderResource::TEX_TYPE::BLURX);
	m_pShaderBuffer.lock()->BindVS(VS_TYPE::GAUSSIANBLUR);
	m_pShaderBuffer.lock()->BindPS(PS_TYPE::DOF);
	DirectX::XMMATRIX mtx = MyMath::ConvertMatrix(Vector3(vp.x, vp.y, 10), Vector3(0, 0, 0), Vector3(vp.x * 0.5f, vp.y * 0.5f, 0));
	m_pShaderBuffer.lock()->SetWorld(mtx);
	Geometory::GetInstance().DrawPolygon();
}

void RenderPipeline::DrawUI(const std::weak_ptr<Camera> pCamera)
{
	CallDraw(DrawStep::UI, pCamera);
}

void RenderPipeline::AddRenderer(const std::weak_ptr<Component>& pComponent)
{
	std::weak_ptr<Renderer> pRenderer = std::dynamic_pointer_cast<Renderer>(pComponent.lock());
	if (pRenderer.expired())return;
	m_pDrawList.emplace_back(pRenderer);
}

void RenderPipeline::ReleaseRenderer(const std::weak_ptr<Component>& pComponent)
{
	if (pComponent.expired())return;

	for (weak_ptr_list<Renderer>::iterator itr = m_pDrawList.begin(); itr != m_pDrawList.end(); ++itr)
	{
		if (itr->expired())continue;

		if (itr->lock() == pComponent.lock())
		{
			itr = m_pDrawList.erase(itr);
		}

		if (itr == m_pDrawList.end())break;
	}
}

void RenderPipeline::CallWrite(const WriteStep::kind type, const std::weak_ptr<ViewPoint>& pVP)
{
	for (const auto& itr : m_pDrawList)
	{
		if (itr.lock()->m_pOwner.lock()->IsActive())
		{
			itr.lock()->Write(m_pShaderBuffer, type);
		}
	}
}

void RenderPipeline::CallDraw(const DrawStep::kind type, const std::weak_ptr<ViewPoint>& pVP)
{
	for (const auto& itr : m_pDrawList)
	{
		if (itr.lock()->m_pOwner.lock()->IsActive())
		{                      
			if (itr.lock()->CalcFrustumState(pVP))
			{
				itr.lock()->Draw(m_pShaderBuffer, type);
			}
		}
	}
}