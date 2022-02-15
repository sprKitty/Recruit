#pragma once
//===================================
// class：Vector2
//-----------------------------------
// 制作者：矢橋　嵩琉
//-----------------------------------
// 制作日	2021/03/16
//				 08/06 更新
//===================================
#include <DirectXMath.h>
#include <vector>
#include <list>

class Vector2
{
public:
	Vector2() = default;
	Vector2(float x, float y) :x(x), y(y) {}
	Vector2(float v) :x(v), y(v) {}
	Vector2(const DirectX::XMFLOAT2& val) :x(val.x), y(val.y) {}
	~Vector2(){}

	const Vector2& operator =(const Vector2 &v)
	{
		x = v.x;
		y = v.y;
		return *this;
	}

	const Vector2 operator +(const Vector2 &v) const
	{
		Vector2 a;
		a.x = x + v.x;
		a.y = y + v.y;
		return a;
	}

	const Vector2 operator -(const Vector2 &v) const
	{
		Vector2 a;
		a.x = x - v.x;
		a.y = y - v.y;
		return a;
	}

	const Vector2 operator *(const Vector2 &v) const
	{
		Vector2 a;
		a.x = x * v.x;
		a.y = y * v.y;
		return a;
	}

	const Vector2 operator /(const Vector2 &v) const
	{
		Vector2 a;
		a.x = x / v.x;
		a.y = y / v.y;
		return a;
	}


	Vector2& operator+=(const Vector2 &v)
	{
		x += v.x;
		y += v.y;
		return *this;
	}
	
	Vector2& operator-=(const Vector2 &v)
	{
		x -= v.x;
		y -= v.y;
		return *this;
	}
	
	Vector2& operator*=(const Vector2 &v)
	{
		x *= v.x;
		y *= v.y;
		return *this;
	}
	
	Vector2& operator/=(const Vector2 &v)
	{
		x /= v.x;
		y /= v.y;
		return *this;
	}

	Vector2& Lerp(const Vector2& vStart, const Vector2& vEnd, float fTime)
	{
		Vector2 a = (vEnd - vStart) * fTime;
		x = a.x;
		y = a.y;
		return *this;
	}

	const float Length()
	{
		return sqrtf(x * x + y * y);
	}

	Vector2& Normalize()
	{
		float length = Length();
		*this /= length;
		return *this;
	}

	const Vector2& Abs()
	{
		x = (x < 0) ? x * -1 : x;
		y = (y < 0) ? y * -1 : y;
		return *this;
	}

	DirectX::XMFLOAT2 Convert()
	{
		DirectX::XMFLOAT2 v2;
		v2.x = x;
		v2.y = y;

		return v2;
	}

	Vector2& Convert(const DirectX::XMFLOAT2& v2)
	{
		x = v2.x;
		y = v2.y;

		return *this;
	}

	float x, y;
};






class VectorInt2
{
public:
	VectorInt2() = default;
	VectorInt2(int x, int y) :x(x), y(y) {}
	VectorInt2(int v) :x(v), y(v) {}
	~VectorInt2() {}

	const VectorInt2& operator =(const VectorInt2 &v)
	{
		x = v.x;
		y = v.y;
		return *this;
	}

	const VectorInt2 operator +(const VectorInt2 &v) const
	{
		VectorInt2 a;
		a.x = x + v.x;
		a.y = y + v.y;
		return a;
	}

	const VectorInt2 operator -(const VectorInt2 &v) const
	{
		VectorInt2 a;
		a.x = x - v.x;
		a.y = y - v.y;
		return a;
	}

	const VectorInt2 operator *(const VectorInt2 &v) const
	{
		VectorInt2 a;
		a.x = x * v.x;
		a.y = y * v.y;
		return a;
	}

	const VectorInt2 operator /(const VectorInt2 &v) const
	{
		VectorInt2 a;
		a.x = x / v.x;
		a.y = y / v.y;
		return a;
	}


	VectorInt2& operator+=(const VectorInt2 &v)
	{
		x += v.x;
		y += v.y;
		return *this;
	}

	VectorInt2& operator-=(const VectorInt2 &v)
	{
		x -= v.x;
		y -= v.y;
		return *this;
	}

	VectorInt2& operator*=(const VectorInt2 &v)
	{
		x *= v.x;
		y *= v.y;
		return *this;
	}

	VectorInt2& operator/=(const VectorInt2 &v)
	{
		x /= v.x;
		y /= v.y;
		return *this;
	}

	int x, y;
};







//===================================
// class：Vector3
//-----------------------------------
// 制作者：矢橋　嵩琉
//-----------------------------------
// 制作日	2021/03/15
//				 08/06 更新
//===================================
class Vector3
{
public:
	Vector3() = default;
	Vector3(float x, float y, float z) :x(x), y(y), z(z){}
	Vector3(const DirectX::XMFLOAT3& val) :x(val.x), y(val.y), z(val.z) {}
	Vector3(float v) :x(v), y(v), z(v) {}
	~Vector3() {}

	const Vector3 &operator =(const Vector3 &v)
	{
		x = v.x;
		y = v.y;
		z = v.z;
		return *this;
	}

	const Vector3 operator +(const Vector3 &v) const
	{
		Vector3 a;
		a.x = x + v.x;
		a.y = y + v.y;
		a.z = z + v.z;
		return a;
	}

	const Vector3 operator -(const Vector3 &v) const
	{
		Vector3 a;
		a.x = x - v.x;
		a.y = y - v.y;
		a.z = z - v.z;
		return a;
	}

	const Vector3 operator *(const Vector3 &v) const
	{
		Vector3 a;
		a.x = x * v.x;
		a.y = y * v.y;
		a.z = z * v.z;
		return a;
	}

	const Vector3 operator /(const Vector3 &v) const
	{
		Vector3 a;
		a.x = x / v.x;
		a.y = y / v.y;
		a.z = z / v.z;
		return a;
	}

	Vector3 &operator+=(const Vector3 &v)
	{
		x += v.x;
		y += v.y;
		z += v.z;
		return *this;
	}

	Vector3 &operator-=(const Vector3 &v)
	{
		x -= v.x;
		y -= v.y;
		z -= v.z;
		return *this;
	}

	Vector3 &operator*=(const Vector3 &v)
	{
		x *= v.x;
		y *= v.y;
		z *= v.z;
		return *this;
	}

	Vector3 &operator/=(const Vector3 &v)
	{
		x /= v.x;
		y /= v.y;
		z /= v.z;
		return *this;
	}

	Vector3& Cross(const Vector3& vA, const Vector3& vB)
	{
		Vector3 a;
		a.x = vA.z * vB.y - vA.y * vB.z;
		a.y = vA.x * vB.z - vA.z * vB.x;
		a.z = vA.x * vB.y - vA.y * vB.x;
		*this = a;
		return *this;
	}

	Vector3& Lerp(const Vector3& vStart, const Vector3& vEnd, float fTime)
	{
		Vector3 a = (vEnd - vStart) * fTime;
		x = a.x;
		y = a.y;
		z = a.z;
		return *this;
	}

	const float Length()
	{
		return sqrtf(x * x + y * y + z * z);
	}

	Vector3& Normalize()
	{
		float length = Length();
		*this /= length;
		return *this;
	}

	const float Dot(const Vector3& vB)
	{
		return x * vB.x + y * vB.y + z * vB.z;
	}

	const Vector3& Abs()
	{
		x = (x < 0) ? x * -1 : x;
		y = (y < 0) ? y * -1 : y;
		z = (z < 0) ? z * -1 : z;
		return *this;
	}

	DirectX::XMFLOAT3 Convert()
	{
		DirectX::XMFLOAT3 v3;
		v3.x = x;
		v3.y = y;
		v3.z = z;

		return v3;
	}

	Vector3& Convert(const DirectX::XMFLOAT3& v3)
	{
		x = v3.x;
		y = v3.y;
		z = v3.z;

		return *this;
	}

	float x, y, z;
};


class VectorInt3
{
public:
	VectorInt3() = default;
	VectorInt3(int x, int y, int z) :x(x), y(y), z(z) {}
	VectorInt3(int v) :x(v), y(v), z(v) {}
	~VectorInt3() {}

	const VectorInt3 &operator =(const VectorInt3 &v)
	{
		x = v.x;
		y = v.y;
		z = v.z;
		return *this;
	}

	const VectorInt3 operator +(const VectorInt3 &v) const
	{
		VectorInt3 a;
		a.x = x + v.x;
		a.y = y + v.y;
		a.z = z + v.z;
		return a;
	}

	const VectorInt3 operator -(const VectorInt3 &v) const
	{
		VectorInt3 a;
		a.x = x - v.x;
		a.y = y - v.y;
		a.z = z - v.z;
		return a;
	}

	const VectorInt3 operator *(const VectorInt3 &v) const
	{
		VectorInt3 a;
		a.x = x * v.x;
		a.y = y * v.y;
		a.z = z * v.z;
		return a;
	}

	const VectorInt3 operator /(const VectorInt3 &v) const
	{
		VectorInt3 a;
		a.x = x / v.x;
		a.y = y / v.y;
		a.z = z / v.z;
		return a;
	}

	VectorInt3 &operator+=(const VectorInt3 &v)
	{
		x += v.x;
		y += v.y;
		z += v.z;
		return *this;
	}

	VectorInt3 &operator-=(const VectorInt3 &v)
	{
		x -= v.x;
		y -= v.y;
		z -= v.z;
		return *this;
	}

	VectorInt3 &operator*=(const VectorInt3 &v)
	{
		x *= v.x;
		y *= v.y;
		z *= v.z;
		return *this;
	}

	VectorInt3 &operator/=(const VectorInt3 &v)
	{
		x /= v.x;
		y /= v.y;
		z /= v.z;
		return *this;
	}

	int x, y, z;
};



//===================================
// class：Vector4
//-----------------------------------
// 制作者：矢橋　嵩琉
//-----------------------------------
// 制作日	2021/06/08
//				 08/06 更新
//===================================
class Vector4
{
public:
	Vector4() = default;
	Vector4(float x, float y, float z, float w) :x(x), y(y), z(z), w(w) {}
	Vector4(const DirectX::XMFLOAT4& val) :x(val.x), y(val.y), z(val.z), w(val.w) {}
	Vector4(float v) :x(v), y(v), z(v), w(v) {}
	~Vector4() {}
	
	const Vector4 &operator =(const Vector4 &v)
	{
		x = v.x;
		y = v.y;
		z = v.z;
		w = v.w;
		return *this;
	}

	const Vector4 operator +(const Vector4 &v) const
	{
		Vector4 a;
		a.x = x + v.x;
		a.y = y + v.y;
		a.z = z + v.z;
		a.w = w + v.w;
		return a;
	}

	const Vector4 operator -(const Vector4 &v) const
	{
		Vector4 a;
		a.x = x - v.x;
		a.y = y - v.y;
		a.z = z - v.z;
		a.w = w - v.w;
		return a;
	}

	const Vector4 operator *(const Vector4 &v) const
	{
		Vector4 a;
		a.x = x * v.x;
		a.y = y * v.y;
		a.z = z * v.z;
		a.w = w * v.w;
		return a;
	}

	const Vector4 operator /(const Vector4 &v) const
	{
		Vector4 a;
		a.x = x / v.x;
		a.y = y / v.y;
		a.z = z / v.z;
		a.w = w / v.w;
		return a;
	}

	Vector4 &operator+=(const Vector4 &v)
	{
		x += v.x;
		y += v.y;
		z += v.z;
		w += v.w;
		return *this;
	}

	Vector4 &operator-=(const Vector4 &v)
	{
		x -= v.x;
		y -= v.y;
		z -= v.z;
		w -= v.w;
		return *this;
	}

	Vector4 &operator*=(const Vector4 &v)
	{
		x *= v.x;
		y *= v.y;
		z *= v.z;
		w *= v.w;
		return *this;
	}

	Vector4 &operator/=(const Vector4 &v)
	{
		x /= v.x;
		y /= v.y;
		z /= v.z;
		w /= v.w;
		return *this;
	}

	Vector4& Lerp(const Vector4& vStart, const Vector4& vEnd, float fTime)
	{
		Vector4 a = (vEnd - vStart) * fTime;
		x = a.x;
		y = a.y;
		z = a.z;
		w = a.w;
		return *this;
	}

	const float Length()
	{
		return sqrtf(x * x + y * y + z * z + w * w);
	}

	Vector4& Normalize()
	{
		float length = Length();
		*this /= length;
		return *this;
	}

	DirectX::XMFLOAT4 Convert()
	{
		DirectX::XMFLOAT4 v4;
		v4.x = x;
		v4.y = y;
		v4.z = z;
		v4.w = w;

		return v4;
	}

	Vector4& Convert(const DirectX::XMFLOAT4& v4)
	{
		x = v4.x;
		y = v4.y;
		z = v4.z;
		w = v4.w;

		return *this;
	}

	float x, y, z, w;
};





class VectorInt4
{
public:
	VectorInt4() = default;
	VectorInt4(int x, int y, int z, int w) :x(x), y(y), z(z), w(w) {}
	VectorInt4(int v) :x(v), y(v), z(v), w(v) {}
	~VectorInt4() {}

	const VectorInt4 &operator =(const VectorInt4 &v)
	{
		x = v.x;
		y = v.y;
		z = v.z;
		w = v.w;
		return *this;
	}

	const VectorInt4 operator +(const VectorInt4 &v) const
	{
		VectorInt4 a;
		a.x = x + v.x;
		a.y = y + v.y;
		a.z = z + v.z;
		a.w = w + v.w;
		return a;
	}

	const VectorInt4 operator -(const VectorInt4 &v) const
	{
		VectorInt4 a;
		a.x = x - v.x;
		a.y = y - v.y;
		a.z = z - v.z;
		a.w = w - v.w;
		return a;
	}

	const VectorInt4 operator *(const VectorInt4 &v) const
	{
		VectorInt4 a;
		a.x = x * v.x;
		a.y = y * v.y;
		a.z = z * v.z;
		a.w = w * v.w;
		return a;
	}

	const VectorInt4 operator /(const VectorInt4 &v) const
	{
		VectorInt4 a;
		a.x = x / v.x;
		a.y = y / v.y;
		a.z = z / v.z;
		a.w = w / v.w;
		return a;
	}

	VectorInt4 &operator+=(const VectorInt4 &v)
	{
		x += v.x;
		y += v.y;
		z += v.z;
		w += v.w;
		return *this;
	}

	VectorInt4 &operator-=(const VectorInt4 &v)
	{
		x -= v.x;
		y -= v.y;
		z -= v.z;
		w -= v.w;
		return *this;
	}

	VectorInt4 &operator*=(const VectorInt4 &v)
	{
		x *= v.x;
		y *= v.y;
		z *= v.z;
		w *= v.w;
		return *this;
	}

	VectorInt4 &operator/=(const VectorInt4 &v)
	{
		x /= v.x;
		y /= v.y;
		z /= v.z;
		w /= v.w;
		return *this;
	}

	int x, y, z, w;
};
