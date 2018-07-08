#pragma once
#include "HXCommon.h"
#include "HXRenderable.h"
#include "vgl.h"
#include "vmath.h"
#include "HXMatrix.h"
#include "HXGLMaterial.h"

namespace HX3D
{
	enum Attrib_IDs
	{
		vPosition = 0,
		vColor = 1,
		vUV = 2,
		vNormal = 3
	};

	class HXICamera;
	class HXGLRenderable : public HXRenderable
	{
	public:
		HXGLRenderable();
		~HXGLRenderable();

		virtual void SetModelMatrix(HXMatrix44& mat);
		// ��ȡ�����������Ч��
		virtual void SetViewMatrix(HXICamera* pCamera);
		// ��ȡ�����������Ч��
		virtual void SetProjectionMatrix(HXICamera* pCamera);
		// ������Ⱦϵͳ��Ҫ�����ݣ����� OPENGL �� VBO�ȣ�
		virtual void GenerateArguments(HXSubMesh* pSubMesh);
		virtual void RenderShadowMap();
		virtual void Render();

		vmath::mat4 mMatrixModel;
		vmath::mat4 mMatrixView;
		vmath::mat4 mMatrixProjection;

		GLuint mVAO;
		GLuint mVBO;
		HXGLMaterial* mMaterial;
	};
}
