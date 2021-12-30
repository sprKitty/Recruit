#pragma once
#include <App/Component/Component.h>
#include <Shader/ShaderBuffer.h>

namespace WriteType
{
	enum kind
	{
		DEPTH_OF_SHADOW,	// �V���h�E�[�x
		DEPTH_OF_FIELD,		// ��ʊE�[�x
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
		WORLD_OF_CHARACTER,	// �L�����N�^�[�p
		WORLD_OF_WATER,	// 
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

	virtual void Write(const std::weak_ptr<ShaderBuffer> pBuf, const WriteType::kind type) {}
	virtual void Draw(const std::weak_ptr<ShaderBuffer> pBuf, const DrawType::kind type) {}

};
