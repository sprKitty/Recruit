#pragma once
#include <App/Component/Component.h>
#include <App/Component/Image.h>


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