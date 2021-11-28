#pragma once
#include <memory>
#include <vector>

class Object;

class Component : public std::enable_shared_from_this<Component>
{
protected:

public:
	Component() {}
	virtual ~Component() {}
	virtual void Reset() {}
	virtual void Init() {}
	virtual void Uninit() {}
	virtual void Update() {}

	std::weak_ptr<Object> m_pOwner;
};