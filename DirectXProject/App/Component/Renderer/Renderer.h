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
		WORLD_OF_SHADOW,	// �V���h�E�}�b�s���O
		WORLD_OF_CHARACTER,		// �L�����N�^�[�p
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
