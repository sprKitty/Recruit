#include <App/Component/Object.h>
#include <System/ClassDesign/Delegate.h>
#include <System/ClassDesign/Manager.h>

class Mouse : public Manager
{
public:
	Mouse() {}
	~Mouse() {}
	
	void Initialize()override;
	void Finalize()override;
	void Update()override;

	/* 
	* @ brief 実行関数をセット
	* @ return[void]
	* @ param[pFunc] DelegateBase template void
	* @	param[pFunc] DelegateBase template const Vector3&
	*/
	void SetExecuteFunc(const std::shared_ptr<DelegateBase<void, const Vector3&> > pFunc);

	void CalcScreentoXZ(DirectX::XMMATRIX mView, DirectX::XMMATRIX mProj);

	inline Vector3& GetWorldPos() 
	{
		return m_vWorldpos;
	}

	inline void SetScreenPos(int x, int y)
	{
		m_vScreenPos.x = static_cast<float>(x) + DEVISIONX * x;
		m_vScreenPos.y = static_cast<float>(y) + DEVISIONY * y;
	}

private:
	Vector3 CalcScreenToWorld(float depthZ, DirectX::XMMATRIX mView, DirectX::XMMATRIX mProj);

private:
	Vector3 m_vWorldpos;
	Vector2 m_vScreenPos;
	std::vector<std::shared_ptr<DelegateBase<void, const Vector3&> > > m_pFunctionList;
};