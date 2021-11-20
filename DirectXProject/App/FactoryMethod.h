#pragma once
#include <memory>
#include <App/Camera.h>
#include <App/Light.h>
#include <System/Mouse.h>
#include <App/Component/Event/EventTrigger.h>
#include <App/Component/Event/Event.h>
#include <App/Component/Renderer/Renderer3D.h>
#include <App/Component/Renderer/BillBoardRenderer.h>
#include <App/Component/Object.h>
#include <App/Component/Character/Player.h>
#include <App/Component/Character/MasterWitch.h>
#include <App/Event/Talk.h>
#include <App/EventMgr.h>
#include <App/GameKeyBind.h>
#include <System/ClassDesign/Singleton.h>


class FactoryMethod : public Singleton<FactoryMethod>
{
public:
	friend class Singleton<FactoryMethod>;

public:
	void Initialize()override {}
	void Finalize()override {}

	Object::OWNER_OBJ CreateObject()
	{
		Object::OWNER_OBJ pObj(new Object());
		pObj->SetType(Object::Type::NONE);
		std::weak_ptr<BillBoardRenderer> pBBR = pObj->AddComponent<BillBoardRenderer>();
		if (!pBBR.expired())
		{
			pBBR.lock()->EnableDraw(DrawType::WORLD_OF_NORMAL);
			if (!m_pCamera.expired())
			{
				pBBR.lock()->SetCamera(m_pCamera);
			}
		}
		if (!m_pMouse.expired())
		{
			m_pMouse.lock()->SetExecuteFunc(Delegate<Object, void, const Vector3&>::CreateDelegator(pObj, &Object::SetPos));
		}
		return std::move(pObj);
	}

	Object::OWNER_OBJ CreatePlayerObject(std::weak_ptr<GameKeyBind> pGKB)
	{
		Object::OWNER_OBJ pObj(new Object());
		pObj->SetType(Object::Type::PLAYER);
		
		
		std::weak_ptr<Player> pPlayer = pObj->AddComponent<Player>();
		std::weak_ptr<BillBoardRenderer> pBBR = pObj->AddComponent<BillBoardRenderer>();
		if (!pPlayer.expired() && !pBBR.expired())
		{
			pPlayer.lock()->SetBillBoardRenderer(pBBR);
			pBBR.lock()->EnableDraw(DrawType::WORLD_OF_NORMAL);
			pBBR.lock()->SetCamera(m_pCamera);
		}
		if (!m_pMouse.expired())
		{
			m_pMouse.lock()->SetExecuteFunc(Delegate<Player, void, const Vector3&>::CreateDelegator(pPlayer, &Player::SetMousePos));
		}
		if (!pGKB.expired())
		{
			pGKB.lock()->SetKeyInfo(KeyBind::MOVE, KeyType::PRESS, VK_RBUTTON, Delegate_void<Player, void>::CreateDelegator(pPlayer, &Player::EnableChangeDestination));
		}
		std::weak_ptr<EventTrigger> pET(pObj->AddComponent<EventTrigger>());
		if (!pET.expired())
		{
			pET.lock()->SetType(EventTrigger::Type::TALK_1);
		}
		
		return std::move(pObj);
	}

	Object::OWNER_OBJ CreateBossWitchObject()
	{
		Object::OWNER_OBJ pObj(new Object());
		pObj->SetType(Object::Type::BOSS_WITCH);
		std::weak_ptr<MasterWitch> pMasterWitch = pObj->AddComponent<MasterWitch>();
		std::weak_ptr<BillBoardRenderer> pBBR = pObj->AddComponent<BillBoardRenderer>();
		pObj->AddComponent<Event>();
		if (!pMasterWitch.expired() && !pBBR.expired())
		{
			pMasterWitch.lock()->SetBillBoardRenderer(pBBR);
			pBBR.lock()->EnableDraw(DrawType::WORLD_OF_NORMAL);
			pBBR.lock()->SetCamera(m_pCamera);
		}
		std::weak_ptr<EventTrigger> pET(pObj->AddComponent<EventTrigger>());
		if (!pET.expired())
		{
			pET.lock()->SetType(EventTrigger::Type::TALK_1);
		}
		return std::move(pObj);
	}

	Object::OWNER_OBJ CreateBoss1Object()
	{
		Object::OWNER_OBJ pObj(new Object());
		return std::move(pObj);
	}

	Object::OWNER_OBJ CreateEventObject()
	{
		Object::OWNER_OBJ pObj(new Object());
		pObj->AddComponent<Event>();
		return std::move(pObj);
	}

	std::shared_ptr<Talk> CreateTalkEvent(const char* pPath)
	{
		std::shared_ptr<Talk> pTalk(new Talk());
		pTalk->Load(pPath);
		pTalk->Init();
		return std::move(pTalk);
	}

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

protected:
	FactoryMethod() {}
	~FactoryMethod()override {}

private:
	std::weak_ptr<Camera> m_pCamera;
	std::weak_ptr<Mouse> m_pMouse;
};