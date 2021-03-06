#pragma once
#include "LevelBase.h"

class LV_StageBranch final : public LevelBase
{
public:
	LV_StageBranch() {}
	virtual ~LV_StageBranch() override {}

	virtual void Initialize(const std::weak_ptr<SceneBase> pScene, const Object::WORKER_OBJ pObject, const std::weak_ptr<MessageWindow> pMW)override;
	virtual const Level_Type::Kind Transition(const Object::WORKER_OBJ pObject)override;
	virtual void Finalize(const Object::WORKER_OBJ pObject)override;

private:

};