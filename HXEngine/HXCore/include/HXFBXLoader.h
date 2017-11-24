#pragma once
#include "HXCommon.h"
#include "HXIMeshLoader.h"
#include <fbxsdk.h>

namespace HX3D
{
	class HXMesh;
	class HXFBXMesh;
	struct HXVertex;
	class HXFBXLoader : public HXIMeshLoader
	{
	public:
		HXFBXLoader();
		~HXFBXLoader();

		virtual bool LoadMeshFromFile(std::string strFileName, std::string strAnimName, HXMesh** ppMesh);
		
		// ���ڱ���Material��MeshΨһ����
		static std::string gCurPathFileName;

	private:

		bool LoadScene(std::string strFileName);
		void ProcessNode(FbxNode* pNode, HXFBXMesh* pMesh);
		void ProcessMesh(FbxNode* pNode, HXFBXMesh* pMesh);
		// ��ȡ�������ꡢ��ɫ��UV�����ߵȡ�����SUBMESH��������������
		void ProcessPolygons(FbxMesh* pFbxMesh, HXFBXMesh* pMesh);
		// ��ȡ���ز��ʺ���ͼ
		//void ProcessMaterial(FbxMesh* pFbxMesh);

		
	private:
		FbxManager* m_pFbxManager;
		FbxScene* m_pScene;
		
	};
}