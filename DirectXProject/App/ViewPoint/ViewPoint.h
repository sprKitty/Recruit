#pragma once
#include <MyMath.h>
#include <System/ClassDesign/Property.h>
#include <System/RenderTarget.h>
#include <memory>
#include <tuple>


namespace FrustumType
{
	enum kind
	{
		INSIDE,
		PARTLYINSIDE,
		OUTSIDE,
	};
}

class Transform;
class RenderTarget;
class ShaderBuffer;

class ViewPoint
{
public:
	ViewPoint();
	virtual ~ViewPoint() {}

	virtual void Init() = 0;
	virtual void Uninit() = 0;
	virtual void Update() = 0;

	virtual void Bind3D(const std::weak_ptr<ShaderBuffer> pBuf, const int nBufferNum = 0) = 0;

	void CopyParameter(const std::weak_ptr<ViewPoint> pVP);
	void CalcReflect(const Vector3* pPoint = nullptr, const Vector3* pNormal = nullptr);
	void Restore();
	void BindRenderTarget();
	
	inline const float GetAspect() { return vpSize.get().x / vpSize.get().y; }
	DirectX::XMMATRIX GetVPMatrix() { return view.get() * projection.get(); }
	const FrustumType::kind CollisionFrustum(const DirectX::XMFLOAT3& pos, const float fRadius);
	inline void SetRenderTarget(std::unique_ptr<RenderTarget>& pRT) { m_pRenderTarget = std::move(pRT); }
	ID3D11ShaderResourceView* GetRenderingTexture(int num);

public:
	Property<std::weak_ptr<Transform> >			targetTransform;
	Property<DirectX::XMMATRIX>					view;
	Property<DirectX::XMMATRIX>					projection;
	Property<DirectX::XMFLOAT4X4>				worldMatrix;
	Property<Vector4>							color;
	Property<Vector3>							position;
	Property<Vector3>							look;
	Property<Vector3>							up;
	Property<Vector2>							vpSize;
	Property<Vector3>							side;
	Property<Vector3>							front;
	Property<Vector2>							parallelScale;
	Property<float>								nearclip;
	Property<float>								farclip;
	Property<float>								fov;
	Property<bool>								perspective;

protected:
	void CalcView(const float fUp = 1.0f);
	void CalcProjection();
	void CalcWorldMatrix();
	void CreateViewFrustum();
	void UpdateViewFrustum();

protected:
	static const int FRUSTUM_SIZE = 8;
	
	std::unique_ptr<RenderTarget>	m_pRenderTarget;

private:
	Vector3							m_vKeepPos;	// ˆêŽž•Û‘¶—p
	Vector3							m_vKeepLook;	// ˆêŽž•Û‘¶—p
	DirectX::XMMATRIX				m_mKeepView;	// ˆêŽž•Û‘¶—p 
	DirectX::XMFLOAT4X4				m_mWorld;
	DirectX::XMFLOAT4				m_frustum[6];
	DirectX::XMFLOAT4				m_frustumWorld[6];
};