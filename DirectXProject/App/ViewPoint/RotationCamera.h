#pragma once
#include <App/ViewPoint/Camera.h>


class RotationCamera : public Camera
{
public:
	RotationCamera() {}
	~RotationCamera()override {}

	void Init()override;
	void Uninit()override;
	void Update()override;

	void UpdateRotation();
private:
	Vector3 m_vNormal;
	float m_fDisatance;
	float m_fDegeree;
};