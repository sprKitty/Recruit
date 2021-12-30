#pragma once
#include <App/ViewPoint.h>


class Transform;
class RenderTarget;

class Light : public ViewPoint
{
public:
	Light() {}
	~Light()override {}

	void Init()override;
	void Uninit()override;
	void Update()override;

	void Bind3D(const std::weak_ptr<ShaderBuffer> pBuf, const int nBufferNum = 0)override;
};