#pragma once
#include <System/ClassDesign/Singleton.h>
#include <App/Component/Renderer/Renderer.h>
#include <App/ViewPoint/Camera.h>
#include <App/ViewPoint/Light.h>
#include <Defines.h>


class MultiPass;
class ShaderBuffer;

class RenderPipeline : public Singleton<RenderPipeline>
{
public:
	friend class Singleton<RenderPipeline>;
	
	static const int KAWASE_BlOOM_NUM = 5;

	using KAWASE_BlOOM_VP = std::weak_ptr<ViewPoint>[KAWASE_BlOOM_NUM];

public:
	
	void Initialize()override;
	void Finalize()override;
	
	
	void DrawShadowMap(const std::weak_ptr<Light> pLight);

	void DrawWaterReflection(const std::weak_ptr<Camera> pCamera, const std::weak_ptr<Light> pLight, const std::weak_ptr<ViewPoint> pDOSVP);

	void Draw(const std::weak_ptr<Camera> pCamera, const std::weak_ptr<Light> pLight, const std::weak_ptr<ViewPoint> pDOSVP, const std::weak_ptr<ViewPoint> pWaterRefVP);

	void PlayGaussianBlurX(const std::weak_ptr<Camera> pCamera, const std::weak_ptr<ViewPoint> pVP, const float fDeviation = 2.0f, const int number = 0);
	
	void PlayGaussianBlurY(const std::weak_ptr<Camera> pCamera, const std::weak_ptr<ViewPoint> pVP, const float fDeviation = 2.0f, const int number = 0);

	void PlayKawaseBloom(const std::weak_ptr<Camera> pCamera, KAWASE_BlOOM_VP pVPList);

	void DrawDOFTexture(const std::weak_ptr<Camera> pCamera, const std::weak_ptr<ViewPoint> pVPDMain, const std::weak_ptr<ViewPoint> pVPDepth, const std::weak_ptr<ViewPoint> pVPBlur);

	
	void DrawUI(const std::weak_ptr<Camera> pCamera);

	/*************************************************
	* @brief レンダーパイプラインにオブジェクトを追加
	* @param[Renderer3D_PTR&] pRenderer3D 描くオブジェクト
	*************************************************/
	void AddRenderer(const std::weak_ptr<Component>& pComponent);

	/*************************************************
	* @brief リストに登録されているオブジェクトを消す
	* @param[Renderer3D_PTR&] pRenderer3D 消すオブジェクトのポインタ
	*************************************************/	
	void ReleaseRenderer(const std::weak_ptr<Component>& pComponent);

	inline void SetShaderBuffer(const std::weak_ptr<ShaderBuffer> ptr) { m_pShaderBuffer = ptr; }

protected:
	RenderPipeline() {}
	~RenderPipeline() override{}

private:
	void CallWrite(const WriteStep::kind type, const std::weak_ptr<ViewPoint>& pVP);
	void CallDraw(const DrawStep::kind type, const std::weak_ptr<ViewPoint>& pVP);

private:
	weak_ptr_list<Renderer> m_pDrawList;
	std::weak_ptr<ShaderBuffer> m_pShaderBuffer;
};

