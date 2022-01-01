#pragma once
#include <memory>
#include <App/Scene/SceneBase.h>
#include <App/Component/Object.h>


class MessageWindow;

class LevelBase
{
public:
	LevelBase() {}
	virtual ~LevelBase() {}

	virtual void Initialize(const std::weak_ptr<SceneBase> pScene, const Object::WORKER_OBJ pObject, const std::weak_ptr<MessageWindow> pMW) = 0;
	
	// ‰ð•úˆ—
	virtual void Finalize(const Object::WORKER_OBJ pObject) = 0;

protected:
	inline void ReleaseObjectList()
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