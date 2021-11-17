#pragma once
#include <memory>
#include <vector>
#include <Transform.h>

class Object;

class Component : public std::enable_shared_from_this<Component>
{
protected:

public:
	Component() {}
	virtual ~Component() {}
	virtual void Init() {}
	virtual void Uninit() {}
	virtual void Update() {}

	std::weak_ptr<Object> m_pOwner;
};