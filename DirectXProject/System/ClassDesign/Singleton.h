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

protected:
	Singleton() {}
	virtual ~Singleton() {}

private:
	Singleton(const Singleton&) = delete;
	Singleton &operator = (const Singleton&) = delete;
	Singleton &operator = (Singleton&&) = delete;
	Singleton &operator = (const Singleton&&) = delete;
};
