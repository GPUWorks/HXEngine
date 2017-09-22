#pragma once
#include "HXCommon.h"
#include <map>

namespace HX3D
{
	class HXMesh;
	class HXMaterial;
	class HXBitmap;
	class HXIMeshLoader;
	class HXResourceManager
	{
	public:
		static HXResourceManager* GetInstance()
		{
			if (NULL == m_pInstance)
			{
				m_pInstance = new HXResourceManager();
			}
			return m_pInstance;
		}
		~HXResourceManager();

		HXMesh* GetMesh(std::string strMeshName);
		HXMaterial* GetMaterial(std::string strMaterialName);
		HXBitmap* GetBitmap(std::string strBitmap);
		// ����FBXʱʹ��FBX�������ɣ��Ժ�������༭�����ɵĲ��ʼ���
		void AddMaterial(std::string strMaterialName, HXMaterial* pMaterial);

	private:
		HXResourceManager();
		static HXResourceManager* m_pInstance;

		HXIMeshLoader*	m_pMeshLoader;
		std::map<std::string, HXMesh*>	meshMap;
		std::map<std::string, HXMaterial*> materialMap;
		std::map<std::string, HXBitmap*> bitmapMap;
	};
}


