#include <MyMath.h>


DirectX::XMMATRIX MyMath::ConvertMatrix(Vector3 scale, Vector3 rot, Vector3 pos)
{
	return DirectX::XMMATRIX(
		DirectX::XMMatrixScaling(scale.x, scale.y, scale.z)*
		DirectX::XMMatrixRotationX(RAD(rot.x))*
		DirectX::XMMatrixRotationY(RAD(rot.y))*
		DirectX::XMMatrixRotationZ(RAD(rot.z))*
		DirectX::XMMatrixTranslation(pos.x, pos.y, pos.z)
	);
}

Vector2 MyMath::XM2ConvertV2(DirectX::XMFLOAT2 xm2)
{
	return Vector2(xm2.x, xm2.y);
}

DirectX::XMFLOAT2 MyMath::V2ConvertXM2(Vector2 v2)
{
	return DirectX::XMFLOAT2(v2.x, v2.y);
}

Vector3 MyMath::XM3ConvertV3(DirectX::XMFLOAT3 xm3)
{
	return Vector3(xm3.x, xm3.y, xm3.z);
}

DirectX::XMFLOAT3 MyMath::V3ConvertXM3(Vector3 v3)
{
	return DirectX::XMFLOAT3(v3.x, v3.y, v3.z);
}

Vector4 MyMath::XM4ConvertV4(DirectX::XMFLOAT4 xm4)
{
	return Vector4(xm4.x, xm4.y, xm4.z, xm4.w);
}

DirectX::XMFLOAT4 MyMath::V4ConvertXM4(Vector4 v4)
{
	return DirectX::XMFLOAT4(v4.x, v4.y, v4.z, v4.w);
}

Vector3 MyMath::Cross(Vector3* pPos0, Vector3* pPos1)
{
	Vector3 pos0 = *pPos0;
	Vector3 pos1 = *pPos1;
	return Vector3(
		pos0.z * pos1.y - pos0.y * pos1.z,
		pos0.x * pos1.z - pos0.z * pos1.x,
		pos0.x * pos1.y - pos0.y * pos1.x
	);
}

Vector3 MyMath::Normalize(Vector3 v)
{
	float length = Length(&v);
	v /= length;
	return Vector3(v);
}


bool MyMath::TangentandBinormal(const Vertex& v0, const Vertex& v1, const Vertex& v2, Vector3* pTangent, Vector3* pBinormal)
{
	Vector3 CP0[3] =
	{
		Vector3(v0.pos.x,v0.uv.x,v0.uv.y),
		Vector3(v0.pos.y,v0.uv.x,v0.uv.y),
		Vector3(v0.pos.z,v0.uv.x,v0.uv.y),
	};
	Vector3 CP1[3] =
	{
		Vector3(v1.pos.x,v1.uv.x,v1.uv.y),
		Vector3(v1.pos.y,v1.uv.x,v1.uv.y),
		Vector3(v1.pos.z,v1.uv.x,v1.uv.y),
	};
	Vector3 CP2[3] =
	{
		Vector3(v2.pos.x,v2.uv.x,v2.uv.y),
		Vector3(v2.pos.y,v2.uv.x,v2.uv.y),
		Vector3(v2.pos.z,v2.uv.x,v2.uv.y),
	};

	// ïΩñ ÉpÉâÉÅÅ[É^Ç©ÇÁUVé≤ç¿ïWéZèo
	float U[3], V[3];
	for (int i = 0; i < 3; ++i)
	{
		Vector3 V1 = CP1[i] - CP0[i];
		Vector3 V2 = CP2[i] - CP1[i];
		Vector3 ABC;
		ABC.Cross(V1, V2);

		// ílÇ™0ÇÃèÍçáê≥ÇµÇ¢ílÇäiî[Ç∑ÇÈÇ±Ç∆Ç™Ç≈Ç´Ç»Ç¢ÇÃÇ≈âΩÇ‡ÇµÇ»Ç¢
		// Ç®ÇªÇÁÇ≠ÉÇÉfÉãÇ…ñ‚ëËÇ™Ç†ÇÈÅc
		if (ABC.x == 0.0f)
			return false;
		U[i] = -(ABC.y / ABC.x);
		V[i] = -(ABC.z / ABC.x);
	}
	Vector3 vTangent = MyMath::Normalize(Vector3(U[0], U[1], U[2]));
	Vector3 vNormal = MyMath::Normalize(Vector3(V[0], V[1], V[2]));

	// nullptrÇ™ì¸Ç¡ÇƒÇ¢ÇΩèÍçáílÇäiî[ÇµÇ»Ç¢
	if (pTangent)
		*pTangent = vTangent;
	if (pBinormal)
		*pBinormal = vNormal;
	return true;
}

float MyMath::Length(Vector3* pDir)
{
	Vector3 vDir = *pDir;
	float length = sqrtf(vDir.x * vDir.x + vDir.y * vDir.y + vDir.z * vDir.z);
	return length;
}

float MyMath::Dot(Vector3* pPos0, Vector3* pPos1)
{
	Vector3 pos0 = *pPos0;
	Vector3 pos1 = *pPos1;
	return pos0.x * pos1.x + pos0.y * pos1.y + pos0.z * pos1.z;
}

float MyMath::Radian(float x1, float y1, float x2, float y2)
{
	return atan2f(y2 - y1, x2 - x1);
}

void MyMath::RotRadiusCalcPoint(float radius, float angle, float & calc0, float & calc1)
{
	calc0 = -radius * cosf(angle);
	calc1 = radius * sinf(angle);
}
