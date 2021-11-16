#pragma once
#include <memory>

class Light;
class Object;

class ParallelLightMgr
{
public:

enum class Type
{
	T_NEAR,
	T_MEDIUM,
	T_FAR,
	T_MAX,
};
public:
	ParallelLightMgr();
	~ParallelLightMgr();

	void Init();
	void Update();
	void Bind(Type type = Type::T_MAX);

private:
	std::shared_ptr<Light> m_pNearLight;
	std::shared_ptr<Light> m_pMediumLight;
	std::shared_ptr<Light> m_pFarLight;
private:
	void Release();
};