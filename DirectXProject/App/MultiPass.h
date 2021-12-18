#pragma once
#include <System/DirectX.h>
#include <System/RenderTarget.h>
#include <memory>


class RenderTarget;
class DepthStencil;

class MultiPass
{
public:
	MultiPass() {}
	~MultiPass() {}

	void Create(const Vector2& vLeft, const Vector2& vRight, const Vector4& vClear, const DXGI_FORMAT format);

	void Bind();

	ID3D11ShaderResourceView* GetRenderTex() { return m_pRenderTarget->GetSRV(); }

	inline const std::weak_ptr<RenderTarget> GetRenderTarget() { return m_pRenderTarget; }

	inline const std::weak_ptr<DepthStencil> GetDepthStencil() { return m_pDepthDtencil; }

	inline const Vector2 GetVPSize() { return Vector2(m_vp.Width, m_vp.Height); }

private:
	D3D11_VIEWPORT m_vp;
	std::shared_ptr<RenderTarget> m_pRenderTarget;
	std::shared_ptr<DepthStencil> m_pDepthDtencil;
	Vector4 m_vClearColor;
};