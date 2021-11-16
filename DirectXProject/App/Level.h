#pragma once
#include <memory>

class SceneBase;

class Level
{
public:
	Level() {}
	~Level() {}

	virtual void Create(std::weak_ptr<SceneBase> pScene);

private:

};