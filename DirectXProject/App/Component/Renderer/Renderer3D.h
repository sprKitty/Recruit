#pragma once
#include "Renderer.h"
#include <App/OBJModel.h>
#include <System/DrawBuffer.h>


class Transform;
class Mesh;
class Image;
class Instancing;

class Renderer3D : public Renderer
{
public:
	Renderer3D() {}
	~Renderer3D() override {}

	void Init()override;
	void Uninit()override;
	void Update()override;
	
	void Write(const std::weak_ptr<ShaderBuffer>& pBuf, const WriteType::kind type)override;
	void Draw(const std::weak_ptr<ShaderBuffer>& pBuf, const DrawType::kind type)override;

	void CalcFrustumState(const std::weak_ptr<ViewPoint>& pVP)override;
	
	void SetMainImage(const std::string str);
	void SetBumpImage(const std::string str);

	inline void SetEmissiveColor(const Vector4& color) { m_vEmissive = color; }
	inline void EnableWrite(const WriteType::kind type) { m_isWriteType[type] = true; }
	inline void EnableDraw(const DrawType::kind type) { m_isDrawType[type] = true; }
	inline void DisableWrite(const WriteType::kind type) { m_isWriteType[type] = false; }
	inline void DisableDraw(const DrawType::kind type) { m_isDrawType[type] = false; }

private:
	std::vector<bool> m_isWriteType;
	std::vector<bool> m_isDrawType;
	std::weak_ptr<Transform> m_pTransform;
	std::weak_ptr<Mesh> m_pMesh;
	std::weak_ptr<Instancing> m_pInstancing;
	std::shared_ptr<Image> m_pMainImage;
	std::shared_ptr<Image> m_pBumpImage;
	Vector4 m_vEmissive;
};