#include "MultiPass.h"


void MultiPass::Create(const Vector2& vLeft, const Vector2& vRight, const Vector4& vClear, const DXGI_FORMAT format)
{
	m_vp.TopLeftX = vLeft.x;
	m_vp.TopLeftY = vLeft.y;
	m_vp.Width = vRight.x;
	m_vp.Height = vRight.y;
	m_vp.MaxDepth = 1.0f;
	m_vp.MinDepth = 0.0f;
	m_vClearColor = vClear;

	m_pRenderTarget.reset(new RenderTarget());
	m_pDepthDtencil.reset(new DepthStencil());
	m_pRenderTarget->Create(static_cast<UINT>(vRight.x), static_cast<UINT>(vRight.y), format, format, DrawPass::MULTIPATH);
	m_pDepthDtencil->Create(static_cast<UINT>(vRight.x), static_cast<UINT>(vRight.y), DrawPass::MULTIPATH);
}

void MultiPass::Bind()
{
	ID3D11RenderTargetView* aaa[]=
	{
		m_pRenderTarget->GetRTV(),
	};
	DirectX11::GetInstance().BeginDraw(&m_vp, aaa, m_pDepthDtencil->Get(), &m_vClearColor);
}
