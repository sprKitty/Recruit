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

const Vector3 MyMath::CalcTangent(const Vector3 & pos0, const Vector2 & uv0, const Vector3 & pos1, const Vector2 & uv1, const Vector3 & pos2, const Vector2 & uv2)
{
	Vector3 v0 = pos1 - pos0;
	Vector3 v1 = pos2 - pos0;
	Vector2 t0 = uv1 - uv0;
	Vector2 t1 = uv2 - uv0;

	float t = t0.x * t1.y - t1.x * t0.y;

	Vector3 tangent(
		(t1.y * v0.x - t0.y * v1.x) / t,
		(t1.y * v0.y - t0.y * v1.y) / t,
		(t1.y * v0.z - t0.y * v1.z) / t
	);
	tangent.Normalize();
	return tangent;
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