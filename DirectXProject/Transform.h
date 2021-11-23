#pragma once
#include <App/Component/Component.h>
#include <Vector.h>
#include <DirectXMath.h>
#include <MyMath.h>


class Transform : public Component
{
public:
	Transform() {}
	~Transform()override {}

	void Init()override;
	void Uninit()override;
	void Update()override;

	inline const DirectX::XMMATRIX& GetWorldMatrix()
	{
		return m_mWorldMatrix;
	}

	inline void SetMatrix(const DirectX::XMMATRIX& mtx)
	{
		m_mMatrix = mtx;
	}

	inline void SetPos(const Vector3& vPos)
	{
		localpos = vPos;
	}

public:
	//DirectX::XMFLOAT4X4 world;
	Vector3 localpos;
	Vector3 localrot;
	Vector3 localscale;

private:
	DirectX::XMMATRIX m_mMatrix;
	DirectX::XMMATRIX m_mWorldMatrix;

};