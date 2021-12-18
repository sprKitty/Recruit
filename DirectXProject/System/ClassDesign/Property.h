#pragma once

template<class T>
class Property
{
public:
	Property() {}
	Property(T* p) { ptr = p; }
	~Property() {}

	inline const T& get() { return *ptr; }
	inline void set(const T& param) { *ptr = param; }

private:
	T* ptr;

};
