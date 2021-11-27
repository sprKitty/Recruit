#pragma once
#include <memory>
#include <System/ClassDesign/Singleton.h>
#include <App/Component/Object.h>


class Camera;
class Light;
class Mouse;
class Talk;
class Light;
class GameKeyBind;

class FactoryMethod : public Singleton<FactoryMethod>
{
public:
	friend class Singleton<FactoryMethod>;

public:
	void Initialize()override {}
	void Finalize()override {}

	Object::WORKER_OBJ CreateObject();

	Object::WORKER_OBJ CreatePlayerObject(std::weak_ptr<GameKeyBind> pGKB);

	Object::WORKER_OBJ CreatePlayerMagic();

	Object::WORKER_OBJ CreateBossWitchObject();

	Object::WORKER_OBJ CreateBossWitchMagic();

	Object::WORKER_OBJ CreateBoss1Object();

	Object::WORKER_OBJ CreateEventObject();

	std::shared_ptr<Talk> CreateTalkEvent(const char* pPath);

	inline void SetCamera(std::weak_ptr<Camera> pCamera)
	{
		m_pCamera = pCamera;
	}

	void SetMouse(std::weak_ptr<Mouse> pMouse)
	{
		m_pMouse = pMouse;
	}

	void ReleaseResource()
	{
		m_pCamera.reset();
		m_pMouse.reset();
	}

	void MoveObjectList(Object::OWNER_OBJECTLIST& pObjectList)
	{
		for (int i = 0; i < m_pObjectList.size(); ++i)
		{
			pObjectList.push_back(m_pObjectList[i]);
		}
		m_pObjectList.clear();
	}

protected:
	FactoryMethod() {}
	~FactoryMethod()override {}

private:
	Object::OWNER_OBJECTLIST m_pObjectList;
	std::weak_ptr<Camera> m_pCamera;
	std::weak_ptr<Mouse> m_pMouse;
};