#include <MyMath.h>


DirectX::XMMATRIX MyMath::ConvertMatrix(Vector3 scale, Vector3 rot, Vector3 pos)
{
	return DirectX::XMMATRIX(
		DirectX::XMMatrixScaling(scale.x, scale.y, scale.z)*
		DirectX::XMMatrixRotationX(DirectX::XMConvertToRadians(rot.x))*
		DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(rot.y))*
		DirectX::XMMatrixRotationZ(DirectX::XMConvertToRadians(rot.z))*
		DirectX::XMMatrixTranslation(pos.x, pos.y, pos.z)
	);
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

	// 平面パラメータからUV軸座標算出
	float U[3], V[3];
	for (int i = 0; i < 3; ++i)
	{
		Vector3 V1 = CP1[i] - CP0[i];
		Vector3 V2 = CP2[i] - CP1[i];
		Vector3 ABC;
		ABC.Cross(V1, V2);

		// 値が0の場合正しい値を格納することができないので何もしない
		// おそらくモデルに問題がある…
		if (ABC.x == 0.0f)
			return false;
		U[i] = -(ABC.y / ABC.x);
		V[i] = -(ABC.z / ABC.x);
	}
	Vector3 vTangent(U[0], U[1], U[2]);
	Vector3 vNormal(V[0], V[1], V[2]);
	vNormal.Normalize();
	vTangent.Normalize();

	// nullptrが入っていた場合値を格納しない
	if (pTangent)
		*pTangent = vTangent;
	if (pBinormal)
		*pBinormal = vNormal;
	return true;
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

const float MyMath::Lerp(const float fTime, const float fStart, const float fEnd)
{
	return (fEnd - fStart) * fTime;
}