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

	/*
	* @brief オブジェクトリストに追加
	*/
	void AddObject(const Object::OWNER_OBJ& pObj);
	
	/*
	* @brief ファクトリーのオブジェクトリストをシーンのリストに移動
	*/
	void MoveObject_FactoytoScene();

	/*
	* @brief オブジェクトリストからオブジェクトを削除
	*/
	void DeleteObject();

	const Object::WORKER_OBJECTLIST GetObjectList();

	inline void SetShaderBuffer(const std::weak_ptr<ShaderBuffer> ptr) { m_pShaderBuffer = ptr; }

protected:
	Object::OWNER_OBJECTLIST m_pObjList;
	std::list<Object*> m_pUIList;

	std::shared_ptr<Camera> m_pCamera;
	std::shared_ptr<Light> m_pLight;
	std::weak_ptr<ShaderBuffer> m_pShaderBuffer;

private:
};