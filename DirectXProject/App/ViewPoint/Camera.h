#pragma once
#include <App/ViewPoint/ViewPoint.h>
#include <MyMath.h>

class Camera : public ViewPoint
{
public:
	Camera() {}
	~Camera()override {}

	virtual void Init()override;
	virtual void Uninit()override;
	virtual void Update()override;

	virtual void Bind3D(const std::weak_ptr<ShaderBuffer> pBuf, const int nBufferNum = 0)override;
	virtual void Bind2D(const std::weak_ptr<ShaderBuffer> pBuf);

protected:
	static const Vector3 CameraInitPos;
	static const Vector3 CameraInitLook;
	static const float FOV;

private:
};