#pragma once
#include <System/ClassDesign/Singleton.h>
#include <App/Component/Renderer/Renderer.h>
#include <Defines.h>

class RenderTarget;
class Camera;
class Light;


class RenderPipeline : public Singleton<RenderPipeline>
{
public:
	friend class Singleton<RenderPipeline>;

private:
	using RENDERER_PTR = std::weak_ptr<Renderer>;

public:
	
	void Initialize();
	void Finalize();
	
	/*************************************************
	* @brief リストに登録されているオブジェクトを消す
	* @param[Renderer3D_PTR&] pRenderer3D 消すオブジェクトのポインタ
	*************************************************/	
	void ReleaseRenderer(const std::weak_ptr<Component>& pComponent);
	

	/*************************************************
	* @brief レンダーターゲットのどこに書くか
	* @param[WriteType] typeW 書き込みタイプ
	*************************************************/
	void Write(const WriteType::kind& typeW);


	/*************************************************
	* @brief どういった感じでに描くか
	* @param[DrawType] typeD 描き方タイプ
	*************************************************/
	void Draw(const DrawType::kind& typeD);
	

	/*************************************************
	* @brief レンダーパイプラインにオブジェクトを追加
	* @param[Renderer3D_PTR&] pRenderer3D 描くオブジェクト
	*************************************************/
	inline void AddRenderer(const std::weak_ptr<Component>& pComponent)
	{
		std::weak_ptr<Renderer> pRenderer = std::dynamic_pointer_cast<Renderer>(pComponent.lock());
		m_pDraws.push_back(pRenderer);
	}

	void SetCamera(const std::weak_ptr<Camera> pCam)
	{
		m_pRentCamera = pCam;
	}

	void SetLight(const std::weak_ptr<Light> pLight)
	{
		m_pRentLight = pLight;
	}

protected:
	RenderPipeline() {}
	~RenderPipeline() override{}

private:

	void Call(WriteType::kind typeW, DrawType::kind typeD);

private:
	std::vector<RENDERER_PTR> m_pDraws;
	std::vector<std::shared_ptr<RenderTarget> > m_pRenderTargets;

	std::weak_ptr<Camera> m_pRentCamera;
	std::weak_ptr<Light> m_pRentLight;
};

