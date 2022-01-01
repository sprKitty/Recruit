#pragma once
#include <App/Level/LevelBase.h>

class LV_StageBranch : public LevelBase
{
public:
	LV_StageBranch() {}
	~LV_StageBranch() override {}

	void Initialize(const std::weak_ptr<SceneBase> pScene, const Object::WORKER_OBJ pObject, const std::weak_ptr<MessageWindow> pMW)override;

	void Finalize(const Object::WORKER_OBJ pObject)override;

private:

};