#pragma once
#include "HXCommon.h"
#include "HXVector.h"


namespace HX3D
{
	class HXMesh;
	class HXRenderList;

	class HXGameObject
	{
	public:
		HXGameObject(HXMesh* pMesh);
		~HXGameObject();
	
		void Update();

		void Insert_To_RenderList(HXRenderList* pRenderList);
		void SetPostion(const HXVector3D& pos);
		HXVector3D GetPosition();
		void Move(const HXVector3D& mov);

		void SetRotation(const HXVector3D& rotation);
		HXVector3D GetRotation();
		void Pitch(float fDegree);	// ��X����ת
		void Yaw(float fDegree);	// ��Y����ת
		void Roll(float fDegree);	// ��Z����ת
		
		void SetScale(const HXVector3D& scale);
		HXVector3D GetScale();

		HXMesh* GetMesh();

	private:
		HXVector3D mPostion;
		HXVector3D mEulerDegree;
		HXVector3D mScale;

		HXMesh* m_pMesh;
	};
}


