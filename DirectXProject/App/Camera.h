#pragma once

#include <MyMath.h>

class Camera
{
public:
	Camera();
	~Camera();

	void Init(bool isMain);
	void Uninit();
	void Update();
	void RotateUpdate();

	inline DirectX::XMFLOAT4 GetPos() { return m_vPos; }
	inline DirectX::XMFLOAT4 GetLook() { return m_vLook; }
	inline DirectX::XMMATRIX GetView(Vector3* pUp = nullptr);
	inline DirectX::XMMATRIX GetProj();
	inline DirectX::XMMATRIX GetZoomProj();
	inline void SetZoom(int zoom) { m_nZoom = zoom; }
	inline void SetSensivility(float sensi) { m_fSensivility = sensi; }
	void Bind3D(bool zoom);
	void ReverseBind3D();
	void Blur(bool flg);
	void Bind2D();
	void UpVector(DirectX::XMFLOAT4 &up, DirectX::XMFLOAT4 &right);
	float GetViewAngle();
	float GetAspect();
	static Camera* m_pMain;
private:
	DirectX::XMFLOAT4 m_vPos;
	DirectX::XMFLOAT4 m_vLook;
	DirectX::XMFLOAT4 m_vUp;
	DirectX::XMFLOAT4 m_vLimitLook[2];
	float m_fNearClip;
	float m_fFarClip;
	float m_fSensivility;
	float m_fRotate;
	int m_nZoom;
	DirectX::XMFLOAT2 m_vAngle;
	bool m_isF1;
};