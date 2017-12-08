#pragma once
#include <fbxsdk.h>
#include "HXMesh.h"

namespace HX3D
{
	class HXFBXMesh : public HXMesh
	{
	public:
		HXFBXMesh();
		~HXFBXMesh();

		bool Initialize(FbxMesh* pFbxMesh);
		void UpdateVertexPosition(const FbxVector4* pVertices);
		virtual HXMesh* Clone(HXRenderSystem* pRenderSystem);
		

	private:
		void ReadVertex(FbxMesh* pFbxMesh, int nCtrlPointIndex, HXVertex& vertex);
		void ReadColor(FbxMesh* pFbxMesh, int nCtrlPointIndex, int nVertexCounter, HXVertex& vertex);
		// TODO: ���UV��ȡ
		void ReadUV(FbxMesh* pFbxMesh, int nCtrlPointIndex, int nTextureUVIndex, HXVertex& vertex);
		void ReadNormal(FbxMesh* pFbxMesh, int nCtrlPointIndex, int nVertexCounter, HXVertex& vertex);

		// important ������ʱ����ϵת���й�(ͬһ��mesh�µ�����submesh�������һ����)������Ի������š���ת��λ�Ƶ�
		FbxAMatrix matrixMeshGlobalPositionIn3DMax;

		// �µĳ�Ա����cloneʱҪ��ֵ
	};
}