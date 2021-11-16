#pragma once
#include <App/Component/Object.h>
#include <Shader/ShaderBuffer.h>
#include <Defines.h>
#include <System/Input.h>

class Camera;
class Light;

namespace Scene_Type
{
	enum kind
	{
		SCENE_TITLE = 0,
		SCENE_GAME,

		SCENE_MAX
	};
}

class SceneBase : public std::enable_shared_from_this<SceneBase>
{
public:
	SceneBase() {}
	virtual ~SceneBase(){}

	virtual void Init() = 0;
	virtual void Uninit() = 0;
	virtual Scene_Type::kind Update() = 0;
	virtual void Draw() = 0;

	virtual void AddObj(const Object::OWNER_OBJ& pObj)
	{
		if (!PTRNULLCHECK(pObj))
		{
			m_pObjList.push_back(std::move(pObj));
		}
	}

	//virtual void DeleteObj()
	//{
	//	for (auto pObj = m_pObjList.begin(); pObj != m_pObjList.end();)
	//	{
	//		if (!(*pObj)->GetParent().expired())
	//		{
	//			if ((*pObj)->GetParent().lock())
	//			{
	//				(*pObj)->Uninit();
	//				delete *pObj;
	//				*pObj = nullptr;
	//				pObj = m_pObjList.erase(pObj);
	//				continue;
	//			}
	//		}
	//		if (pObj == m_pObjList.end())
	//			break;
	//		++pObj;
	//	}
	//	for (auto pObj = m_pObjList.begin(); pObj != m_pObjList.end();)
	//	{
	//		if ((*pObj)->GetDelete())
	//		{
	//			(*pObj)->Uninit();
	//			delete *pObj;
	//			*pObj = nullptr;
	//			pObj = m_pObjList.erase(pObj);
	//			continue;
	//		}
	//		if (pObj == m_pObjList.end())
	//			break;
	//		++pObj;
	//	}
	//}

protected:
	Object::OWNER_OBJECTLIST m_pObjList;
	std::list<Object*> m_pUIList;

	std::shared_ptr<Camera> m_pCamera;
	std::shared_ptr<Light> m_pLight;

private:
};