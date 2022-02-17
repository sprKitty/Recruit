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
	
	//const FrustumType::kind CollisionViewFrustum(const DirectX::XMFLOAT3& pos, const float fRadius);

	inline const float GetAspect() { return vpSize.get().x / vpSize.get().y; }

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
	//void CreateViewFrustum();
	//void UpdateViewFrustum();

protected:
	const int FRUSTUM_SIZE = 6;
	
	std::unique_ptr<RenderTarget>	m_pRenderTarget;
	//std::weak_ptr<Transform>		m_pTargetTransform;	// 
	//DirectX::XMMATRIX				m_mView;			//  
	//DirectX::XMMATRIX				m_mProjection;		// 
	//DirectX::XMFLOAT4X4				m_mWorldMatrix;		//
	//std::vector<DirectX::XMFLOAT4>	m_frustum;			// 視錐台
	//std::vector<DirectX::XMFLOAT4>	m_frustumWorld;			// 視錐台
	//Vector4							m_vColor;			// 色
	//Vector3							m_vPos;				// 座標
	//Vector3							m_vLook;			// 注視点
	//Vector3							m_vUp;				// 上ベクトル
	//Vector3							m_vSide;			// 横ベクトル
	//Vector3							m_vFront;			// 正面ベクトル
	//Vector2							m_vScreenSize;		// スクリーンサイズ
	//Vector2							m_vParallelScale;	// 平行投影拡縮
	//float							m_fNearClip;		// ニアクリップ
	//float							m_fFarClip;			// ファクリップ
	//float							m_fFov;				// 視野角
	//bool							m_isPerspective;	// 投影方法

private:
	Vector3							m_vKeepPos;	// 一時保存用
	Vector3							m_vKeepLook;	// 一時保存用
	DirectX::XMMATRIX				m_mKeepView;	// 一時保存用 
};