#include "..\include\HXGLRenderable.h"
#include "HXVector.h"
#include "HXMesh.h"
#include "LoadShaders.h"
#include "HXGLCamera.h"
#include "HXResourceManager.h"
#include "HXLoadConfigMat.h"
#include "HXGLTexture.h"
#include "HXGLTransform.h"
#include "HXRenderSystem.h"
#include "HXSceneManager.h"
#include "HXFogLinear.h"
#include "HXRoot.h"
#include "HXGLRenderSystem.h"
#include "HXGLShadowMap.h"
#include "HXStatus.h"

namespace HX3D
{
	HXGLRenderable::HXGLRenderable()
	{
		
	}

	HXGLRenderable::~HXGLRenderable()
	{
	}

	void HXGLRenderable::SetModelMatrix(HXMatrix44& mat)
	{
		mMatrixModel = HXGLTransform::ConvertMatrix(mat);
	}

	void HXGLRenderable::SetViewMatrix(HXICamera* pCamera)
	{
		// UVN���
		// Ϊ����Ч�ʣ���������
		// mMatrixView = vmath::lookat(vmath::vec3(eye.x, eye.y, eye.z), vmath::vec3(at.x, at.y, at.z), vmath::vec3(up.x, up.y, up.z));
		mMatrixView = ((HXGLCamera*)pCamera)->mMatrixView;
	}

	void HXGLRenderable::SetProjectionMatrix(HXICamera* pCamera)
	{
		// Ϊ����Ч�ʣ���������
		// mMatrixProjection = vmath::frustum(left, right, bottom, top, n, f);
		mMatrixProjection = ((HXGLCamera*)pCamera)->mMatrixProjection;
	}

	void HXGLRenderable::GenerateArguments(HXSubMesh* pSubMesh)
	{
		HXRenderable::GenerateArguments(pSubMesh);

		if (m_pSubMesh->triangleCount > MAX_TRIANGLE_COUNT)
		{
			std::cerr << "Mesh over max triangle count !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << std::endl;
			return;
		}

		glGenVertexArrays(1, &mVAO);
		glBindVertexArray(mVAO);
		glGenBuffers(1, &mVBO);
		glBindBuffer(GL_ARRAY_BUFFER, mVBO);
		
		int nVertexCount = m_pSubMesh->triangleCount * 3;
		//GLfloat positions[nVertexCount * 3] = { 0 };
		//GLfloat positions[nVertexCount][3] = { 0 };	// ��̬�������ʱ����֪���������С
		//GLfloat positions[MAX_TRIANGLE_COUNT * 3][3] = {0};

		GLfloat* positions = new GLfloat[nVertexCount * 3];
		GLfloat* colors = new GLfloat[nVertexCount * 4];
		GLfloat* uvs = new GLfloat[nVertexCount * 2];
		GLfloat* normals = new GLfloat[nVertexCount * 3];
		GLfloat* tangents = new GLfloat[nVertexCount * 3];
		int nIndex = 0;
		for (std::vector<HXVertex>::iterator itr = m_pSubMesh->vertexList.begin(); itr != m_pSubMesh->vertexList.end(); ++itr)
		{
			positions[nIndex * 3 + 0] = itr->pos.x;
			positions[nIndex * 3 + 1] = itr->pos.y;
			positions[nIndex * 3 + 2] = itr->pos.z;
			colors[nIndex * 4 + 0] = itr->color.r / 255.0f;
			colors[nIndex * 4 + 1] = itr->color.g / 255.0f;
			colors[nIndex * 4 + 2] = itr->color.b / 255.0f;
			colors[nIndex * 4 + 3] = itr->color.a / 255.0f;
			uvs[nIndex * 2 + 0] = itr->u;
			uvs[nIndex * 2 + 1] = 1.0f - itr->v;
			normals[nIndex * 3 + 0] = itr->normal.x;
			normals[nIndex * 3 + 1] = itr->normal.y;
			normals[nIndex * 3 + 2] = itr->normal.z;
			tangents[nIndex * 3 + 0] = itr->tangent.x;
			tangents[nIndex * 3 + 1] = itr->tangent.y;
			tangents[nIndex * 3 + 2] = itr->tangent.z;

			++nIndex;
		}
		
		int nSizePositions = nVertexCount * 3 * sizeof(GLfloat);
		int nSizeColors = nVertexCount * 4 * sizeof(GLfloat);
		int nSizeUVs = nVertexCount * 2 * sizeof(GLfloat);
		int nSizeNormals = nVertexCount * 3 * sizeof(GLfloat);
		int nSizeTangents = nVertexCount * 3 * sizeof(GLfloat);
		glBufferData(GL_ARRAY_BUFFER, nSizePositions + nSizeColors + nSizeUVs + nSizeNormals + nSizeTangents, NULL, GL_STATIC_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER, 0, nSizePositions, positions);
		glBufferSubData(GL_ARRAY_BUFFER, nSizePositions, nSizeColors, colors);
		glBufferSubData(GL_ARRAY_BUFFER, nSizePositions + nSizeColors, nSizeUVs, uvs);
		glBufferSubData(GL_ARRAY_BUFFER, nSizePositions + nSizeColors + nSizeUVs, nSizeNormals, normals);
		glBufferSubData(GL_ARRAY_BUFFER, nSizePositions + nSizeColors + nSizeUVs + nSizeNormals, nSizeTangents, tangents);

		delete[] positions;
		delete[] colors;
		delete[] uvs;
		delete[] normals;
		delete[] tangents;

		glVertexAttribPointer(vPosition, 3, GL_FLOAT, GL_FALSE, 0, (const void*)(0));
		glEnableVertexAttribArray(vPosition);
		glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, (const void*)(nSizePositions));
		glEnableVertexAttribArray(vColor);
		glVertexAttribPointer(vUV, 2, GL_FLOAT, GL_FALSE, 0, (const void*)(nSizePositions + nSizeColors));
		glEnableVertexAttribArray(vUV);
		glVertexAttribPointer(vNormal, 3, GL_FLOAT, GL_FALSE, 0, (const void*)(nSizePositions + nSizeColors + nSizeUVs));
		glEnableVertexAttribArray(vNormal);
		glVertexAttribPointer(vTangent, 3, GL_FLOAT, GL_FALSE, 0, (const void*)(nSizePositions + nSizeColors + nSizeUVs + nSizeNormals));
		glEnableVertexAttribArray(vTangent);

		m_pMaterial = (HXGLMaterial*)HXResourceManager::GetInstance()->GetMaterial(pSubMesh->materialName);

		// һ��Ҫ���ϣ���Ȼ��������VBO���Ƶ������޷���ʾ
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glUseProgram(0);
	}

	void HXGLRenderable::RenderShadowMap()
	{
		if (m_pSubMesh->triangleCount > MAX_TRIANGLE_COUNT)
		{
			//std::cerr << "Mesh over max triangle count" << std::endl;
			return;
		}

		// TODO: ��ȡ����ͬ���ʵ����
		//mMaterial->SetShadowMapMaterialRenderStateAllRenderable();
		//mMaterial->SetShadowMapMaterialRenderStateEachRenderable(this);

		glBindVertexArray(mVAO);

		if (m_pSubMesh->IsStaticMesh == false)
		{
			glBindBuffer(GL_ARRAY_BUFFER, mVBO);

			int nVertexCount = m_pSubMesh->triangleCount * 3;
			GLfloat* positions = new GLfloat[nVertexCount * 3];
			int nIndex = 0;
			for (std::vector<HXVertex>::iterator itr = m_pSubMesh->vertexList.begin(); itr != m_pSubMesh->vertexList.end(); ++itr)
			{
				positions[nIndex * 3 + 0] = itr->pos.x;
				positions[nIndex * 3 + 1] = itr->pos.y;
				positions[nIndex * 3 + 2] = itr->pos.z;
				++nIndex;
			}
			glBufferSubData(GL_ARRAY_BUFFER, 0, nVertexCount * 3 * sizeof(GLfloat), positions);
			delete[] positions;
		}

		glDrawArrays(GL_TRIANGLES, 0, m_pSubMesh->triangleCount * 3);
		HXStatus::GetInstance()->nDrawCall += 1;

		//���������Ⱦʹ�þɽӿڣ���������״̬
		//glBindVertexArray(0);
		//glBindBuffer(GL_ARRAY_BUFFER, 0);
		////glUseProgram(0);
	}

	void HXGLRenderable::Render()
	{
		if (m_pSubMesh->triangleCount > MAX_TRIANGLE_COUNT)
		{
			//std::cerr << "Mesh over max triangle count" << std::endl;
			return;
		}
		
		// TODO: ��ȡ����ͬ���ʵ����
		//mMaterial->SetMaterialRenderStateAllRenderable();
		//mMaterial->SetMaterialRenderStateEachRenderable(this);

		glBindVertexArray(mVAO);

		// ���RenderShadowMap���Ѹ��£������ﲻ��Ҫ������
		if (m_pSubMesh->IsStaticMesh == false && m_pSubMesh->IsCastShadow == false)
		{
			glBindBuffer(GL_ARRAY_BUFFER, mVBO);

			int nVertexCount = m_pSubMesh->triangleCount * 3;			
			GLfloat* positions = new GLfloat[nVertexCount * 3];
			int nIndex = 0;
			for (std::vector<HXVertex>::iterator itr = m_pSubMesh->vertexList.begin(); itr != m_pSubMesh->vertexList.end(); ++itr)
			{
				positions[nIndex * 3 + 0] = itr->pos.x;
				positions[nIndex * 3 + 1] = itr->pos.y;
				positions[nIndex * 3 + 2] = itr->pos.z;
				++nIndex;
			}
			glBufferSubData(GL_ARRAY_BUFFER, 0, nVertexCount * 3 * sizeof(GLfloat), positions);
			delete[] positions;
		}

		/////////////////////////////////////////////////////////////////////////////////
		//// for test  MVP����ͶӰƽ���ϵ����꣬͸�ӳ�����NDC���֮꣬��(��դ��֮ǰ)��Ⱦ���߻��Զ�ӳ�䵽��Ļ����
		//vmath::vec4 vec(0.5f, 0.5f, 0.5f, 1.0f);
		//vmath::mat4 matT = vmath::translate(100.0f, 0.0f, 0.0f);
		//// ΪʲôӦ�ó������ҳˣ���ɫ�������ҳ� ?  
		//// ��Ϊvec * mMatrixModel �е� static inline vecN<T,N> operator*(const vecN<T,M>& vec, const matNM<T,N,M>& mat)
		//// ��������������������� ����ͳһ     �����ô˳˷�    
		//// vec = (mMatrixProjection * (mMatrixView * (mMatrixModel * vec)));
		//vmath::vec4 vec0 = vec * mMatrixModel;
		//vmath::vec4 vec1 = vec * mMatrixModel*mMatrixView;
		//vmath::vec4 vec2 = vec * mMatrixModel*mMatrixView*mMatrixProjection;
		//vmath::vec4 vec3 = vec * matT;
		//// ����������: a11 a21 a31 a41  Mat[column][row]		// http://www.xuebuyuan.com/247515.html
		//// ����������: 
		//// a11
		//// a12
		//// a13
		//// a14
		//// �˾��������˷�����ȷ��  
		//vmath::vec4 vec4 = mMatrixModel * vec;		
		//vmath::vec4 vec5 = mMatrixView * mMatrixModel * vec;
		//vmath::vec4 vec6;
		//for (std::vector<HXVertex>::iterator itr = m_pSubMesh->vertexList.begin(); itr != m_pSubMesh->vertexList.end(); ++itr)
		//{
		//	vec6 = vmath::vec4(itr->pos.x, itr->pos.y, itr->pos.z, 1);
		//	vec6 = mMatrixProjection * mMatrixView * mMatrixModel * vec6;
		//	vec6[0] /= vec6[3];
		//	vec6[1] /= vec6[3];
		//	vec6[2] /= vec6[3];
		//	vec6[3] /= vec6[3];
		//	// ת������NDC��׼�豸����ϵͳ����
		//	// shader������ɫ���仯���������[-1,+1]����Ķ��ᱻ�ü���?(opengl: -w <= x <= w  -w <= y <= w  -w <= z <= w) (direct3d: -w <= x <= w  -w <= y <= w  0 <= z <= w) (��׼���: w = 1)
		//	int test = 0;
		//}
		//vmath::vec4 vec7 = matT * vec;
		//// �����˷���ȷ
		//mMatrixModel*mMatrixView;
		////////////////////////////////////////////////////////////////////////////////

		glDrawArrays(GL_TRIANGLES, 0, m_pSubMesh->triangleCount * 3);
		HXStatus::GetInstance()->nDrawCall += 1;

		//���������Ⱦʹ�þɽӿڣ���������״̬
		//glBindVertexArray(0);
		//glBindBuffer(GL_ARRAY_BUFFER, 0);
		//glUseProgram(0);
	}

	float HXGLRenderable::GetZDepth()
	{
		vmath::mat4 matMV = mMatrixView * mMatrixModel;
		return matMV[3][2];
	}
}
