#pragma once
#include <memory>
#include <App/Scene/SceneBase.h>
#include <App/Component/Object.h>


class LevelBase
{
public:
	LevelBase() {}
	virtual ~LevelBase() {}


	virtual void Create(std::weak_ptr<SceneBase> pScene) = 0;
	
	// ‰ð•úˆ—
	inline void Finalize()
	{
		for (const auto& itr : m_pObjectList)
		{
			if (itr.expired())continue;

			itr.lock()->DisableActive();
			itr.lock()->EnableDelete();
		}
	}

protected:
	Object::WORKER_OBJECTLIST m_pObjectList;

private:
	
};