#pragma once
#include <System/DirectX.h>
#include <MyMath.h>
//#include <vector>

class RenderTarget
{
public:
	RenderTarget();
	~RenderTarget();

	void Create(const FLOAT width, const FLOAT height, const DXGI_FORMAT format, const Vector4& vClearColor);
	void Addition(const DXGI_FORMAT format, const Vector4& vClearColor);
	void CreateDepthStencil(const DXGI_FORMAT depthFormat);
	
	inline const int GetNum() { return m_nNum; }
	inline ID3D11ShaderResourceView** GetSRV() { return m_ppSRV; }
	inline ID3D11RenderTargetView** GetRTV() { return m_ppRTV; }
	inline ID3D11DepthStencilView* GetDepthStecil() { return m_pDepthStencil; }
	inline D3D11_VIEWPORT& GetViewPort() { return m_vp; }
	inline std::vector<Vector4>& GetClearColorList() { return m_vClearColorList; }
private:
	ID3D11RenderTargetView** m_ppRTV;
	ID3D11ShaderResourceView** m_ppSRV;
	ID3D11DepthStencilView*	m_pDepthStencil;
	D3D11_VIEWPORT m_vp;
	std::vector<Vector4> m_vClearColorList;
	int m_nNum;
};
