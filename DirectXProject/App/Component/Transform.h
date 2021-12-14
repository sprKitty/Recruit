#pragma once
#include <App/Component/Component.h>
#include <MyMath.h>


class Transform : public Component
{
public:
	Transform() {}
	~Transform()override {}

	void Init()override;
	void Uninit()override;
	void Update()override;

	const DirectX::XMMATRIX GetWorldMatrix(const Vector3& vScale = 1, const Vector3& vAngle = 0, const Vector3& vPos = 0);

	inline void SetMatrix(const DirectX::XMMATRIX& mtx)
	{
		m_mMatrix = mtx;
	}

	inline void SetPos(const Vector3& vPos)
	{
		localpos = vPos;
	}

public:
	Vector3 localpos;
	Vector3 localrot;
	Vector3 localscale;

private:
	DirectX::XMMATRIX m_mMatrix;

};