#include "MultiPass.h"


void MultiPass::Create(const Vector2& vLeft, const Vector2& vRight, const Vector4& vClear)
{
	m_vp.TopLeftX = vLeft.x;
	m_vp.TopLeftY = vLeft.y;
	m_vp.Width = vRight.x;
	m_vp.Height = vRight.y;
	m_vp.MaxDepth = 1.0f;
	m_vp.MinDepth = 0.0f;
	m_vClearColor = vClear;

	m_pRenderTarget.reset(new RenderTarget());
	m_pDepthDtencil.reset(new MyDepthStencil());
	m_pRenderTarget->Create(static_cast<UINT>(vRight.x), static_cast<UINT>(vRight.y), DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_FORMAT_R8G8B8A8_UNORM);
	m_pDepthDtencil->Create(static_cast<UINT>(vRight.x), static_cast<UINT>(vRight.y), DrawPass::MULTIPATH);
}

void MultiPass::Bind()
{
	DirectX11::GetInstance().BeginDraw(&m_vp, m_pRenderTarget->Get(), m_pDepthDtencil->Get(), &m_vClearColor);
}
