#include "BillBoardRenderer.h"
#include <App/Component/Object.h>
#include <App/Component/Mesh.h>
#include <App/Component/Transform.h>
#include <App/Fade/FadeBase.h>
#include <App/TexAnimation.h>
#include <App/ViewPoint/Camera.h>
#include <App/RenderPipeline.h>
#include <System/DebugLog.h>


void BillBoardRenderer::Init()
{
	m_pTransform = m_pOwner.lock()->GetComponent<Transform>();
	m_isXaxisLock = false;
	m_isYaxisLock = false;
	m_isZaxisLock = true;
	RenderPipeline::GetInstance().AddRenderer(weak_from_this());
}

void BillBoardRenderer::Uninit()
{
	RenderPipeline::GetInstance().ReleaseRenderer(weak_from_this());
}

void BillBoardRenderer::Update()
{
	if (m_pMesh.expired())
	{
		m_pMesh = m_pOwner.lock()->GetComponent<Mesh>();
	}
	CalcBillBoard();
}

void BillBoardRenderer::Write(const std::weak_ptr<ShaderBuffer>& pBuf, const WriteStep::kind type)
{
	if (!m_pOwner.lock()->IsActive())return;
	if (!m_isWriteStepList[type])return;

	if (!m_pMainTexAnim.expired())
	{
		m_pMainTexAnim.lock()->Bind(pBuf);
	}
	else
	{
		m_Image.Bind(pBuf);
	}

	if (!m_pTransform.expired())
	{
		pBuf.lock()->SetWorld(m_pTransform.lock()->GetWorldMatrix());
	}

	if (!m_pMesh.expired())
	{
		m_pMesh.lock()->Bind();
	}
}

void BillBoardRenderer::Draw(const std::weak_ptr<ShaderBuffer>& pBuf, const DrawStep::kind type)
{
	if (!m_isDrawStepList[type])return;
	if (!m_pOwner.lock()->IsActive())return;

	CalcBillBoard();

	pBuf.lock()->BindVS(m_vsType);
	pBuf.lock()->BindPS(m_psType);

	//pBuf.lock()->SetEmissiveColor(1);
	if (!m_pFadeAnimation.expired())
	{
		m_pFadeAnimation.lock()->Bind(pBuf);
	}
	else
	{
		pBuf.lock()->SetTexturePS(nullptr, ShaderResource::TEX_TYPE::GRAYSCALE);
	}

	if (!m_pBumpTexAnim.expired())
	{
		m_pBumpTexAnim.lock()->Bind(pBuf);
	}
	else
	{
		pBuf.lock()->SetTexturePS(nullptr, ShaderResource::TEX_TYPE::BUMP);
	}

	if (!m_pMainTexAnim.expired())
	{
		m_pMainTexAnim.lock()->Bind(pBuf);
	}
	else
	{
		m_Image.Bind(pBuf);
	}

	if (!m_pTransform.expired())
	{
		pBuf.lock()->SetWorld(m_pTransform.lock()->GetWorldMatrix());
	}

	if(!m_pMesh.expired())
	{
		m_pMesh.lock()->Bind();
	}
}

const bool BillBoardRenderer::CalcFrustumState(const std::weak_ptr<ViewPoint>& pVP)
{
	return true;
	//if (pVP.expired())return;

	//CalcBillBoard();
	//
	//if (!m_pTransform.expired())
	//{
	//	float fRadius = 0;
	//	DirectX::XMMATRIX mtx = m_pTransform.lock()->GetWorldMatrix();
	//	DirectX::XMFLOAT4X4 w;
	//	DirectX::XMStoreFloat4x4(&w, mtx);
	//	fRadius = (w._11 > fRadius) ? w._11 : fRadius;
	//	fRadius = (w._22 > fRadius) ? w._22 : fRadius;
	//	fRadius = (w._33 > fRadius) ? w._33 : fRadius;
	//	//m_frustumType = pVP.lock()->CollisionViewFrustum(DirectX::XMFLOAT3(w._41, w._42, w._43), fRadius);
	//}
}

void BillBoardRenderer::CalcBillBoard()
{
	if (m_pCamera.expired())return;
	DirectX::XMMATRIX mInvCam = DirectX::XMMatrixInverse(nullptr, m_pCamera.lock()->view.get());
	DirectX::XMFLOAT4X4 vW;
	DirectX::XMStoreFloat4x4(&vW, mInvCam);
	vW._41 = 0.0f;
	vW._42 = 0.0f;
	vW._43 = 0.0f;

	if (m_isXaxisLock)
	{
		vW._11 = 1.0f;
		vW._12 = 0.0f;
		vW._13 = 0.0f;
	}

	if (m_isYaxisLock)
	{
		vW._21 = 0.0f;
		vW._22 = 1.0f;
		vW._23 = 0.0f;
	}

	if (m_isZaxisLock)
	{
		vW._31 = 0.0f;
		vW._32 = 0.0f;
		vW._33 = 1.0f;
	}

	
	mInvCam = DirectX::XMLoadFloat4x4(&vW);
	if (!m_pTransform.expired())
	{
		m_pTransform.lock()->SetMatrix(mInvCam);
	}
	else
	{
		m_pTransform = m_pOwner.lock()->GetComponent<Transform>();
	}
}
