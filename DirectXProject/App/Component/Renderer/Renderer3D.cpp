#include <App/Component/Renderer/Renderer3D.h>
#include <App/Component/Object.h>
#include <App/Component/Mesh.h>
#include <App/Component/Transform.h>
#include <App/Component/Image.h>
#include <App/Component/Instancing.h>
#include <App/RenderPipeline.h>
#include <System/Input.h>
#include <System/Geometory.h>


void Renderer3D::Init()
{
	m_pTransform = m_pOwner.lock()->GetComponent<Transform>();
	RenderPipeline::GetInstance().AddRenderer(weak_from_this());
	m_isWriteType.resize(WriteType::MAX);
	for (bool itr : m_isWriteType)
	{
		itr = false;
	}
	
	m_isDrawType.resize(DrawType::MAX);
	for (bool itr : m_isDrawType)
	{
		itr = false;
	}
	m_isWriteType[WriteType::DEPTH_OF_FIELD] = true;

}

void Renderer3D::Uninit()
{
	RenderPipeline::GetInstance().ReleaseRenderer(weak_from_this());
}

void Renderer3D::Update()
{
	if (m_pInstancing.expired())
	{
		m_pInstancing = m_pOwner.lock()->GetComponent<Instancing>();
		if (m_pMesh.expired())
		{
			m_pMesh = m_pOwner.lock()->GetComponent<Mesh>();
		}
	}
	else
	{
		m_pMesh.reset();
	}
}

void Renderer3D::Write(const std::weak_ptr<ShaderBuffer> pBuf, const WriteType::kind type)
{
	if (!m_isWriteType[type])return;

	if (!PTRNULLCHECK(m_pMainImage))
	{
		m_pMainImage->Bind(pBuf);
	}

	if (m_pInstancing.expired())
	{
		if (!m_pMesh.expired())
		{
			pBuf.lock()->SetWorld(m_pTransform.lock()->GetWorldMatrix());
			m_pMesh.lock()->Bind();
		}
	}
	else
	{
		m_pInstancing.lock()->Bind(pBuf);
	}
}

void Renderer3D::Draw(const std::weak_ptr<ShaderBuffer> pBuf, const DrawType::kind type)
{
	if (!m_isDrawType[type])return;

	switch (type)
	{
	case DrawType::WORLD_OF_NORMAL:
		pBuf.lock()->BindVS(VS_TYPE::NORMAL);
		pBuf.lock()->BindPS(PS_TYPE::NORMAL);
		break;
	case DrawType::WORLD_OF_TRIPLANAR:
		pBuf.lock()->BindVS(VS_TYPE::TRIPLANAR);
		pBuf.lock()->BindPS(PS_TYPE::TRIPLANAR);
		break;
	case DrawType::WORLD_OF_CHARACTER:
		pBuf.lock()->BindVS(VS_TYPE::NORMAL);
		pBuf.lock()->BindPS(PS_TYPE::CHARACTER);
		break;
	case DrawType::WORLD_OF_EFFECT:
		pBuf.lock()->BindVS(VS_TYPE::NORMAL);
		pBuf.lock()->BindPS(PS_TYPE::EFFECT);
		break;
	case DrawType::WORLD_OF_WATER:
		pBuf.lock()->BindVS(VS_TYPE::WATERREFLECTION);
		pBuf.lock()->BindPS(PS_TYPE::WATERREFLECTION);
		break;
	default:
		break;
	}

	if (!PTRNULLCHECK(m_pMainImage))
	{
		m_pMainImage->Bind(pBuf);
	}

	if (!PTRNULLCHECK(m_pBumpImage))
	{
		m_pBumpImage->Bind(pBuf);
	}

	if (m_pInstancing.expired())
	{
		if (!m_pMesh.expired())
		{
			pBuf.lock()->SetWorld(m_pTransform.lock()->GetWorldMatrix());
			m_pMesh.lock()->Bind();
		}
	}
	else
	{
		m_pInstancing.lock()->Bind(pBuf);
	}
}

void Renderer3D::SetMainImage(const std::string str)
{
	m_pMainImage.reset(new Image());
	m_pMainImage->SetTexture(str.c_str());
}

void Renderer3D::SetBumpImage(const std::string str)
{
	m_pBumpImage.reset(new Image());
	m_pBumpImage->SetTexture(str.c_str());
	m_pBumpImage->SetTexType(ShaderResource::TEX_TYPE::BUMP);
}