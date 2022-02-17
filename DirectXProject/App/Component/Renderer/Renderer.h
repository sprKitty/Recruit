#pragma once
#include <App/Component/Component.h>
#include <Shader/ShaderBuffer.h>
#include <App/ViewPoint/ViewPoint.h>

namespace WriteStep
{
	enum kind
	{
		LIGHT_DEPTH,
		MAX,
	};
}

namespace DrawStep
{
	enum kind
	{
		WORLD,
		WATER,
		WATERRELECTION,
		UI,
		MAX,
	};
}

class ShaderBuffer;

class Renderer : public Component
{
public:
	Renderer() 
	{
		m_vsType = VS_TYPE::MAX;
		m_psType = PS_TYPE::MAX;

		m_isDrawStepList.resize(DrawStep::MAX);
		for (auto itr : m_isDrawStepList)
		{
			itr = false;
		}
		
		m_isWriteStepList.resize(WriteStep::MAX);
		for (auto itr : m_isWriteStepList)
		{
			itr = false;
		}
	}
	virtual ~Renderer()override {}

	virtual void Init()override {}
	virtual void Uninit()override {}
	virtual void Update()override {}

	virtual void Write(const std::weak_ptr<ShaderBuffer>& pBuf, const WriteStep::kind type) {}
	virtual void Draw(const std::weak_ptr<ShaderBuffer>& pBuf, const DrawStep::kind type) {}

	virtual const bool CalcFrustumState(const std::weak_ptr<ViewPoint>& pVP) { return true; }

	inline void EnableDrawStep(const DrawStep::kind type) { m_isDrawStepList[type] = true; }
	inline void DisableDrawStep(const DrawStep::kind type) { m_isDrawStepList[type] = false; }
	inline void EnableWriteStep(const WriteStep::kind type) { m_isWriteStepList[type] = true; }
	inline void DisableWriteStep(const WriteStep::kind type) { m_isWriteStepList[type] = false; }


	void SetVS(const VS_TYPE::Kind type) { m_vsType = type; }
	void SetPS(const PS_TYPE::Kind type) { m_psType = type; }

protected:
	std::vector<bool> m_isDrawStepList;
	std::vector<bool> m_isWriteStepList;
	VS_TYPE::Kind m_vsType;
	PS_TYPE::Kind m_psType;
};
