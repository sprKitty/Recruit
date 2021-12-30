#pragma once
#include <App/Component/Component.h>


class Terrain : public Component
{
public:
	Terrain() {}
	~Terrain()override {}

	void Init()override;
	void Uninit()override;
	void Update()override;

private:

};