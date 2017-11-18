#pragma once
#include "HXCommon.h"
#include "HXRenderable.h"
#include "HXVector.h"

namespace HX3D
{
	struct HXRenderList;
	struct HXFrustum;
	class HXICamera;
	class HXGDIRenderable : public HXRenderable
	{
	public:
		HXGDIRenderable();
		~HXGDIRenderable();

		virtual void SetModelMatrix(const HXVector3D& pos, const HXVector3D& eulerDegree, const HXVector3D& scale);
		// ��ȡ�����������Ч��
		virtual void SetViewMatrix(HXICamera* pCamera);
		// ��ȡ�����������Ч��
		virtual void SetProjectionMatrix(HXICamera* pCamera);
		// ������Ⱦϵͳ��Ҫ�����ݣ����� OPENGL �� VBO�ȣ�
		virtual void GenerateArguments(HXSubMesh* pSubMesh);
		virtual void Render();

		HXRenderList*	mRenderList;

		HXVector3D mPos;
		HXVector3D mEulerDegree;
		HXVector3D mScale;

		HXFrustum* mFrustum;
	};
}