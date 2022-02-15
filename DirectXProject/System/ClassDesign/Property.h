#pragma once

/*
* @brief ゲットセットが用意してある変数クラス
*/
template<class T>
class Property
{
public:
	Property() {}
	~Property() {}

	inline void set(const T& val) { m_val = val; }
	inline const T& get() { return m_val; }

private:
	T m_val;
};
