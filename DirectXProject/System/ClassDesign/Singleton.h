#pragma once
#include <memory>

template <class T>
class Singleton
{
public:
	static inline T& GetInstance()
	{
		static T Instace;
		return Instace;
	}

public:
	virtual void Initialize() = 0;
	virtual void Finalize() = 0;
protected:
	Singleton() {}
	virtual ~Singleton() {}

private:
	Singleton(const Singleton&) = delete;
	Singleton &operator = (const Singleton&) = delete;
	Singleton &operator = (Singleton&&) = delete;
	Singleton &operator = (const Singleton&&) = delete;
};
