#pragma once
#include <App/Component/Object.h>
#include <Defines.h>
#include <Shader/ShaderBuffer.h>
#include <System/Input.h>
#include <App/Camera.h>
#include <App/Light.h>

enum Scene_Type
{
	SCENE_TITLE = 0,
	SCENE_GAME,

	SCENE_MAX
};

class Scene
{
public:
	Scene()
		:m_pCamera(nullptr)
		, m_pLight(nullptr)
	{}
	virtual ~Scene()
	{
		if (m_pCamera)
		{
			delete m_pCamera;
			m_pCamera = nullptr;
		}
	}

	virtual void Start() {}
	virtual void Uninit() {}
	virtual void Init() {}
	virtual Scene_Type Update(Scene_Type type) { return Scene_Type(); }
	virtual void Draw() {}

	virtual void AddObj(Object* pObj)
	{
		if (pObj)
			m_pObjList.push_back(pObj);
	}

	virtual void DeleteObj()
	{
		for (auto pObj = m_pObjList.begin(); pObj != m_pObjList.end();)
		{
			if ((*pObj)->GetParent())
			{
				if ((*pObj)->GetParent()->GetDelete())
				{
					(*pObj)->Uninit();
					delete *pObj;
					*pObj = nullptr;
					pObj = m_pObjList.erase(pObj);
					continue;
				}
			}
			if (pObj == m_pObjList.end())
				break;
			++pObj;
		}
		for (auto pObj = m_pObjList.begin(); pObj != m_pObjList.end();)
		{
			if ((*pObj)->GetDelete())
			{
				(*pObj)->Uninit();
				delete *pObj;
				*pObj = nullptr;
				pObj = m_pObjList.erase(pObj);
				continue;
			}
			if (pObj == m_pObjList.end())
				break;
			++pObj;
		}
	}
protected:
	std::list<Object*> m_pObjList;
	std::list<Object*> m_pUIList;

	Camera* m_pCamera;
	Light* m_pLight;

private:
};