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
		m_pFather = pFather;
		m_pTransform = pRenderSystem->CreateTransform();
	}

	HXGameObject::~HXGameObject()
	{
		if (m_pMesh)
		{
			delete m_pMesh;
			m_pMesh = NULL;
		}
		if (m_pTransform)
		{
			delete m_pTransform;
			m_pTransform = NULL;
		}
		for (std::vector<HXGameObject*>::iterator itr = vctChildren.begin(); itr != vctChildren.end(); ++itr)
		{
			delete *itr;
		}
		vctChildren.clear();
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
		m_nRenderQueue = gameobjectinfo->nPriority;
		SetCastShadow(gameobjectinfo->bCastShadow);
		SetActivity(gameobjectinfo->bActivity);

		if (NULL == m_pFather)
		{
			m_pFather = HXSceneManager::GetInstance()->GetGameObjectTreeRoot();
		}
		m_pFather->AddChild(this);
		m_pTransform->SetScale(gameobjectinfo->scale);
		m_pTransform->SetRotation(gameobjectinfo->rotation);
		m_pTransform->SetPosition(gameobjectinfo->position);

		m_pTransform->CaculateModelMatrix(m_pFather->m_pTransform->mCurModelMatrix);
	}

	void HXGameObject::Update()
	{
		/*if (m_pFather)
		{
			m_pTransform->CaculateModelMatrix(m_pFather->m_pTransform->mCurModelMatrix);
		}
		else
		{
			m_pTransform->CaculateModelMatrix();
		}*/
		if (m_pMesh)
		{
			m_pMesh->UpdateAnimation();
		}

		for (std::vector<HXGameObject*>::iterator itr = vctChildren.begin(); itr != vctChildren.end(); ++itr)
		{
			(*itr)->Update();
		}
	}

	/*void HXGameObject::Insert_To_RenderList(HXRenderList* pRenderList)
	{
		m_pMesh->Insert_To_RenderList(mPostion, mEulerDegree, mScale, pRenderList);
	}*/

	HXMesh* HXGameObject::GetMesh()
	{
		return m_pMesh;
	}

	HXGameObject* HXGameObject::GetFather()
	{
		return m_pFather;
	}

	void HXGameObject::SetFather(HXGameObject* father)
	{
		m_pFather = father;
	}

	void HXGameObject::AddChild(HXGameObject* child)
	{
		vctChildren.push_back(child);
	}

	std::vector<HXGameObject*>& HXGameObject::GetChildren()
	{
		return vctChildren;
	}

	HXITransform* HXGameObject::GetTransform()
	{
		return m_pTransform;
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

	int HXGameObject::GetRenderQueue() const
	{
		return m_nRenderQueue;
	}

	void HXGameObject::SetRenderQueue(int nRenderQueue)
	{
		m_nRenderQueue = nRenderQueue;
		HXSceneManager::GetInstance()->UpdateRenderableQueue();
	}
}


