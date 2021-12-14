#pragma once
#include <MyMath.h>


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
	inline const DirectX::XMFLOAT4& GetPos() { return m_pos; }
	inline const DirectX::XMFLOAT4& GetLook() { return m_look; }
	inline const DirectX::XMFLOAT4& GetUp() { return m_up; }
	inline const DirectX::XMFLOAT4& GetRight() { return m_right; }
	inline const DirectX::XMFLOAT4& GetDirection() { return m_direction; }
	inline const float GetFov() { return m_fFov; }
	inline const float GetAspect() { return m_vScreenSize.x / m_vScreenSize.y; }

	void SetVPSize(const Vector2& vSize) { m_vScreenSize = vSize; }

	void CalcView();
	void CalcProjection();

private:
	DirectX::XMMATRIX m_mView;
	DirectX::XMMATRIX m_mProjection;
	DirectX::XMFLOAT4 m_pos;
	DirectX::XMFLOAT4 m_look;
	DirectX::XMFLOAT4 m_up;
	DirectX::XMFLOAT4 m_right;
	DirectX::XMFLOAT4 m_direction;
	Vector2 m_vScreenSize;
	float m_fNearClip;
	float m_fFarClip;
	float m_fFov;
};

//class Light
//{
//public:
//	Light();
//	~Light();
//
//	void Init(Vector3* pPos,Vector3* pDirection, Vector2* pNearFar, Vector2 debug, float uvoffset,float fov = 45.0f, float multi = 1.0f);
//	void Uninit();
//	void Update();
//
//	void SetDirection(Vector4 dir) { m_vDirection = dir; }
//	void SetDecay(Vector4 decay) { m_vDecay = decay; }
//	void SetDistance(float dis) { m_fDistance = dis; }
//	void SetSpot(bool flg) { m_isSpot = flg; }
//	void On(bool flg) { m_isOn = flg; }
//	void Bind();
//	void UpdateValue(int num);
//	Vector3 GetPos() { return Vector3(m_vPos.x, m_vPos.y, m_vPos.z); }
//	DirectX::XMMATRIX GetView();
//	DirectX::XMMATRIX GetProj();
//private:
//	Vector4 m_vPos;
//	Vector4 m_vLook;
//	Vector4 m_vUp;
//	Vector4 m_vColor;
//	Vector4 m_vDirection;
//	Vector4 m_vDecay;
//	Vector4 m_vDebugValue;
//	Vector2 m_rotation;
//
//	float m_fDistance;
//	float m_rot;
//	float m_fUVOffset;
//	bool m_isSpot;
//	bool m_isOn;
//	float m_fFOV;
//	float m_fMultipray;
//	float m_fNearClip;
//	float m_fFarClip;
//};