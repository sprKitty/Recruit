#include "BillBoardRenderer.h"
#include <App/Component/Object.h>
#include <App/Component/Image.h>
#include <App/Camera.h>
#include <App/RenderPipeline.h>
#include <System/Geometory.h>

void BillBoardRenderer::Init()
{
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
	RenderPipeline::GetInstance().AddRenderer(weak_from_this());
}

void BillBoardRenderer::Uninit()
{
	RenderPipeline::GetInstance().ReleaseRenderer(weak_from_this());
}

void BillBoardRenderer::Update()
{
}

void BillBoardRenderer::Write(const WriteType::kind type)
{
	if (!m_isWriteType[type])return;

}

void BillBoardRenderer::Draw(const DrawType::kind type)
{
	if (!m_isDrawType[type])
	{
		std::cout << "ビルボード使用するカメラが設定されていません" << std::endl;
		return;
	}
	if (!m_pMainImage.expired())
	{
		m_pMainImage.lock()->Bind();
	}
	if (!m_pBumpImage.expired())
	{
		m_pBumpImage.lock()->Bind();
	}

	DirectX::XMMATRIX mtx = DirectX::XMMatrixIdentity();
	CalcBillBoard(mtx);
	ShaderBuffer::GetInstance().SetWorld(mtx);
	Geometory::GetInstance().DrawCharacterPolygon();
}

void BillBoardRenderer::CalcBillBoard(DirectX::XMMATRIX & mtx)
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
	Transform t = m_pOwner.lock()->GetTransform();
	mtx = DirectX::XMMATRIX(
		DirectX::XMMatrixScaling(t.scale.x, t.scale.y, 0)
		* mInvCam
		* DirectX::XMMatrixTranslation(t.pos.x, t.pos.y, t.pos.z));
}
