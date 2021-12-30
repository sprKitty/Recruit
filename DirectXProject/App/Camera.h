#pragma once
#include <App/ViewPoint.h>


class Transform;
class RenderTarget;

class Camera : public ViewPoint
{
public:
	Camera() {}
	~Camera()override {}

	void Init()override;
	void Uninit()override;
	void Update()override;

	void Bind3D(const std::weak_ptr<ShaderBuffer> pBuf, const int nBufferNum = 0)override;
	void Bind2D(const std::weak_ptr<ShaderBuffer> pBuf);


private:
	Vector3 m_vLatePos;
	Vector3 m_vLateLook;
	bool m_isLate;
};