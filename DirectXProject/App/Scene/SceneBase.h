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

	void DeleteObject()
	{
		for (auto itr = m_pObjList.begin(); itr != m_pObjList.end();)
		{
			if ((*itr)->IsDelete())
			{
				itr->reset();
				itr = m_pObjList.erase(itr);
			}
			if (itr == m_pObjList.end())
			{
				break;
			}
			++itr;
		}
	}

	inline void SetShaderBuffer(const std::weak_ptr<ShaderBuffer> ptr) { m_pShaderBuffer = ptr; }

protected:
	Object::OWNER_OBJECTLIST m_pObjList;
	std::list<Object*> m_pUIList;

	std::shared_ptr<Camera> m_pCamera;
	std::shared_ptr<Light> m_pLight;
	std::weak_ptr<ShaderBuffer> m_pShaderBuffer;

private:
};