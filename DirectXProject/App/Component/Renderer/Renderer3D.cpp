#include <App/Component/Renderer/Renderer3D.h>
#include <App/Component/Object.h>
#include <App/Component/Mesh.h>
#include <App/Component/Transform.h>
#include <App/Component/Image.h>
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
}

void Renderer3D::Uninit()
{
	RenderPipeline::GetInstance().ReleaseRenderer(weak_from_this());
}

void Renderer3D::Update()
{
	if (m_pMesh.expired())
	{
		m_pMesh = m_pOwner.lock()->GetComponent<Mesh>();
	}
}

void Renderer3D::Write(const WriteType::kind type)
{
	if (!m_isWriteType[type])return;

	if (!PTRNULLCHECK(m_pMainImage))
	{
		m_pMainImage->Bind();
	}

	ShaderBuffer::GetInstance().SetWorld(m_pTransform.lock()->GetWorldMatrix());

	if (!m_pMesh.expired())
	{
		m_pMesh.lock()->Bind();
	}
}

void Renderer3D::Draw(const DrawType::kind type)
{
	if (!m_isDrawType[type])return;

	if (!PTRNULLCHECK(m_pMainImage))
	{
		m_pMainImage->Bind();
	}

	if (!PTRNULLCHECK(m_pBumpImage))
	{
		m_pBumpImage->Bind();
	}

	ShaderBuffer::GetInstance().SetWorld(m_pTransform.lock()->GetWorldMatrix());

	if (!m_pMesh.expired())
	{
		m_pMesh.lock()->Bind();
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