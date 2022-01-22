#pragma once
#include <memory>
#include <App/Scene/SceneBase.h>
#include <App/Component/Object.h>


namespace Level_Type
{
	enum Kind
	{
		BRANCH,
		MASTERWITCH,
		DRAGON1,
		DRAGON2,
		TUTORIAL,
		MAX,
	};
}

class MessageWindow;

class LevelBase
{
public:
	LevelBase() {}
	virtual ~LevelBase() {}

	/*
	* @brief èâä˙âªèàóù
	*/
	virtual void Initialize(const std::weak_ptr<SceneBase> pScene, const Object::WORKER_OBJ pObject, const std::weak_ptr<MessageWindow> pMW) = 0;
	
	/*
	* @brief çXêVèàóù
	*/
	virtual const Level_Type::Kind Transition(const Object::WORKER_OBJ pObject) = 0;

	/*
	* @brief âï˙èàóù
	*/
	virtual void Finalize(const Object::WORKER_OBJ pObject) = 0;

protected:
	inline void ReleaseObjectList()
	{
		for (const auto& itr : m_pObjectList)
		{
			if (itr.expired())continue;
			itr.lock()->Uninit();
		}
	}

protected:
	Object::WORKER_OBJECTLIST m_pObjectList;

private:
	
};