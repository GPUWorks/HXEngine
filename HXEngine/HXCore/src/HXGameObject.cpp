#include "..\include\HXGameObject.h"
#include "HXMesh.h"
#include "HXCore.h"
#include "HXMath.h"
#include "HXRenderSystem.h"
#include "HXLoadConfigPrefab.h"
#include "HXResourceManager.h"
#include "HXLoadConfigModel.h"
#include "HXRoot.h"
#include "HXSceneManager.h"

namespace HX3D
{
	HXGameObject::HXGameObject(HXGameObject* pFather, HXRenderSystem* pRenderSystem)
	{
		m_strName = "";
		m_pMesh = NULL;
		transform = pRenderSystem->CreateTransform();
		transform->gameObject = this;
		if (pFather)
		{
			transform->parent = pFather->GetTransform();
		}
		m_bActivity = true;
		m_bStatic = true;
		layer = L_DEFAULT;
	}

	HXGameObject::~HXGameObject()
	{
		if (m_pMesh)
		{
			delete m_pMesh;
			m_pMesh = NULL;
		}
		for (std::vector<HXITransform*>::iterator itr = transform->GetChildren().begin(); itr != transform->GetChildren().end(); ++itr)
		{
			delete (*itr)->gameObject;
		}
		transform->GetChildren().clear();
		if (transform)
		{
			delete transform;
			transform = NULL;
		}
	}

	void HXGameObject::Initialize(HXGameObjectInfo* gameobjectinfo)
	{
		// ��strModelFile = ""ʱ����������renderable��gameobject�����縸�����gameobject
		HXModelInfo* pModelInfo = HXResourceManager::GetInstance()->GetModelInfo(gameobjectinfo->strModelFile);
		if (pModelInfo)
		{
			// ����FBX
			HXMesh* pMesh = HXResourceManager::GetInstance()->GetMesh(pModelInfo->m_strMeshFile, pModelInfo->m_strAnimFile);
			if (pMesh)
			{
				// ���ز���
				int nMatCount = 0;
				for (std::vector<std::string>::iterator itr = pModelInfo->m_vctSubMeshMat.begin(); itr != pModelInfo->m_vctSubMeshMat.end(); ++itr)
				{
					HXMaterial* pMat = HXResourceManager::GetInstance()->GetMaterial(*itr);
					if (NULL == pMat)
					{
						// ��������ڸò�������ʹ�÷�ɫ����
						*itr = "./prefab/_Material/Error/Error.material";
						HXResourceManager::GetInstance()->GetMaterial(*itr);
					}
					++nMatCount;
				}
				// ���û���ʣ������Ĭ�ϲ���
				if (nMatCount == 0)
				{
					pModelInfo->m_vctSubMeshMat.push_back("./prefab/_Material/Error/Error.material");
					HXResourceManager::GetInstance()->GetMaterial("./prefab/_Material/Error/Error.material");
				}

				// �������ʵ�SubMesh
				int nSubMeshIndex = 0;
				for (std::vector<HXSubMesh*>::iterator itr = pMesh->subMeshList.begin(); itr != pMesh->subMeshList.end(); ++itr)
				{
					if (nSubMeshIndex < nMatCount)
					{
						pMesh->subMeshList[nSubMeshIndex]->materialName = pModelInfo->m_vctSubMeshMat[nSubMeshIndex];
					}
					else
					{
						// ��������������ڲ��������������������ʹ�õ�һ������
						pMesh->subMeshList[nSubMeshIndex]->materialName = pModelInfo->m_vctSubMeshMat[0];
					}
					// �����Ƿ�Ͷ����Ӱ ����SetCastShadow(gameobjectinfo->bCastShadow);������
					// (*itr)->IsCastShadow = gameobjectinfo->bCastShadow;
					++nSubMeshIndex;
				}
				m_pMesh = pMesh->Clone(HXRoot::GetInstance()->GetRenderSystem());
				m_pMesh->PlayDefaultAnimation();
			}
			else
			{
				std::cout << pModelInfo->m_strMeshFile << " not exist" << std::endl;
				return;
			}
		}

		m_strName = gameobjectinfo->strGameObjName;
		m_strModelFile = gameobjectinfo->strModelFile;
		SetCastShadow(gameobjectinfo->bCastShadow);
		SetActivity(gameobjectinfo->bActivity);
		SetStatic(gameobjectinfo->bStatic);
		SetLayer(gameobjectinfo->layer);

		if (NULL == transform->parent)
		{
			transform->parent = HXSceneManager::GetInstance()->GetGameObjectTreeRoot()->transform;
		}
		transform->parent->AddChild(transform);
		transform->SetLocalScale(gameobjectinfo->scale);
		transform->SetLocalRotation(gameobjectinfo->rotation);
		transform->SetLocalPosition(gameobjectinfo->position);

		transform->CaculateModelMatrix(transform->parent->mGlobalModelMatrix);
	}

	void HXGameObject::Update()
	{
		if (!m_bActivity)
		{
			return;
		}
		if (!m_bStatic)
		{
			if (transform->parent)
			{
				transform->CaculateModelMatrix(transform->parent->mGlobalModelMatrix);
			}
			else
			{
				transform->CaculateModelMatrix();
			}
		}
		if (m_pMesh)
		{
			m_pMesh->UpdateAnimation();
		}

		for (std::vector<HXITransform*>::iterator itr = transform->GetChildren().begin(); itr != transform->GetChildren().end(); ++itr)
		{
			(*itr)->gameObject->Update();
		}
	}

	HXMesh* HXGameObject::GetMesh()
	{
		return m_pMesh;
	}

	//HXGameObject* HXGameObject::GetFather()
	//{
	//	return transform->parent->gameObject;
	//}

	//void HXGameObject::SetFather(HXGameObject* father)
	//{
	//	transform->parent = father->transform;
	//}

	//void HXGameObject::AddChild(HXGameObject* child)
	//{
	//	transform->AddChild(child->transform);
	//}

	HXITransform* HXGameObject::GetTransform()
	{
		return transform;
	}

	bool HXGameObject::GetActivity() const
	{
		return m_bActivity;
	}

	void HXGameObject::SetActivity(bool activity)
	{
		m_bActivity = activity;
		HXSceneManager::GetInstance()->UpdateRenderableQueue();
	}

	bool HXGameObject::GetCastShadow() const
	{
		return m_bCastShadow;
	}

	void HXGameObject::SetCastShadow(bool bCastShadow)
	{
		m_bCastShadow = bCastShadow;
		if (m_pMesh)
		{
			for (std::vector<HXSubMesh*>::iterator itr = m_pMesh->subMeshList.begin(); itr != m_pMesh->subMeshList.end(); ++itr)
			{
				// �����Ƿ�Ͷ����Ӱ
				(*itr)->IsCastShadow = bCastShadow;
			}
		}
	}

	void HXGameObject::SetCastShadowRecurve(bool bCastShadow)
	{
		SetCastShadow(bCastShadow);
		for (std::vector<HXITransform*>::iterator itr = transform->GetChildren().begin(); itr != transform->GetChildren().end(); ++itr)
		{
			(*itr)->gameObject->SetCastShadowRecurve(bCastShadow);
		}
	}

	bool HXGameObject::GetStatic() const
	{
		return m_bStatic;
	}

	void HXGameObject::SetStatic(bool bStatic)
	{
		m_bStatic = bStatic;
	}

	void HXGameObject::SetStaticRecurve(bool bStatic)
	{
		SetStatic(bStatic);
		for (std::vector<HXITransform*>::iterator itr = transform->GetChildren().begin(); itr != transform->GetChildren().end(); ++itr)
		{
			(*itr)->gameObject->SetStaticRecurve(bStatic);
		}
	}

	ELayer HXGameObject::GetLayer() const
	{
		return layer;
	}

	void HXGameObject::SetLayer(ELayer eLayer)
	{
		layer = eLayer;
		if (m_pMesh)
		{
			for (std::vector<HXSubMesh*>::iterator itr = m_pMesh->subMeshList.begin(); itr != m_pMesh->subMeshList.end(); ++itr)
			{
				// �����Ƿ�Ͷ����Ӱ
				(*itr)->layer = eLayer;
			}
		}
	}

	void HXGameObject::SetLayerRecurve(ELayer eLayer)
	{
		SetLayer(eLayer);
		for (std::vector<HXITransform*>::iterator itr = transform->GetChildren().begin(); itr != transform->GetChildren().end(); ++itr)
		{
			(*itr)->gameObject->SetLayerRecurve(eLayer);
		}
	}
}


