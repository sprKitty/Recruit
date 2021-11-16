#include "RenderPipeline.h"
#include <System/RenderTarget.h>
#include <Shader/ShaderBuffer.h>
#include <App/Camera.h>

void RenderPipeline::Initialize()
{
	for (int i = 0; i < WriteType::MAX; ++i)
	{
		std::shared_ptr<RenderTarget> pRT(new RenderTarget());
		m_pRenderTargets.push_back(std::move(pRT));
	}
	m_pRenderTargets[WriteType::DEPTH_OF_SHADOW]->Create(1, SCREEN_WIDTH, SCREEN_HEIGHT, DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_FORMAT_R8G8B8A8_UNORM);
	m_pRenderTargets[WriteType::DEPTH_OF_FIELD]->Create(1, SCREEN_WIDTH, SCREEN_HEIGHT, DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_FORMAT_R8G8B8A8_UNORM);
}

void RenderPipeline::Finalize()
{
	m_pRenderTargets.clear();
}

void RenderPipeline::ReleaseRenderer(const std::weak_ptr<Component>& pRenderer3D)
{
	if(pRenderer3D.expired())return;

	for (std::vector<RENDERER_PTR>::iterator itr = m_pDraws.begin(); itr != m_pDraws.end();)
	{
		if (itr->expired())continue;

		if (itr->lock() == pRenderer3D.lock())
		{
			itr = m_pDraws.erase(itr);
		}

		if (itr == m_pDraws.end())break;

		++itr;
	}

}

void RenderPipeline::Write(const WriteType::kind & typeW)
{
	if (m_pRentCamera.expired())
	{
		return;
	}
	if (m_pRentLight.expired())
	{
		return;
	}

	
	m_pRenderTargets[typeW]->Draw(Vector4(1), GetContext(), GetDepthStencil(), true);
	switch (typeW)
	{
	case WriteType::DEPTH_OF_FIELD:
		m_pRentCamera.lock()->Bind3D(false);
		ShaderBuffer::GetInstance().BindVS(VS_TYPE::CAMERADEPTH);
		ShaderBuffer::GetInstance().BindPS(PS_TYPE::CAMERADEPTH);
		break;

	case WriteType::DEPTH_OF_SHADOW:
		m_pRentCamera.lock()->Bind3D(false);
		ShaderBuffer::GetInstance().BindVS(VS_TYPE::LIGHTDEPTH);
		ShaderBuffer::GetInstance().BindPS(PS_TYPE::LIGHTDEPTH);
		break;

	default:
		break;
	}
	Call(typeW, DrawType::MAX);
}

void RenderPipeline::Draw(const DrawType::kind & typeD)
{
	switch (typeD)
	{
	case DrawType::WORLD_OF_NORMAL:
		m_pRentCamera.lock()->Bind3D(false);
		ShaderBuffer::GetInstance().BindVS(VS_TYPE::NORMAL);
		ShaderBuffer::GetInstance().BindPS(PS_TYPE::NORMAL);

		break;

	case DrawType::WORLD_OF_SHADOW:
		m_pRentCamera.lock()->Bind3D(false);

		break;

	case DrawType::WORLD_OF_TOON:
		m_pRentCamera.lock()->Bind3D(false);
		break;

	case DrawType::UI:
		m_pRentCamera.lock()->Bind2D();
		ShaderBuffer::GetInstance().BindVS(VS_TYPE::NORMAL);
		ShaderBuffer::GetInstance().BindPS(PS_TYPE::NORMAL);
		break;
	default:
		break;
	}
	Call(WriteType::MAX, typeD);
}

void RenderPipeline::Call(WriteType::kind typeW, DrawType::kind typeD)
{
	if (typeW == WriteType::MAX)
	{
		for (const auto& itr : m_pDraws)
		{
			itr.lock()->Draw(typeD);
		}
	}
	else if(typeD == DrawType::MAX)
	{
		for (const auto& itr : m_pDraws)
		{
			itr.lock()->Write(typeW);
		}
	}
}
