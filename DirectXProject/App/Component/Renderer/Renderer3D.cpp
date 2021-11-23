#include <App/Component/Renderer/Renderer3D.h>
#include <App/Component/Object.h>
#include <App/RenderPipeline.h>
#include <Transform.h>
#include <System/Input.h>
#include <System/Geometory.h>


Renderer3D::Renderer3D()
	: m_pMMDModel(nullptr)
	, m_pOBJModel(nullptr)
	, m_isBillboard(false)
	, m_isParentScale(true)
{
}


Renderer3D::~Renderer3D()
{
}

void Renderer3D::Init()
{
	m_pTransform = m_pOwner.lock()->GetComponent<Transform>();
	RenderPipeline::GetInstance().AddRenderer(weak_from_this());
	m_isWriteType.resize(WriteType::MAX);
	for (bool itr : m_isWriteType)
	{
		itr = false;
	}
	
	m_isDrawType.resize(DrawType::MAX);
	for (bool itr : m_isDrawType)
	{
		itr = false;
	}
}

void Renderer3D::Uninit()
{
	if (m_pOBJModel)
	{
		delete m_pOBJModel;
		m_pOBJModel = nullptr;
	}
	if (m_pMMDModel)
	{
		delete m_pMMDModel;
		m_pMMDModel = nullptr;
	}
	RenderPipeline::GetInstance().ReleaseRenderer(weak_from_this());
}


void Renderer3D::Update()
{
	//DirectX::XMMATRIX mtx, mtxParent;
	//mtx = m_pTransform.lock()->GetWorldMatrix();
	//if (!m_pOwner.lock()->GetParent().expired())
	//{
	//	//if (m_isParentScale)
	//		//mtxParent = m_pOwner.lock()->GetParent().lock()->GetWorldMatrix();
	//	//else
	//		//mtxParent = m_pOwner.lock()->GetParent().lock()->GetWorld();
	//	if (m_pOwner.lock()->GetParent().lock()->GetParent().expired())
	//	{
	//		//if (m_isParentScale)
	//			//mtxParent *= m_pOwner.lock()->GetParent().lock()->GetParent().lock()->GetWorldMatrix();
	//		//else
	//			//mtxParent *= m_pOwner.lock()->GetParent()->GetParent()->GetWorld();
	//	}
	//	mtx = t.GetMatrix();
	//	mtx *= mtxParent;
	//	m_pOwner.lock()->SetWorldMatrix(mtx);
	//}
	//else
	//	m_pOwner.lock()->SetWorldMatrix(mtx);
}


void Renderer3D::Write(const WriteType::kind type)
{
	if (!m_isWriteType[type])return;

	ShaderBuffer::GetInstance().SetColor(DirectX::XMFLOAT4(1, 1, 1, 1));
	
	ShaderBuffer::GetInstance().SetWorld(m_pTransform.lock()->GetWorldMatrix());


	if (m_pMMDModel)
	{
		m_pMMDModel->Draw();
	}
	else if (m_pOBJModel)
	{
		m_pOBJModel->Draw();
	}
	else
	{
		Geometory::GetInstance().DrawCube();
	}
}

void Renderer3D::Draw(const DrawType::kind type)
{
	if (!m_isDrawType[type])return;

	ShaderBuffer::GetInstance().SetColor(DirectX::XMFLOAT4(1, 1, 1, 1));

	ShaderBuffer::GetInstance().SetWorld(m_pTransform.lock()->GetWorldMatrix());

	if (m_pMMDModel)
	{
		m_pMMDModel->Draw();
	}
	else if (m_pOBJModel)
	{
		m_pOBJModel->Draw();
	}
	else
	{
		Geometory::GetInstance().DrawCube();
	}
}


void Renderer3D::SetModel(const char * fileName)
{
	if (strstr(fileName, ".pmx")/* && !m_pFBDirectX::XModel*/ && !m_pOBJModel)
	{
		m_pMMDModel = new MMDModel();
		m_pMMDModel->Create(fileName);
		return;
	}
	if (strstr(fileName, ".obj")/* && !m_pFBDirectX::XModel*/ && !m_pMMDModel)
	{
		m_pOBJModel = new OBJModel();
		m_pOBJModel->Create(fileName);
	}
}