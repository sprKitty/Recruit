#include "BillBoardRenderer.h"
#include <App/Component/Object.h>
#include <App/Component/Mesh.h>
#include <App/Component/Transform.h>
#include <App/TexAnimation.h>
#include <App/Camera.h>
#include <App/RenderPipeline.h>
#include <System/DebugLog.h>


void BillBoardRenderer::Init()
{
	m_pTransform = m_pOwner.lock()->GetComponent<Transform>();
	m_isXaxisLock = false;
	m_isYaxisLock = false;
	m_isZaxisLock = true;
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

void BillBoardRenderer::Write(const WriteType::kind type)
{
	if (!m_isWriteType[type])return;

	if (!m_pMainTexAnim.expired())
	{
		m_pMainTexAnim.lock()->Bind();
	}

	if (!m_pTransform.expired())
	{
		ShaderBuffer::GetInstance().SetWorld(m_pTransform.lock()->GetWorldMatrix());
	}

	if (!m_pMesh.expired())
	{
		m_pMesh.lock()->Bind();
	}
}

void BillBoardRenderer::Draw(const DrawType::kind type)
{
	if (!m_isDrawType[type])
	{
		return;
	}

	if (!m_pMainTexAnim.expired())
	{
		m_pMainTexAnim.lock()->Bind();
	}
	
	if (!m_pBumpTexAnim.expired())
	{
		m_pBumpTexAnim.lock()->Bind();
	}

	if (!m_pTransform.expired())
	{
		ShaderBuffer::GetInstance().SetWorld(m_pTransform.lock()->GetWorldMatrix());
	}

	if(!m_pMesh.expired())
	{
		m_pMesh.lock()->Bind();
	}
}

void BillBoardRenderer::CalcBillBoard()
{
	if (m_pCamera.expired())return;
	DirectX::XMMATRIX mInvCam = DirectX::XMMatrixInverse(nullptr, m_pCamera.lock()->GetView());
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
	//mtx = DirectX::XMMATRIX(
	//	DirectX::XMMatrixScaling(t.localscale.x, t.localscale.y, 0)
	//	* mInvCam
	//	* DirectX::XMMatrixTranslation(t.localpos.x, t.localpos.y, t.localpos.z));
	//m_pOwner.lock()->SetWorldMatrix(mtx);
}
