#pragma once
#include <System/DirectX.h>
#include <System/RenderTarget.h>
#include <memory>


class RenderTarget;
class MyDepthStencil;

class MultiPass
{
public:
	MultiPass() {}
	~MultiPass() {}

	void Create(const Vector2& vLeft, const Vector2& vRight,const Vector4& vClear);

	void Bind();

	ID3D11ShaderResourceView* GetRenderTex() { return m_pRenderTarget->GetTexture(); }

	const Vector2 GetVPSize() { return Vector2(m_vp.Width, m_vp.Height); }

private:
	D3D11_VIEWPORT m_vp;
	std::shared_ptr<RenderTarget> m_pRenderTarget;
	std::shared_ptr<MyDepthStencil> m_pDepthDtencil;
	Vector4 m_vClearColor;
};