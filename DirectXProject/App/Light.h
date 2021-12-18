#pragma once
#include <MyMath.h>
#include <memory>
#include <System/ClassDesign/Property.h>


class Transform;

class Light
{
public:
	Light();
	~Light();

	void Init();
	void Uninit();
	void Update();

	void Bind3D();

	inline const DirectX::XMMATRIX& GetView() { return m_mView; }
	inline const DirectX::XMMATRIX& GetProjection() { return m_mProjection; }
	inline const Vector3& GetUp() { return m_vUp; }
	inline const Vector3& GetSide() { return m_vSide; }
	inline const Vector3& GetFront() { return m_vFront; }
	inline const float GetAspect() { return m_vScreenSize.x / m_vScreenSize.y; }


	inline void Perspectiveprojection() { m_isPerspective = true; }
	inline void Parallelprojection() { m_isPerspective = false; }

	Property<Vector4> color;
	Property<Vector3> position;
	Property<Vector3> look;
	Property<Vector2> vpSize;
	Property<Vector2> parallelScale;
	Property<float> nearclip;
	Property<float> farclip;
	Property<float> fov;


private:
	void CalcView();
	void CalcProjection();

private:
	std::weak_ptr<Transform> m_pTargetTrans;	// 
	DirectX::XMMATRIX m_mView;					// 
	DirectX::XMMATRIX m_mProjection;			// 
	Vector4 m_vColor;							// 色
	Vector3 m_vPos;								// 座標
	Vector3 m_vLook;							// 注視点
	Vector3 m_vUp;								// 上ベクトル
	Vector3 m_vSide;							// 横ベクトル
	Vector3 m_vFront;							// 正面ベクトル
	Vector3 m_vOffset;							// なにこれ
	Vector2 m_vScreenSize;						// スクリーンサイズ
	Vector2 m_vParallelScale;					// 平行投影拡縮
	float m_fNearClip;							// ニアクリップ
	float m_fFarClip;							// ファクリップ
	float m_fFov;								// 視野角
	bool m_isPerspective;							// 投影方法
};