#pragma once
#include "HXCommon.h"
#include "HXIMeshLoader.h"
#include <fbxsdk.h>

namespace HX3D
{
	class HXMesh;
	class HXMeshFBX;
	class HXVertex;
	class HXFBXLoader : public HXIMeshLoader
	{
	public:
		HXFBXLoader();
		~HXFBXLoader();

		bool LoadScene(std::string strFileName);
		void ProcessNode(FbxNode* pNode, HXMeshFBX* pMesh);
		void ProcessMesh(FbxNode* pNode, HXMeshFBX* pMesh);
		// ��ȡ�������ꡢ��ɫ��UV�����ߵȡ�����SUBMESH��������������
		void ProcessPolygons(FbxMesh* pFbxMesh, HXMeshFBX* pMesh);
		// ��ȡ���ز��ʺ���ͼ
		void ProcessMaterial(FbxMesh* pFbxMesh);
		virtual bool LoadMeshFromFile(std::string strFileName, HXMesh** ppMesh);

		// ���ڱ���Material��MeshΨһ����
		static std::string gCurPathFileName;
	private:
		FbxManager* m_pFbxManager;
		FbxScene* m_pScene;
		
	};
}