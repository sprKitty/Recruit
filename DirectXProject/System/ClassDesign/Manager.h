#pragma once

class Manager
{
public:
	Manager(){}
	virtual ~Manager() {}

	virtual void Initialize() = 0;
	virtual void Finalize() = 0;
	virtual void Update() = 0;
private:
	Manager(const Manager&) = delete;
	Manager &operator = (const Manager&) = delete;
	Manager &operator = (Manager&&) = delete;
	Manager &operator = (const Manager&&) = delete;
};