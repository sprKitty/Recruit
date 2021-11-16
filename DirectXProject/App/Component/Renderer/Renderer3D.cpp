#include <App/Component/Renderer/Renderer3D.h>
#include <App/Component/Object.h>
#include <App/RenderPipeline.h>

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
}


void Renderer3D::Write(const WriteType::kind type)
{
	if (!m_isWriteType[type])return;

	ShaderBuffer::GetInstance().SetColor(DirectX::XMFLOAT4(1, 1, 1, 1));
	Transform t = m_pOwner.lock()->GetTransform();
	DirectX::XMMATRIX mtx, mtxParent;
	if (!m_isBillboard)
	{
		if (!m_pOwner.lock()->GetParent().expired())
		{
			if (m_isParentScale)
				mtxParent = m_pOwner.lock()->GetParent().lock()->GetWorldMatrix();
			//else
				//mtxParent = m_pOwner.lock()->GetParent().lock()->GetWorld();
			if (m_pOwner.lock()->GetParent().lock()->GetParent().expired())
			{
				if (m_isParentScale)
					mtxParent *= m_pOwner.lock()->GetParent().lock()->GetParent().lock()->GetWorldMatrix();
				//else
					//mtxParent *= m_pOwner.lock()->GetParent()->GetParent()->GetWorld();
			}
			mtx = MyMath::ConvertMatrix(t.scale, t.rot, t.pos);
			mtx *= mtxParent;
			ShaderBuffer::GetInstance().SetWorld(mtx);
		}
		else
			ShaderBuffer::GetInstance().SetWorld(m_pOwner.lock()->GetWorldMatrix());
	}
	else
	{
		//DirectX::XMMATRIX camM = Camera::m_pMain->GetView();
		//DirectX::XMMATRIX invCamM = DirectX::XMMatrixInverse(nullptr, camM);
		//DirectX::XMFLOAT4X4 mat;
		//DirectX::XMStoreFloat4x4(&mat, invCamM);
		//mat._41 = 0;
		//mat._42 = 0;
		//mat._43 = 0;
		////mat._11 = 1;
		////mat._12 = 0;
		////mat._13 = 0;
		////mat._21 = 0;
		////mat._22 = 1;
		////mat._23 = 0;
		//mat._31 = 0;
		//mat._32 = 0;
		//mat._33 = 1;
		//invCamM = DirectX::XMLoadFloat4x4(&mat);
		//if (!m_pOwner.lock()->GetParent().expired())
		//{
		//	Transform tP = m_pOwner.lock()->GetParent().lock()->GetTransform();
		//	mtxParent = MyMath::ConvertMatrix(1, 0, tP.pos);
		//	mtx = DirectX::XMMATRIX(
		//		DirectX::XMMatrixScaling(t.scale.x, t.scale.y, 0)*
		//		invCamM*
		//		DirectX::XMMatrixTranslation(t.pos.x, t.pos.y, t.pos.z));
		//	mtx *= mtxParent;
		//	ShaderBuffer::GetInstance().SetWorld(mtx);
		//}
		//else
		//{
		//	mtx = DirectX::XMMATRIX(
		//		DirectX::XMMatrixScaling(t.scale.x, t.scale.y, 1)*
		//		invCamM*
		//		DirectX::XMMatrixTranslation(t.pos.x, t.pos.y, t.pos.z));
		//	ShaderBuffer::GetInstance().SetWorld(mtx);
		//}
	}

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
	Transform t = m_pOwner.lock()->GetTransform();
	DirectX::XMMATRIX mtx, mtxParent;
	if (!m_isBillboard)
	{
		if (!m_pOwner.lock()->GetParent().expired())
		{
			if (m_isParentScale)
				mtxParent = m_pOwner.lock()->GetParent().lock()->GetWorldMatrix();
			//else
				//mtxParent = m_pOwner.lock()->GetParent().lock()->GetWorld();
			if (m_pOwner.lock()->GetParent().lock()->GetParent().expired())
			{
				if (m_isParentScale)
					mtxParent *= m_pOwner.lock()->GetParent().lock()->GetParent().lock()->GetWorldMatrix();
				//else
					//mtxParent *= m_pOwner.lock()->GetParent()->GetParent()->GetWorld();
			}
			mtx = MyMath::ConvertMatrix(t.scale, t.rot, t.pos);
			mtx *= mtxParent;
			ShaderBuffer::GetInstance().SetWorld(mtx);
		}
		else
			ShaderBuffer::GetInstance().SetWorld(m_pOwner.lock()->GetWorldMatrix());
	}
	else
	{
		//DirectX::XMMATRIX camM = Camera::m_pMain->GetView();
		//DirectX::XMMATRIX invCamM = DirectX::XMMatrixInverse(nullptr, camM);
		//DirectX::XMFLOAT4X4 mat;
		//DirectX::XMStoreFloat4x4(&mat, invCamM);
		//mat._41 = 0;
		//mat._42 = 0;
		//mat._43 = 0;
		////mat._11 = 1;
		////mat._12 = 0;
		////mat._13 = 0;
		////mat._21 = 0;
		////mat._22 = 1;
		////mat._23 = 0;
		//mat._31 = 0;
		//mat._32 = 0;
		//mat._33 = 1;
		//invCamM = DirectX::XMLoadFloat4x4(&mat);
		//if (!m_pOwner.lock()->GetParent().expired())
		//{
		//	Transform tP = m_pOwner.lock()->GetParent().lock()->GetTransform();
		//	mtxParent = MyMath::ConvertMatrix(1, 0, tP.pos);
		//	mtx = DirectX::XMMATRIX(
		//		DirectX::XMMatrixScaling(t.scale.x, t.scale.y, 0)*
		//		invCamM*
		//		DirectX::XMMatrixTranslation(t.pos.x, t.pos.y, t.pos.z));
		//	mtx *= mtxParent;
		//	ShaderBuffer::GetInstance().SetWorld(mtx);
		//}
		//else
		//{
		//	mtx = DirectX::XMMATRIX(
		//		DirectX::XMMatrixScaling(t.scale.x, t.scale.y, 1)*
		//		invCamM*
		//		DirectX::XMMatrixTranslation(t.pos.x, t.pos.y, t.pos.z));
		//	ShaderBuffer::GetInstance().SetWorld(mtx);
		//}
	}

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