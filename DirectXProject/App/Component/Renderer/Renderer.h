#pragma once
#include <App/Component/Component.h>
#include <Shader/ShaderBuffer.h>
#include <App/ViewPoint/ViewPoint.h>

namespace WriteType
{
	enum kind
	{
		DEPTH_OF_SHADOW,	// シャドウ深度
		DEPTH_OF_FIELD,		// 被写界深度
		MAX,
	};
}

namespace DrawType
{
	enum kind
	{
		WORLD_OF_NORMAL,	// 
		WORLD_OF_EFFECT,	// 
		WORLD_OF_TRIPLANAR,	// 
		WORLD_OF_CHARACTER,	// キャラクター用
		WORLD_OF_WATER,	// 
		WORLD_OF_GRASS,	// 
		UI,
		MAX,
	};
}

class ShaderBuffer;

class Renderer : public Component
{
public:
	Renderer() {}
	virtual ~Renderer()override {}

	virtual void Init()override {}
	virtual void Uninit()override {}
	virtual void Update()override {}

	virtual void Write(const std::weak_ptr<ShaderBuffer>& pBuf, const WriteType::kind type) {}
	virtual void Draw(const std::weak_ptr<ShaderBuffer>& pBuf, const DrawType::kind type) {}

	virtual void CalcFrustumState(const std::weak_ptr<ViewPoint>& pVP) {}

protected:
	FrustumType::kind m_frustumType;
};
