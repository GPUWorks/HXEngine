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

		std::string GetName() { return m_strName; }
		void SetName(std::string strName) { m_strName = strName; }
		HXMesh* GetMesh();
		HXGameObject* GetFather();
		void SetFather(HXGameObject* father);
		void AddChild(HXGameObject* child);
		std::vector<HXGameObject*>& GetChildren();
		HXITransform* GetTransform();
		bool GetCastShadow() const;
		void SetCastShadow(bool bCastShadow);

		std::string m_strName;
		// ��Ⱦ���ȼ�
		int m_nRenderQueue;
		std::string m_strModelFile;
		
	private:
		// Local Coordinate
		HXITransform* m_pTransform;
		bool m_bCastShadow;
		HXMesh* m_pMesh;
		HXGameObject* m_pFather;	// ��������World Coordinate
		std::vector<HXGameObject*> vctChildren;
	};
}


