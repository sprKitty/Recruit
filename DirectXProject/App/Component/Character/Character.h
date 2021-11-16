#pragma once
#include <App/Component/Component.h>


class Renderer3D;

class Character : public Component
{
public:
	Character() {}
	~Character()override {}

	virtual void Init()override {}
	virtual void Uninit()override {}
	virtual void Update()override {}

protected:
	virtual void Move() = 0;
	virtual void PointAtoB_AStar();

private:

};