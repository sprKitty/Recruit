#pragma once
#include <App/ViewPoint/Camera.h>


class LateCamera : public Camera
{
public:
	LateCamera() {}
	~LateCamera()override {}

	void Init()override;
	void Uninit()override;
	void Update()override;

private:
	Vector3 m_vLatePos;
	Vector3 m_vLateLook;
};