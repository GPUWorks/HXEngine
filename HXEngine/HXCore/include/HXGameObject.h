#pragma once
#include "HXCommon.h"
#include "HXVector.h"
#include "HXMatrix.h"
#include "HXITransform.h"

namespace HX3D
{
	class HXMesh;
	struct HXRenderList;
	class HXRenderSystem;
	class HXGameObject
	{
	public:
		HXGameObject(std::string strName, HXMesh* pMesh, HXRenderSystem* pRenderSystem);
		~HXGameObject();
	
		void Update();

		//void Insert_To_RenderList(HXRenderList* pRenderList);

		HXMesh* GetMesh();
		HXGameObject* GetFather();
		void SetFather(HXGameObject* father);
		HXITransform* GetTransform();

		// ��Ⱦ���ȼ�
		int m_nPriority;
		bool m_bCastShadow;
		
	private:
		std::string m_strName;
		// Local Coordinate
		/*HXVector3D mPostion;
		HXVector3D mEulerDegree;
		HXVector3D mScale;*/
		HXITransform* m_pTransform;

		HXMesh* m_pMesh;
		// TODO: config����ʱֻ֧������ prefabgameobj--->modelgameobj
		HXGameObject* m_pFather;	// ��������World Coordinate
	};
}


