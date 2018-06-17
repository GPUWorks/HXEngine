#include "..\include\HXSceneManager.h"
#include "HXMesh.h"
#include "HXGameObject.h"
#include "HXCore.h"
#include "HXFrustum.h"
#include "HXResourceManager.h"
#include "HXRenderState.h"
#include "HXStatus.h"
#include "HXRoot.h"
#include "HXRenderSystem.h"
#include "HXRenderable.h"
#include "HXICamera.h"
#include "HXLoadConfigModel.h"
#include "HXLoadConfigMat.h"
#include <algorithm>
#include "HXFogLinear.h"
#include "HXLoadConfigScene.h"
#include "HXLoadConfigPrefab.h"

namespace HX3D
{
	HXSceneManager* HXSceneManager::m_pInstance = NULL;

	HXSceneManager::HXSceneManager():m_pMainCamera(NULL), fog(NULL), nameSuffix(0)
	{
		mRenderList = new HXRenderList();
		ambient = HXCOLOR(0,0,0,255);
	}

	HXSceneManager::~HXSceneManager()
	{
		for (std::vector<HXGameObject*>::iterator itr = gameObjectList.begin(); itr != gameObjectList.end(); itr++)
		{
			delete(*itr);
		}
		gameObjectList.clear();
		
		for (std::vector<HXLight*>::iterator itr = lightVct.begin(); itr != lightVct.end(); itr++)
		{
			delete *itr;
		}
		lightVct.clear();

		delete mRenderList;

		if (m_pMainCamera)
		{
			delete m_pMainCamera;
		}

		if (fog)
		{
			delete fog;
		}
	}

	void HXSceneManager::LoadScene(std::string strSceneCfgFile)
	{
		// GDI��������ԭ��ʹ�ó����򵥰汾
		if (HXRoot::GetInstance()->GetRenderSystem() && 
			HXRoot::GetInstance()->GetRenderSystem()->GetRenderSystemType() == RenderSystem_GDI)
		{
			//strSceneCfgFile.insert(strSceneCfgFile.length() - 6, "_GDI");
			strSceneCfgFile += ".gdi";
		}

		HXLoadConfigScene cfg;
		cfg.LoadFile(strSceneCfgFile);
		// camera
		CreateMainCamera(cfg.mSceneInfo.cameraInfo.eye, cfg.mSceneInfo.cameraInfo.at, cfg.mSceneInfo.cameraInfo.up
			, cfg.mSceneInfo.cameraInfo.ffov, cfg.mSceneInfo.cameraInfo.nearZ, cfg.mSceneInfo.cameraInfo.farZ);
		// fog
		CreateFog(&cfg.mSceneInfo.fogInfo);
		// ambient
		ambient = cfg.mSceneInfo.ambient;
		// light
		for (std::vector<HXLightInfo>::iterator itr = cfg.mSceneInfo.vctLight.begin(); itr != cfg.mSceneInfo.vctLight.end(); ++itr)
		{
			HXLightInfo info = *itr;
			CreateLight(&info);
		}
		// ������պ�
		CreateSkyBox(HXVector3D(200, 200, 200));
		// GameObject
		for (std::vector<HXPrefabGameObjInfo>::iterator itr = cfg.mSceneInfo.vctGameObjInfo.begin(); itr != cfg.mSceneInfo.vctGameObjInfo.end(); ++itr)
		{
			HXPrefabGameObjInfo& prefabgoinfo = *itr;
			HXGameObject* pFatherGameObject = CreateGameObject(NULL, prefabgoinfo.strGameObjName, "", prefabgoinfo.nPriority, prefabgoinfo.bCastShadow);
			if (NULL == pFatherGameObject)
			{
				return;
			}
			/*if (pFatherGameObject->GetMesh())
			{
				pFatherGameObject->GetMesh()->PlayDefaultAnimation();
			}*/
			pFatherGameObject->GetTransform()->SetScale(prefabgoinfo.scale);
			pFatherGameObject->GetTransform()->SetRotation(prefabgoinfo.rotation);
			pFatherGameObject->GetTransform()->SetPostion(prefabgoinfo.position);

			HXLoadConfigPrefab cfgPrefab;
			cfgPrefab.LoadFile(prefabgoinfo.strPrefabFile);
			for (std::vector<HXModelGameObjInfo>::iterator itr1 = cfgPrefab.mPrefabInfo.vctGameObjInfo.begin(); itr1 != cfgPrefab.mPrefabInfo.vctGameObjInfo.end(); ++itr1)
			{
				HXModelGameObjInfo& modelgoinfo = *itr1;
				HXGameObject* pGameObject = CreateGameObject(pFatherGameObject, modelgoinfo.strGameObjName, modelgoinfo.strModelFile, prefabgoinfo.nPriority, prefabgoinfo.bCastShadow);
				if (NULL == pGameObject)
				{
					return;
				}
				if (pGameObject->GetMesh())
				{
					pGameObject->GetMesh()->PlayDefaultAnimation();
				}
				pGameObject->GetTransform()->SetScale(modelgoinfo.scale);
				pGameObject->GetTransform()->SetRotation(modelgoinfo.rotation);
				pGameObject->GetTransform()->SetPostion(modelgoinfo.position);

				pGameObject->SetFather(pFatherGameObject);
			}
		}
	}

	void HXSceneManager::UnLoadScene()
	{
		if (m_pMainCamera)
		{
			delete m_pMainCamera;
			m_pMainCamera = NULL;
		}
		if (fog)
		{
			delete fog;
			fog = NULL;
		}
		for (std::vector<HXLight*>::iterator itr = lightVct.begin(); itr != lightVct.end(); ++itr)
		{
			delete *itr;
		}
		lightVct.clear();
		for (std::vector<HXGameObject*>::iterator itr = gameObjectList.begin(); itr != gameObjectList.end(); ++itr)
		{
			delete *itr;
		}
		gameObjectList.clear();
	}

	HXGameObject* HXSceneManager::CreateGameObject(HXGameObject* pFather, std::string strGameObjectName, std::string strModelName, int nPriority, bool bCastShadow)
	{
		/*std::map<std::string, HXGameObject*>::iterator itr = gameObjectList.find(strGameObjectName);
		if (itr != gameObjectList.end() && pFather == NULL)
		{
			std::cerr << strGameObjectName + " alreay existed" << std::endl;
			return itr->second;
		}*/
		HXModelInfo* pModelInfo = HXResourceManager::GetInstance()->GetModelInfo(strModelName);
		if (NULL == pModelInfo)
		{
			// ��strModelName = ����ʱ����������renderable��gameobject�����縸�����gameobject
			HXGameObject* gameObject = new HXGameObject(strGameObjectName, NULL, HXRoot::GetInstance()->GetRenderSystem());
			gameObject->m_nPriority = nPriority;
			gameObject->m_bCastShadow = bCastShadow;
			if (pFather == NULL)
			{
				gameObjectList.push_back(gameObject);
			}
			else
			{
				pFather->AddChild(gameObject);
			}
			return gameObject;
		}

		// ����FBX
		HXMesh* pMesh = HXResourceManager::GetInstance()->GetMesh(pModelInfo->m_strMeshFile, pModelInfo->m_strAnimFile);
		if (NULL == pMesh)
		{
			return NULL;
		}
		
		// ���ز���
		int nMatCount = 0;
		for (std::vector<std::string>::iterator itr = pModelInfo->m_vctSubMeshMat.begin(); itr != pModelInfo->m_vctSubMeshMat.end(); ++itr)
		{
			HXMaterialInfo* pMat = HXResourceManager::GetInstance()->GetMaterialInfo(*itr);
			if (NULL == pMat)
			{
				// ��������ڸò�������ʹ�÷�ɫ����
				*itr = "./builtin/ErrorMat.xml";
				HXResourceManager::GetInstance()->GetMaterialInfo(*itr);
			}
			++nMatCount;
		}
		// ���û���ʣ������Ĭ�ϲ���
		if (nMatCount == 0)
		{
			pModelInfo->m_vctSubMeshMat.push_back("./builtin/ErrorMat.xml");
			HXResourceManager::GetInstance()->GetMaterialInfo("./builtin/ErrorMat.xml");
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
			++nSubMeshIndex;
		}


		HXGameObject* gameObject = new HXGameObject(strGameObjectName, pMesh->Clone(HXRoot::GetInstance()->GetRenderSystem()), HXRoot::GetInstance()->GetRenderSystem());
		gameObject->m_nPriority = nPriority;
		gameObject->m_bCastShadow = bCastShadow;
		if (pFather == NULL)
		{
			gameObjectList.push_back(gameObject);
		}
		else
		{
			pFather->AddChild(gameObject);
		}
		return gameObject;

	}

	HXGameObject* HXSceneManager::GetGameObject(std::string strGameObjectName)
	{
		for (std::vector<HXGameObject*>::iterator itr = gameObjectList.begin(); itr != gameObjectList.end(); ++itr)
		{
			if ((*itr)->GetName() == strGameObjectName)
			{
				return *itr;
			}
		}
		return NULL;
	}

	std::vector<HXGameObject*> HXSceneManager::GetGameObjectList()
	{
		return gameObjectList;
	}

	HXLight* HXSceneManager::CreateLight(HXLightInfo* lightInfo)
	{
		HXLight* pLight = new HXLight(lightInfo);
		lightVct.push_back(pLight);
		return pLight;
	}

	HXICamera* HXSceneManager::CreateMainCamera(const HXVector3D& eye, const HXVector3D& at, const HXVector3D& up,
		float ffov, float nearZ, float farZ, float viewportWidth, float viewportHeigth,
		float left, float right, float bottom, float top)
	{
		if (HXRoot::GetInstance()->GetRenderSystem())
		{
			m_pMainCamera = HXRoot::GetInstance()->GetRenderSystem()->CreateCamera(eye, at, up, ffov, nearZ, farZ, viewportWidth, viewportHeigth,
				left, right, bottom, top);
		}
		return m_pMainCamera;
	}

	HXICamera* HXSceneManager::GetMainCamera()
	{
		return m_pMainCamera;
	}


	HXGameObject* HXSceneManager::CreateSkyBox(HXVector3D scale)
	{
		if (HXRoot::GetInstance()->GetRenderSystem() && 
			HXRoot::GetInstance()->GetRenderSystem()->GetRenderSystemType() == RenderSystem_GDI)
		{
			return NULL;
		}

		std::string strgoName = "HXSkyBox";

		/*std::map<std::string, HXGameObject*>::iterator itr = gameObjectList.find(strgoName);
		if (itr != gameObjectList.end())
		{
			std::cerr << "HXSkyBox alreay existed" << std::endl;
			return itr->second;
		}*/

		// ����mesh
		HXMesh* pMesh = HXResourceManager::GetInstance()->GetMesh("HXSkyBoxMesh", "");
		if (NULL == pMesh)
		{
			return NULL;
		}

		// ���ز���
		HXMaterialInfo* pMat = HXResourceManager::GetInstance()->GetMaterialInfo("./FBX/SkyBox/SkyBox.material");
		if (NULL == pMat)
		{
			return NULL;
		}

		// �������ʵ�SubMesh
		pMesh->subMeshList[0]->materialName = "./FBX/SkyBox/SkyBox.material";

		HXGameObject* gameObject = new HXGameObject(strgoName, pMesh->Clone(HXRoot::GetInstance()->GetRenderSystem()), HXRoot::GetInstance()->GetRenderSystem());
		gameObject->GetTransform()->SetScale(scale);
		gameObject->m_nPriority = 0;
		gameObject->m_bCastShadow = false;
		gameObjectList.push_back(gameObject);
		return gameObject;
	}

	void HXSceneManager::CreateFog(HXFogInfo* info)
	{
		if (info->type == HXFogType::FOG_Linear)
		{
			fog = new HXFogLinear(info);
		}
	}

	void HXSceneManager::UseFog(bool useFog)
	{
		if (fog)
		{
			fog->useFog = useFog;
		}
	}

	bool mycompare(HXGameObject* i, HXGameObject* j)
	{
		return( i->m_nPriority < j->m_nPriority );
	}

	void HXSceneManager::OnDisplay(bool shadow)
	{
		if (!m_pMainCamera)
		{
			return;
		}

		if (shadow)
		{
			m_pMainCamera->Update();
		}
		else
		{
			HXStatus::GetInstance()->ResetStatus();
		}
		
		// TODO: OIT������ʱ�ȼ�����
		// TODO: ����push children
		std::vector<HXGameObject*> sortGameObject;
		for (std::vector<HXGameObject*>::iterator itr = gameObjectList.begin(); itr != gameObjectList.end(); itr++)
		{
			sortGameObject.push_back(*itr);
			std::vector<HXGameObject*> children = (*itr)->GetChildren();
			for (std::vector<HXGameObject*>::iterator itr1 = children.begin(); itr1 != children.end(); ++itr1)
			{
				sortGameObject.push_back(*itr1);
			}
		}
		std::sort(sortGameObject.begin(), sortGameObject.end(), mycompare);

		for (std::vector<HXGameObject*>::iterator itr = sortGameObject.begin(); itr != sortGameObject.end(); itr++)
		{
			// For test
			// itr->second->Yaw(itr->second->GetRotation().y + 1.0f);
			// itr->second->Roll(itr->second->GetRotation().z + 1.0f);

			// ��������ModelMatrix ������
			if (shadow)
			{
				(*itr)->Update();
			}
			
			if (shadow && !(*itr)->m_bCastShadow)
			{
				continue;
			}

			HXMesh* pMesh = (*itr)->GetMesh();
			if (pMesh == NULL)
			{
				continue;
			}
			// ���¶���
			//pMesh->UpdateAnimation();

			HXStatus::GetInstance()->nVertexCount += pMesh->vertexCount;
			HXStatus::GetInstance()->nTriangleCount += pMesh->triangleCount;

			for (std::vector<HXSubMesh*>::iterator itr1 = pMesh->subMeshList.begin(); itr1 != pMesh->subMeshList.end(); itr1++)
			{
				HXRenderSystem* pRenderSystem = HXRoot::GetInstance()->GetRenderSystem();
				if (pRenderSystem)
				{
					//(*itr1)->renderable->SetModelMatrix(itr->second->GetPosition(), itr->second->GetRotation(), itr->second->GetScale());
					(*itr1)->renderable->SetModelMatrix((*itr)->GetTransform()->mCurModelMatrix);
					// TODO: ��ȡ�����Camera������Ч��   Just for test here
					(*itr1)->renderable->SetViewMatrix(m_pMainCamera);
					(*itr1)->renderable->SetProjectionMatrix(m_pMainCamera);
					pRenderSystem->RenderSingle((*itr1)->renderable, shadow);
				}
			}
		}

		if (!shadow)
		{
			HXStatus::GetInstance()->ShowStatusInfo();
		}
	}

	void HXSceneManager::OnViewPortResize(int nScreenWidth, int nScreenHeight)
	{
		if (m_pMainCamera)
		{
			m_pMainCamera->OnViewPortResize(nScreenWidth, nScreenHeight);
		}
	}

	void HXSceneManager::LoadGameObjectInEditor(std::string strPath)
	{
		HXPrefabGameObjInfo prefabgoinfo;
		// TODO: ��ֹ����
		prefabgoinfo.strGameObjName = "GameObject" + IntToString(nameSuffix++);
		prefabgoinfo.strPrefabFile = strPath;
		prefabgoinfo.nPriority = 20;
		prefabgoinfo.bCastShadow = true;
		prefabgoinfo.position = HXVector3D(0,0,0);
		prefabgoinfo.rotation = HXVector3D(0,0,0);
		prefabgoinfo.scale = HXVector3D(1,1,1);

		HXGameObject* pFatherGameObject = CreateGameObject(NULL, prefabgoinfo.strGameObjName, "", prefabgoinfo.nPriority, prefabgoinfo.bCastShadow);
		if (NULL == pFatherGameObject)
		{
			return;
		}
		/*if (pFatherGameObject->GetMesh())
		{
			pFatherGameObject->GetMesh()->PlayDefaultAnimation();
		}*/
		pFatherGameObject->GetTransform()->SetScale(prefabgoinfo.scale);
		pFatherGameObject->GetTransform()->SetRotation(prefabgoinfo.rotation);
		pFatherGameObject->GetTransform()->SetPostion(prefabgoinfo.position);

		HXLoadConfigPrefab cfgPrefab;
		cfgPrefab.LoadFile(prefabgoinfo.strPrefabFile);
		for (std::vector<HXModelGameObjInfo>::iterator itr1 = cfgPrefab.mPrefabInfo.vctGameObjInfo.begin(); itr1 != cfgPrefab.mPrefabInfo.vctGameObjInfo.end(); ++itr1)
		{
			HXModelGameObjInfo& modelgoinfo = *itr1;
			HXGameObject* pGameObject = CreateGameObject(pFatherGameObject, modelgoinfo.strGameObjName, modelgoinfo.strModelFile, prefabgoinfo.nPriority, prefabgoinfo.bCastShadow);
			if (NULL == pGameObject)
			{
				return;
			}
			if (pGameObject->GetMesh())
			{
				pGameObject->GetMesh()->PlayDefaultAnimation();
			}
			pGameObject->GetTransform()->SetScale(modelgoinfo.scale);
			pGameObject->GetTransform()->SetRotation(modelgoinfo.rotation);
			pGameObject->GetTransform()->SetPostion(modelgoinfo.position);

			pGameObject->SetFather(pFatherGameObject);
		}
	}
}

