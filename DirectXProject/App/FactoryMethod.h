#pragma once
#include <memory>
#include <System/ClassDesign/Singleton.h>
#include <App/Component/Object.h>
#include <App/Component/Event/EventTrigger.h>
#include <App/Component/Event/Event.h>
#include <App/Component/Renderer/Renderer3D.h>
#include <App/Component/Renderer/BillBoardRenderer.h>
#include <App/Component/Mesh.h>
#include <App/Component/Character/Player.h>
#include <App/Component/Character/MasterWitch.h>
#include <App/Component/Transform.h>
#include <App/Component/Collider.h>
#include <App/Component/Magic/MagicBullet.h>
#include <App/Component/Magic/MagicBall.h>
#include <App/Component/Magic/MagicRazer.h>
#include <App/Component/Instancing.h>
#include <App/Camera.h>
#include <App/Light.h>
#include <App/Event/Talk.h>
#include <App/EventMgr.h>
#include <App/GameKeyBind.h>
#include <System/Mouse.h>


class FactoryMethod : public Singleton<FactoryMethod>
{
public:
	friend class Singleton<FactoryMethod>;

public:
	void Initialize()override {}
	void Finalize()override {}

	Object::WORKER_OBJ CreateObject();
	
	Object::WORKER_OBJ CreateWater();

	Object::WORKER_OBJ CreateOutsideArea();

	Object::WORKER_OBJ CreatePlayerObject(std::weak_ptr<GameKeyBind> pGKB);

	Object::WORKER_OBJ CreatePlayerMagic();

	Object::WORKER_OBJ CreateBossWitchObject();

	Object::WORKER_OBJ CreateBossWitchMagicBullet();

	Object::WORKER_OBJ CreateBossWitchMagicBall();

	Object::WORKER_OBJ CreateBossWitchRazer();

	Object::WORKER_OBJ CreateBoss1Object();

	Object::WORKER_OBJ CreateEventObject();

	std::shared_ptr<Talk> CreateTalkEvent(const char* pPath);

	inline void SetCamera(std::weak_ptr<Camera> pCamera) { m_pCamera = pCamera; }

	inline void SetMouse(std::weak_ptr<Mouse> pMouse) { m_pMouse = pMouse; }

	void ReleaseResource()
	{
		m_pCamera.reset();
		m_pMouse.reset();
	}

	void MoveObjectList(Object::OWNER_OBJECTLIST& pObjectList);

protected:
	FactoryMethod() {}
	~FactoryMethod()override {}

private:
	Object::OWNER_OBJECTLIST m_pObjectList;
	std::weak_ptr<Camera> m_pCamera;
	std::weak_ptr<Mouse> m_pMouse;
};