#include "RenderPipeline.h"
#include <App/Component/Object.h>
#include <App/MultiPass.h>
#include <App/Camera.h>
#include <App/Light.h>
#include <Shader/ShaderBuffer.h>


void RenderPipeline::Initialize()
{
	for (int i = 0; i < WriteType::MAX; ++i)
	{
		std::shared_ptr<MultiPass> pMP(new MultiPass());
		m_pMultiPassList.push_back(std::move(pMP));
	}
	const float fSize = 256 * 2 * 2 * 2;
	m_pMultiPassList[WriteType::DEPTH_OF_FIELD]->Create(0, Vector2(SCREEN_WIDTH, SCREEN_HEIGHT), 1, DXGI_FORMAT_R32G32B32A32_FLOAT);
	m_pMultiPassList[WriteType::DEPTH_OF_SHADOW]->Create(0, Vector2(fSize, fSize), 1, DXGI_FORMAT_R32G32B32A32_FLOAT);
}

void RenderPipeline::Finalize()
{
	m_pMultiPassList.clear();
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

	
	switch (typeW)
	{
	case WriteType::DEPTH_OF_FIELD:
		ShaderBuffer::GetInstance().BindVS(VS_TYPE::DOF);
		ShaderBuffer::GetInstance().BindPS(PS_TYPE::DOF);
		break;

	case WriteType::DEPTH_OF_SHADOW:
		m_pRentLight.lock()->vpSize.set(m_pMultiPassList[typeW]->GetVPSize());
		ShaderBuffer::GetInstance().BindVS(VS_TYPE::LIGHTDEPTH);
		ShaderBuffer::GetInstance().BindPS(PS_TYPE::LIGHTDEPTH);
		break;

	default:
		break;
	}
	m_pRentCamera.lock()->Bind3D();
	m_pRentLight.lock()->Bind3D();
	m_pMultiPassList[typeW]->Bind();
	Call(typeW, DrawType::MAX);
}

void RenderPipeline::Draw(const DrawType::kind & typeD)
{
	switch (typeD)
	{
	case DrawType::WORLD_OF_NORMAL:
		m_pRentCamera.lock()->Bind3D();
		ShaderBuffer::GetInstance().BindVS(VS_TYPE::NORMAL);
		ShaderBuffer::GetInstance().BindPS(PS_TYPE::NORMAL);
		break;

	case DrawType::WORLD_OF_EFFECT:
		m_pRentCamera.lock()->Bind3D();
		ShaderBuffer::GetInstance().BindVS(VS_TYPE::NORMAL);
		ShaderBuffer::GetInstance().BindPS(PS_TYPE::EFFECT);
		break;

	case DrawType::WORLD_OF_TRIPLANAR:
		m_pRentLight.lock()->Bind3D();
		m_pRentCamera.lock()->Bind3D();
		ShaderBuffer::GetInstance().SetTexture(GetRenderTex(WriteType::DEPTH_OF_SHADOW), ShaderResource::TEX_TYPE::DEPTH_OF_SAHDOW);
		ShaderBuffer::GetInstance().BindVS(VS_TYPE::TRIPLANAR);
		ShaderBuffer::GetInstance().BindPS(PS_TYPE::TRIPLANAR);
		break;

	case DrawType::WORLD_OF_SHADOW:
		m_pRentCamera.lock()->Bind3D();

		break;

	case DrawType::WORLD_OF_CHARACTER:
		m_pRentCamera.lock()->Bind3D();
		ShaderBuffer::GetInstance().BindVS(VS_TYPE::NORMAL);
		ShaderBuffer::GetInstance().BindPS(PS_TYPE::CHARACTER);
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

void RenderPipeline::AddRenderer(const std::weak_ptr<Component>& pComponent)
{
	std::weak_ptr<Renderer> pRenderer = std::dynamic_pointer_cast<Renderer>(pComponent.lock());
	m_pDrawList.push_back(pRenderer);
}

ID3D11ShaderResourceView * RenderPipeline::GetRenderTex(const WriteType::kind type, const int num)
{
	return m_pMultiPassList[type]->GetRenderTex();
}

void RenderPipeline::Call(WriteType::kind typeW, DrawType::kind typeD)
{
	if (typeW == WriteType::MAX)
	{
		for (const auto& itr : m_pDrawList)
		{
			if (itr.lock()->m_pOwner.lock()->IsActive())
			{
				itr.lock()->Draw(typeD);
			}
		}
	}
	else if(typeD == DrawType::MAX)
	{
		for (const auto& itr : m_pDrawList)
		{
			if (itr.lock()->m_pOwner.lock()->IsActive())
			{
				itr.lock()->Write(typeW);
			}
		}
	}
}
