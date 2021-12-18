#pragma once
#include <MyMath.h>
#include <System/ClassDesign/Property.h>
#include <memory>


class Transform;

class Camera
{
public:
	Camera();
	~Camera();

	void Init();
	void Uninit();
	void Update();

	void Bind3D();
	void Bind2D();

	inline const DirectX::XMMATRIX& GetView() { return m_mView; }
	inline const DirectX::XMMATRIX& GetProjection() { return m_mProjection; }
	inline const Vector3& GetPos() { return m_vPos; }
	inline const Vector3& GetLook() { return m_vLook; }
	inline const Vector3& GetUp() { return m_vUp; }
	inline const Vector3& GetRight() { return m_vSide; }
	inline const Vector3& GetFront() { return m_vFront; }
	inline const float GetFov() { return m_fFov; }
	inline const float GetAspect() { return m_vScreenSize.x / m_vScreenSize.y; }

	inline void SetTarget(const std::weak_ptr<Transform>& pTrans) { m_pTargetTrans = pTrans; }
	inline void ReleaseTarget() { m_pTargetTrans.reset(); }

	inline void EnableLate() { m_isLate = true; }
	inline void DisableLate() { m_isLate = false; }

private:
	void CalcView();
	void CalcProjection();

private:
	std::weak_ptr<Transform> m_pTargetTrans;
	DirectX::XMMATRIX m_mView;
	DirectX::XMMATRIX m_mProjection;
	Vector3 m_vPos;
	Vector3 m_vLatePos;
	Vector3 m_vLook;
	Vector3 m_vLateLook;
	Vector3 m_vUp;
	Vector3 m_vSide;
	Vector3 m_vFront;
	Vector3 m_vOffset;
	Vector2 m_vScreenSize;
	float m_fNearClip;
	float m_fFarClip;
	float m_fFov;
	bool m_isLate;
};