#include "..\include\HXGLRenderable.h"
#include "HXVector.h"
#include "HXMesh.h"
#include "LoadShaders.h"
// #include "vmath.h"
#include "HXGLCamera.h"

namespace HX3D
{
	HXGLRenderable::HXGLRenderable()
	{
	}


	HXGLRenderable::~HXGLRenderable()
	{
	}

	void HXGLRenderable::SetModelMatrix(const HXVector3D& pos, const HXVector3D& eulerDegree, const HXVector3D& scale)
	{
		// ���Ŵ���S
		vmath::mat4 matS = vmath::scale(scale.x, scale.y, scale.z);
		// ��ת����Q
		vmath::mat4 matX = vmath::rotate(eulerDegree.x, 1.0f, 0.0f, 0.0f);
		vmath::mat4 matY = vmath::rotate(eulerDegree.y, 0.0f, 1.0f, 0.0f);
		vmath::mat4 matZ = vmath::rotate(eulerDegree.z, 0.0f, 0.0f, 1.0f);
		// ƽ��һ��Ҫ�����T
		vmath::mat4 matT = vmath::translate(pos.x, pos.y, pos.z);

		// ģ�Ϳռ䵽����ռ�ת�� SQT(OPENGL��������ϵ������������)
		mMatrixModel = matT*matX*matZ*matY*matS;
		// mMatrixModel = matS * matY * matZ * matX * matT;
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

		glGenVertexArrays(1, &mVAO);
		glBindVertexArray(mVAO);
		glGenBuffers(1, &mVBO);
		glBindBuffer(GL_ARRAY_BUFFER, mVBO);

		if (m_pSubMesh->triangleCount > MAX_TRIANGLE_COUNT)
		{
			std::cerr << "Mesh over max triangle count" << std::endl;
			return;
		}
		int nVertexCount = m_pSubMesh->triangleCount * 3;
		//GLfloat positions[nVertexCount * 3] = { 0 };
		//GLfloat positions[nVertexCount][3] = { 0 };	// ��̬�������ʱ����֪���������С

		//GLfloat positions[MAX_TRIANGLE_COUNT * 3][3] = {0};
		GLfloat* positions = new GLfloat[nVertexCount * 3];
		GLfloat* colors = new GLfloat[nVertexCount * 4];
		GLfloat* uvs = new GLfloat[nVertexCount * 2];
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
			uvs[nIndex * 2 + 1] = itr->v;
			++nIndex;
		}
		
		int nSizePositions = nVertexCount * 3 * sizeof(GLfloat);
		int nSizeColors = nVertexCount * 4 * sizeof(GLfloat);
		int nSizeUVs = nVertexCount * 2 * sizeof(GLfloat);
		glBufferData(GL_ARRAY_BUFFER, nSizePositions + nSizeColors + nSizeUVs, NULL, GL_STATIC_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER, 0, nSizePositions, positions);
		glBufferSubData(GL_ARRAY_BUFFER, nSizePositions, nSizeColors, colors);
		glBufferSubData(GL_ARRAY_BUFFER, nSizePositions + nSizeColors, nSizeUVs, uvs);

		delete[] positions;
		delete[] colors;
		delete[] uvs;

		ShaderInfo shaders[] = {
			{ GL_VERTEX_SHADER, "shader\\cube.vert" },
			{ GL_FRAGMENT_SHADER, "shader\\cube.frag" },
			{ GL_NONE, NULL }
		};

		program = LoadShaders(shaders);
		glUseProgram(program);

		render_model_matrix_loc = glGetUniformLocation(program, "model_matrix");
		render_view_matrix_loc = glGetUniformLocation(program, "view_matrix");
		render_projection_matrix_loc = glGetUniformLocation(program, "projection_matrix");
		render_mvp_matrix_loc = glGetUniformLocation(program, "mvp_matrix");

		glVertexAttribPointer(vPosition, 3, GL_FLOAT, GL_FALSE, 0, (const void*)(0));
		glEnableVertexAttribArray(vPosition);
		glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, (const void*)(nSizePositions));
		glEnableVertexAttribArray(vColor);
		glVertexAttribPointer(vUV, 2, GL_FLOAT, GL_FALSE, 0, (const void*)(nSizePositions + nSizeColors));
		glEnableVertexAttribArray(vUV);
	}

	void HXGLRenderable::Render()
	{
		// ��� HXGLRenderSystem �ѵ���
		// glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		//glEnable(GL_CULL_FACE);
		//glFrontFace(GL_CCW);
		//glFrontFace(GL_CW);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);


		glBindVertexArray(mVAO);
		glUseProgram(program);

		if (m_pSubMesh->IsStaticMesh == false)
		{
			glBindBuffer(GL_ARRAY_BUFFER, mVBO);

			if (m_pSubMesh->triangleCount > MAX_TRIANGLE_COUNT)
			{
				std::cerr << "Mesh over max triangle count" << std::endl;
				return;
			}
			int nVertexCount = m_pSubMesh->triangleCount * 3;			
			GLfloat* positions = new GLfloat[nVertexCount * 3];
			//GLfloat* colors = new GLfloat[nVertexCount * 4];
			int nIndex = 0;
			for (std::vector<HXVertex>::iterator itr = m_pSubMesh->vertexList.begin(); itr != m_pSubMesh->vertexList.end(); ++itr)
			{
				positions[nIndex * 3 + 0] = itr->pos.x;
				positions[nIndex * 3 + 1] = itr->pos.y;
				positions[nIndex * 3 + 2] = itr->pos.z;
				//colors[nIndex * 4 + 0] = itr->color.r / 255.0f;
				//colors[nIndex * 4 + 1] = itr->color.g / 255.0f;
				//colors[nIndex * 4 + 2] = itr->color.b / 255.0f;
				//colors[nIndex * 4 + 3] = itr->color.a / 255.0f;
				++nIndex;
			}

			//glBufferData(GL_ARRAY_BUFFER, nVertexCount * (3 + 4) * sizeof(GLfloat), NULL, GL_STATIC_DRAW);
			glBufferSubData(GL_ARRAY_BUFFER, 0, nVertexCount * 3 * sizeof(GLfloat), positions);
			//glBufferSubData(GL_ARRAY_BUFFER, nVertexCount * 3 * sizeof(GLfloat), nVertexCount * 4 * sizeof(GLfloat), colors);

			delete[] positions;
			//delete[] colors;
		}

		/////////////////////////////////////////////////////////////////////////////////
		// for test  MVP����ͶӰƽ���ϵ����֮꣬��(��դ��ʱ?)��Ⱦ���߻��Զ�ӳ�䵽��Ļ����
		vmath::vec4 vec(0.5f, 0.5f, 0.5f, 1.0f);
		vmath::mat4 matT = vmath::translate(100.0f, 0.0f, 0.0f);
		// ΪʲôӦ�ó������ҳˣ���ɫ�������ҳ� ?  
		// ��Ϊvec * mMatrixModel �е� static inline vecN<T,N> operator*(const vecN<T,M>& vec, const matNM<T,N,M>& mat)
		// ��������������������� ����ͳһ     �����ô˳˷�    
		// vec = (mMatrixProjection * (mMatrixView * (mMatrixModel * vec)));
		vmath::vec4 vec0 = vec * mMatrixModel;
		vmath::vec4 vec1 = vec * mMatrixModel*mMatrixView;
		vmath::vec4 vec2 = vec * mMatrixModel*mMatrixView*mMatrixProjection;
		vmath::vec4 vec3 = vec * matT;
		// ����������: a11 a21 a31 a41  Mat[column][row]		// http://www.xuebuyuan.com/247515.html
		// ����������: 
		// a11
		// a12
		// a13
		// a14
		// �˾��������˷�����ȷ��  
		vmath::vec4 vec4 = mMatrixModel * vec;		
		vmath::vec4 vec5 = mMatrixView * mMatrixModel * vec;
		vmath::vec4 vec6 = mMatrixProjection * mMatrixView * mMatrixModel * vec;
		vmath::vec4 vec7 = matT * vec;
		// �����˷���ȷ
		mMatrixModel*mMatrixView;
		////////////////////////////////////////////////////////////////////////////////

		glUniformMatrix4fv(render_model_matrix_loc, 1, GL_FALSE, mMatrixModel);
		glUniformMatrix4fv(render_view_matrix_loc, 1, GL_FALSE, mMatrixView);
		glUniformMatrix4fv(render_projection_matrix_loc, 1, GL_FALSE, mMatrixProjection);
		glUniformMatrix4fv(render_mvp_matrix_loc, 1, GL_FALSE, mMatrixProjection * mMatrixView * mMatrixModel);

		glDrawArrays(GL_TRIANGLES, 0, m_pSubMesh->triangleCount * 3);
		
		// ��� HXGLRenderSystem �ѵ���
		//glutSwapBuffers();
		//glutPostRedisplay();
	}
}
