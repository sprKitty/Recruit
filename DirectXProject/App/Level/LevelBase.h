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
	* @brief 初期化処理
	*/
	virtual void Initialize(const std::weak_ptr<SceneBase> pScene, const Object::WORKER_OBJ pObject, const std::weak_ptr<MessageWindow> pMW) = 0;
	
	/*
	* @brief 更新処理
	*/
	virtual const Level_Type::Kind Transition(const Object::WORKER_OBJ pObject) = 0;

	/*
	* @brief 解放処理
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