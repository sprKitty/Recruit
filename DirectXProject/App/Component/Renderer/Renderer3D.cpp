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
	m_vEmissive = Vector4(0.f, 0.f, 0.f, 1.f);
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

void Renderer3D::Write(const std::weak_ptr<ShaderBuffer>& pBuf, const WriteType::kind type)
{
	if (!m_isWriteType[type])return;
	if (m_frustumType == FrustumType::OUTSIDE)return;

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

void Renderer3D::Draw(const std::weak_ptr<ShaderBuffer>& pBuf, const DrawType::kind type)
{
	if ((type == DrawType::UI_NORMAL) || (type == DrawType::UI_MAGIC))return;
	if (m_frustumType == FrustumType::OUTSIDE)return;
	if (type != DrawType::MAX)
	{
		if (!m_isDrawType[type])return;
	}

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
	case DrawType::WORLD_OF_GRASS:
		pBuf.lock()->BindVS(VS_TYPE::GRASSMOVE);
		pBuf.lock()->BindPS(PS_TYPE::CHARACTER);
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

void Renderer3D::CalcFrustumState(const std::weak_ptr<ViewPoint>& pVP)
{
	if (pVP.expired())return;

	if (!m_pTransform.expired())
	{
		float fRadius = 0;
		DirectX::XMMATRIX mtx = m_pTransform.lock()->GetWorldMatrix();
		DirectX::XMFLOAT4X4 w;
		DirectX::XMStoreFloat4x4(&w, mtx);
		fRadius = (w._11 > fRadius) ? w._11 : fRadius;
		fRadius = (w._22 > fRadius) ? w._22 : fRadius;
		fRadius = (w._33 > fRadius) ? w._33 : fRadius;
		w._42 += (fRadius * 0.5f);
		//m_frustumType = pVP.lock()->CollisionViewFrustum(DirectX::XMFLOAT3(w._41, w._42, w._43), fRadius);
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