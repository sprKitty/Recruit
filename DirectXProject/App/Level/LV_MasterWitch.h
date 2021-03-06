#pragma once
#include <App/Level/LevelBase.h>

class LV_MasterWitch : public LevelBase
{
public:
	LV_MasterWitch() {}
	~LV_MasterWitch() override{}

	void Initialize(const std::weak_ptr<SceneBase> pScene, const Object::WORKER_OBJ pObject, const std::weak_ptr<MessageWindow> pMW)override;
	const Level_Type::Kind Transition(const Object::WORKER_OBJ pObject)override;
	void Finalize(const Object::WORKER_OBJ pObject)override;

private:

};