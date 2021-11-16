#pragma once
#include <App/Component/Component.h>
#include <Shader/ShaderBuffer.h>

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
		WORLD_OF_NORMAL,	// シャドウマッピング
		WORLD_OF_SHADOW,	// シャドウマッピング
		WORLD_OF_TOON,		// トゥーンマッピング
		UI,
		MAX,
	};
}

class Renderer : public Component
{
public:
	Renderer() {}
	virtual ~Renderer()override {}

	virtual void Init()override {}
	virtual void Uninit()override {}
	virtual void Update()override {}

	virtual void Write(const WriteType::kind type) {}
	virtual void Draw(const DrawType::kind type) {}

};
