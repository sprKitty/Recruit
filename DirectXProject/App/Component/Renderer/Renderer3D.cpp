#include <App/Component/Renderer/Renderer3D.h>
#include <App/Component/Object.h>
#include <App/Component/Mesh.h>
#include <App/Component/Transform.h>
#include <App/Component/Instancing.h>
#include <App/RenderPipeline.h>
#include <App/Image.h>
#include <System/Input.h>
#include <System/Geometory.h>


void Renderer3D::Init()
{
	m_pTransform = m_pOwner.lock()->GetComponent<Transform>();
	m_vEmissive = Vector4(0.f, 0.f, 0.f, 1.f);
	RenderPipeline::GetInstance().AddRenderer(weak_from_this());
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

void Renderer3D::Write(const std::weak_ptr<ShaderBuffer>& pBuf, const WriteStep::kind type)
{
	if (!m_pOwner.lock()->IsActive())return;
	if (!m_isWriteStepList[type])return;

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

void Renderer3D::Draw(const std::weak_ptr<ShaderBuffer>& pBuf, const DrawStep::kind type)
{
	if (!m_isDrawStepList[type])return;
	if (!m_pOwner.lock()->IsActive())return;

	pBuf.lock()->BindVS(m_vsType);
	pBuf.lock()->BindPS(m_psType);

	if (!PTRNULLCHECK(m_pMainImage))
	{
		m_pMainImage->Bind(pBuf);
	}

	if (!PTRNULLCHECK(m_pBumpImage))
	{
		m_pBumpImage->Bind(pBuf);
	}

	pBuf.lock()->SetEmissiveColor(m_vEmissive);

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

const bool Renderer3D::CalcFrustumState(const std::weak_ptr<ViewPoint>& pVP)
{
	if (pVP.expired())return false;

	if (!m_pTransform.expired())
	{
		float fRadius = 0;
		DirectX::XMMATRIX mtx = m_pTransform.lock()->GetWorldMatrix();
		DirectX::XMFLOAT4X4 w;
		DirectX::XMStoreFloat4x4(&w, mtx);
		fRadius = (w._11 > fRadius) ? w._11 : fRadius;
		fRadius = (w._22 > fRadius) ? w._22 : fRadius;
		fRadius = (w._33 > fRadius) ? w._33 : fRadius;
		FrustumType::kind type = pVP.lock()->CollisionFrustum(DirectX::XMFLOAT3(w._41, w._42, w._43), fRadius);
		if (type == FrustumType::OUTSIDE)
		{
			return false;
		}
	}
	return true;
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