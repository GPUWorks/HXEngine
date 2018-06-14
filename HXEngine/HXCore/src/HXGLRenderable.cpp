#include "..\include\HXGLRenderable.h"
#include "HXVector.h"
#include "HXMesh.h"
#include "LoadShaders.h"
// #include "vmath.h"
#include "HXGLCamera.h"
#include "HXResourceManager.h"
#include "HXLoadConfigMat.h"
//#include "HXGLTextureDDS.h"
//#include "HXGLTexturePNG.h"
#include "HXGLTexture.h"
#include "HXGLTransform.h"
#include "HXRenderSystem.h"
#include "HXSceneManager.h"
#include "HXFogLinear.h"
#include "HXRoot.h"
#include "HXGLRenderSystem.h"
#include "HXGLShadowMap.h"

namespace HX3D
{
	HXGLRenderable::HXGLRenderable()
	{
	}


	HXGLRenderable::~HXGLRenderable()
	{
	}

	//void HXGLRenderable::SetModelMatrix(const HXVector3D& pos, const HXVector3D& eulerDegree, const HXVector3D& scale)
	//{
	//	// ���Ŵ���S
	//	vmath::mat4 matS = vmath::scale(scale.x, scale.y, scale.z);
	//	// ��ת����Q
	//	vmath::mat4 matX = vmath::rotate(eulerDegree.x, 1.0f, 0.0f, 0.0f);
	//	vmath::mat4 matY = vmath::rotate(eulerDegree.y, 0.0f, 1.0f, 0.0f);
	//	vmath::mat4 matZ = vmath::rotate(eulerDegree.z, 0.0f, 0.0f, 1.0f);
	//	// ƽ��һ��Ҫ�����T
	//	vmath::mat4 matT = vmath::translate(pos.x, pos.y, pos.z);

	//	// ģ�Ϳռ䵽����ռ�ת�� SQT(OPENGL��������ϵ������������)
	//	mMatrixModel = matT*matX*matZ*matY*matS;
	//	// mMatrixModel = matS * matY * matZ * matX * matT;
	//}

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


	// GLuint texture;

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

			++nIndex;
		}
		
		int nSizePositions = nVertexCount * 3 * sizeof(GLfloat);
		int nSizeColors = nVertexCount * 4 * sizeof(GLfloat);
		int nSizeUVs = nVertexCount * 2 * sizeof(GLfloat);
		int nSizeNormals = nVertexCount * 3 * sizeof(GLfloat);
		glBufferData(GL_ARRAY_BUFFER, nSizePositions + nSizeColors + nSizeUVs + nSizeNormals, NULL, GL_STATIC_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER, 0, nSizePositions, positions);
		glBufferSubData(GL_ARRAY_BUFFER, nSizePositions, nSizeColors, colors);
		glBufferSubData(GL_ARRAY_BUFFER, nSizePositions + nSizeColors, nSizeUVs, uvs);
		glBufferSubData(GL_ARRAY_BUFFER, nSizePositions + nSizeColors + nSizeUVs, nSizeNormals, normals);

		delete[] positions;
		delete[] colors;
		delete[] uvs;
		delete[] normals;

		HXMaterialInfo* pMatInfo = HXResourceManager::GetInstance()->GetMaterialInfo(pSubMesh->materialName);
		std::string strVertShaderFile = pMatInfo->strShaderFile + ".vert";
		std::string strFragShaderFile = pMatInfo->strShaderFile + ".frag";

		ShaderInfo shaders[] = {
			{ GL_VERTEX_SHADER, strVertShaderFile.c_str() },
			{ GL_FRAGMENT_SHADER, strFragShaderFile.c_str() },
			{ GL_NONE, NULL }
		};

		render_scene_prog = LoadShaders(shaders);
		glUseProgram(render_scene_prog);

		int nTexIndex = 0;
		for (std::vector<HXMaterialProperty>::iterator itr = pMatInfo->vctMatProperty.begin(); itr != pMatInfo->vctMatProperty.end(); ++itr)
		{
			switch (itr->type)
			{
			case MPT_TEXTURE:
			{
				GLint tex_uniform_loc = glGetUniformLocation(render_scene_prog, (itr->name).c_str());
				if (tex_uniform_loc == -1)
				{
					// δ�α�ʵ�ʵ��õı��������ᱻ�Զ�ɾ��
					continue;
				}

				HXGLTexture* tex = (HXGLTexture*)HXResourceManager::GetInstance()->GetTexture("GL_" + itr->value);
				if (NULL == tex)
				{
					tex = new HXGLTexture(itr->value);
					HXResourceManager::GetInstance()->AddTexture("GL_" + itr->value, tex);
				}
				// ������
				glUniform1i(tex_uniform_loc, nTexIndex);
				// ����Ԫ
				glActiveTexture(GL_TEXTURE0 + nTexIndex);
				// ��ʱ�����������͵�Texture,����ֻʹ��GL_TEXTURE_2D
				/*glBindTexture(tex->mImageData.target, tex->texId);
				glTexParameteri(tex->mImageData.target, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
				vglUnloadImage(&tex->mImageData);*/
				glBindTexture(GL_TEXTURE_2D, tex->texId);

				GLint property_loc = glGetUniformLocation(render_scene_prog, (itr->name + "_ST").c_str());
				glUniform4f(property_loc, itr->value1, itr->value2, itr->value3, itr->value4);

				++nTexIndex;
			}
			break;
			case MPT_FLOAT:
			{
				GLint property_loc = glGetUniformLocation(render_scene_prog, (itr->name).c_str());
				glUniform1f(property_loc, itr->value1);
			}
			break;
			case MPT_FLOAT2:
			{
				GLint property_loc = glGetUniformLocation(render_scene_prog, (itr->name).c_str());
				glUniform2f(property_loc, itr->value1, itr->value2);
			}
			break;
			case MPT_FLOAT3:
			{
				GLint property_loc = glGetUniformLocation(render_scene_prog, (itr->name).c_str());
				glUniform3f(property_loc, itr->value1, itr->value2, itr->value3);
			}
			break;
			case MPT_FLOAT4:
			{
				GLint property_loc = glGetUniformLocation(render_scene_prog, (itr->name).c_str());
				glUniform4f(property_loc, itr->value1, itr->value2, itr->value3, itr->value4);
			}
			break;
			default:
				break;
			}
		}

		render_scene_uniforms.render_model_matrix_loc = glGetUniformLocation(render_scene_prog, "model_matrix");
		render_scene_uniforms.render_view_matrix_loc = glGetUniformLocation(render_scene_prog, "view_matrix");
		render_scene_uniforms.render_projection_matrix_loc = glGetUniformLocation(render_scene_prog, "projection_matrix");
		render_scene_uniforms.render_mvp_matrix_loc = glGetUniformLocation(render_scene_prog, "mvp_matrix");
		render_scene_uniforms.render_shadow_matrix_loc = glGetUniformLocation(render_scene_prog, "shadow_matrix");

		// FOG TODO: ��ȡ�����				Uniform Block ����
		GLint property_loc = glGetUniformLocation(render_scene_prog, "useFog");
		GLint nUseFog = HXSceneManager::GetInstance()->fog->useFog;
		glUniform1i(property_loc, nUseFog);
		if (nUseFog == 1)
		{
			property_loc = glGetUniformLocation(render_scene_prog, "fogType");
			GLint nfogType = HXSceneManager::GetInstance()->fog->fogType;
			glUniform1i(property_loc, nfogType);

			property_loc = glGetUniformLocation(render_scene_prog, "fogColor");
			HXCOLOR color = HXSceneManager::GetInstance()->fog->fogColor;
			glUniform3f(property_loc, color.r / 255.0f, color.g / 255.0f, color.b / 255.0f);

			if (nfogType == HXFogType::FOG_Linear)
			{
				HXFogLinear* fogLinear = (HXFogLinear*)HXSceneManager::GetInstance()->fog;

				property_loc = glGetUniformLocation(render_scene_prog, "fogStart");
				GLfloat nfogStart = fogLinear->fogStart;
				glUniform1f(property_loc, nfogStart);

				property_loc = glGetUniformLocation(render_scene_prog, "fogEnd");
				GLfloat nfogEnd = fogLinear->fogEnd;
				glUniform1f(property_loc, nfogEnd);
			}
		}

		property_loc = glGetUniformLocation(render_scene_prog, "ambient");
		HXCOLOR color = HXSceneManager::GetInstance()->ambient;
		glUniform3f(property_loc, color.r / 255.0f, color.g / 255.0f, color.b / 255.0f);

		property_loc = glGetUniformLocation(render_scene_prog, "eyePos");
		HXGLCamera* pCamera = (HXGLCamera*)HXSceneManager::GetInstance()->GetMainCamera();
		if (pCamera)
		{
			HXVector3D eyePos = pCamera->mSrcEye;
			glUniform3f(property_loc, eyePos.x, eyePos.y, eyePos.z);
		}
		

		// light
		for (int index = 0; index < HXSceneManager::GetInstance()->lightVct.size(); ++index)
		{
			HXLight* light = HXSceneManager::GetInstance()->lightVct[index];
			if (light)
			{
				std::stringstream ss;
				std::string strIndex;
				ss << index;
				ss >> strIndex;
				std::string strLight = "Lights[" + strIndex + "]";
				property_loc = glGetUniformLocation(render_scene_prog, (strLight + ".isEnable").c_str());
				GLint isEnable = light->enable;
				glUniform1i(property_loc, isEnable);

				property_loc = glGetUniformLocation(render_scene_prog, (strLight + ".lightType").c_str());
				GLint lightType = light->lightType;
				glUniform1i(property_loc, lightType);

				property_loc = glGetUniformLocation(render_scene_prog, (strLight + ".lightColor").c_str());
				HXCOLOR lightColor = light->color;
				glUniform3f(property_loc, lightColor.r / 255.0f, lightColor.g / 255.0f, lightColor.b / 255.0f);

				property_loc = glGetUniformLocation(render_scene_prog, (strLight + ".lightDir").c_str());
				HXVector3D lightDir = light->direct;
				glUniform3f(property_loc, lightDir.x, lightDir.y, lightDir.z);

				property_loc = glGetUniformLocation(render_scene_prog, (strLight + ".shininess").c_str());
				GLfloat shininess = light->shininess;
				glUniform1f(property_loc, shininess);

				property_loc = glGetUniformLocation(render_scene_prog, (strLight + ".strength").c_str());
				GLfloat strength = light->strength;
				glUniform1f(property_loc, strength);

				property_loc = glGetUniformLocation(render_scene_prog, (strLight + ".lightPos").c_str());
				HXVector3D lightPos = light->position;
				glUniform3f(property_loc, lightPos.x, lightPos.y, lightPos.z);

				property_loc = glGetUniformLocation(render_scene_prog, (strLight + ".constantAttenuation").c_str());
				GLfloat constantAttenuation = light->constantAttenuation;
				glUniform1f(property_loc, constantAttenuation);

				property_loc = glGetUniformLocation(render_scene_prog, (strLight + ".LinearAttenuation").c_str());
				GLfloat LinearAttenuation = light->LinearAttenuation;
				glUniform1f(property_loc, LinearAttenuation);

				property_loc = glGetUniformLocation(render_scene_prog, (strLight + ".QuadraticAttenuation").c_str());
				GLfloat QuadraticAttenuation = light->QuadraticAttenuation;
				glUniform1f(property_loc, QuadraticAttenuation);

				property_loc = glGetUniformLocation(render_scene_prog, (strLight + ".SpotCosCutoff").c_str());
				GLfloat SpotCosCutoff = light->SpotCosCutoff;
				glUniform1f(property_loc, SpotCosCutoff);

				property_loc = glGetUniformLocation(render_scene_prog, (strLight + ".SpotExponent").c_str());
				GLfloat SpotExponent = light->SpotExponent;
				glUniform1f(property_loc, SpotExponent);
				
				property_loc = glGetUniformLocation(render_scene_prog, (strLight + ".ConeDirection").c_str());
				HXVector3D ConeDirection = light->ConeDirection;
				glUniform3f(property_loc, ConeDirection.x, ConeDirection.y, ConeDirection.z);
			}
		}

		glVertexAttribPointer(vPosition, 3, GL_FLOAT, GL_FALSE, 0, (const void*)(0));
		glEnableVertexAttribArray(vPosition);
		glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, (const void*)(nSizePositions));
		glEnableVertexAttribArray(vColor);
		glVertexAttribPointer(vUV, 2, GL_FLOAT, GL_FALSE, 0, (const void*)(nSizePositions + nSizeColors));
		glEnableVertexAttribArray(vUV);
		glVertexAttribPointer(vNormal, 3, GL_FLOAT, GL_FALSE, 0, (const void*)(nSizePositions + nSizeColors + nSizeUVs));
		glEnableVertexAttribArray(vNormal);

		// һ��Ҫ���ϣ���Ȼ��������VBO���Ƶ������޷���ʾ
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glUseProgram(0);
	}

	

	// TODO: ��ȡ��rendersystem��ֻ�� modelMatrix glDrawArrays
	void HXGLRenderable::RenderShadowMap()
	{
		if (m_pSubMesh->triangleCount > MAX_TRIANGLE_COUNT)
		{
			//std::cerr << "Mesh over max triangle count" << std::endl;
			return;
		}

		// TODO: ����
		HXGLRenderSystem* rs = (HXGLRenderSystem*)HXRoot::GetInstance()->GetRenderSystem();
		HXGLShadowMap* sm = rs->mShadowMap;

		/*float t = float(GetTickCount() & 0xFFFF) / float(0xFFFF);
		static float q = 0.0f;
		static const vmath::vec3 X(1.0f, 0.0f, 0.0f);
		static const vmath::vec3 Y(0.0f, 1.0f, 0.0f);
		static const vmath::vec3 Z(0.0f, 0.0f, 1.0f);

		vmath::vec3 light_position = vmath::vec3(sinf(t * 6.0f * 3.141592f) * 300.0f, 200.0f, cosf(t * 4.0f * 3.141592f) * 100.0f + 250.0f);

		

		vmath::mat4 light_view_matrix = vmath::lookat(light_position, vmath::vec3(0.0f), Y);
		vmath::mat4 light_projection_matrix(vmath::frustum(-1.0f, 1.0f, -1.0f, 1.0f, 1.0f, FRUSTUM_DEPTH));

		glUseProgram(render_light_prog);*/
		
		glUniformMatrix4fv(sm->render_light_uniforms.model_view_projection_matrix, 1, GL_FALSE, sm->light_projection_matrix * sm->light_view_matrix * mMatrixModel);
		//glUniformMatrix4fv(render_light_uniforms.model_view_projection_matrix, 1, GL_FALSE, scene_projection_matrix * scene_view_matrix * scene_model_matrix);

		//glBindFramebuffer(GL_FRAMEBUFFER, depth_fbo);
		//glViewport(0, 0, DEPTH_TEXTURE_SIZE, DEPTH_TEXTURE_SIZE);

		//// Clear
		//glClearDepth(1.0f);
		//glClear(GL_DEPTH_BUFFER_BIT);

		//glEnable(GL_POLYGON_OFFSET_FILL);
		//glPolygonOffset(2.0f, 4.0f);

		glBindVertexArray(mVAO);
		glDrawArrays(GL_TRIANGLES, 0, m_pSubMesh->triangleCount * 3);
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		//glUseProgram(0);

		//glDisable(GL_POLYGON_OFFSET_FILL);
	}

	void HXGLRenderable::Render()
	{
		if (m_pSubMesh->triangleCount > MAX_TRIANGLE_COUNT)
		{
			//std::cerr << "Mesh over max triangle count" << std::endl;
			return;
		}
		
		glEnable(GL_CULL_FACE);
		glFrontFace(GL_CCW);
		//glFrontFace(GL_CW);

		// TODO:��ȡ��material�����ļ���
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		
		glBindVertexArray(mVAO);
		glUseProgram(render_scene_prog);
		glEnable(GL_TEXTURE_2D);

		if (m_pSubMesh->IsStaticMesh == false)
		{
			glBindBuffer(GL_ARRAY_BUFFER, mVBO);

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


		// ÿ����Ⱦ��״̬��Ҫ���¸�ֵ TODO: ����״̬��ȡ
		HXMaterialInfo* pMatInfo = HXResourceManager::GetInstance()->GetMaterialInfo(m_pSubMesh->materialName);
		int nTexIndex = 0;
		for (std::vector<HXMaterialProperty>::iterator itr = pMatInfo->vctMatProperty.begin(); itr != pMatInfo->vctMatProperty.end(); ++itr)
		{
			switch (itr->type)
			{
			case MPT_TEXTURE:
			{
				GLint tex_uniform_loc = glGetUniformLocation(render_scene_prog, (itr->name).c_str());
				if (tex_uniform_loc == -1)
				{
					// δ�α�ʵ�ʵ��õı��������ᱻ�Զ�ɾ��
					continue;
				}

				HXGLTexture* tex = (HXGLTexture*)HXResourceManager::GetInstance()->GetTexture("GL_" + itr->value);

				glUniform1i(tex_uniform_loc, nTexIndex);
				glActiveTexture(GL_TEXTURE0 + nTexIndex);
				//glBindTexture(tex->mImageData.target, tex->texId);
				glBindTexture(GL_TEXTURE_2D, tex->texId);

				GLint property_loc = glGetUniformLocation(render_scene_prog, (itr->name + "_ST").c_str());
				glUniform4f(property_loc, itr->value1, itr->value2, itr->value3, itr->value4);

				++nTexIndex;
			}
			break;
			case MPT_FLOAT:
			{
				GLint property_loc = glGetUniformLocation(render_scene_prog, (itr->name).c_str());
				glUniform1f(property_loc, itr->value1);
			}
			break;
			case MPT_FLOAT2:
			{
				GLint property_loc = glGetUniformLocation(render_scene_prog, (itr->name).c_str());
				glUniform2f(property_loc, itr->value1, itr->value2);
			}
			break;
			case MPT_FLOAT3:
			{
				GLint property_loc = glGetUniformLocation(render_scene_prog, (itr->name).c_str());
				glUniform3f(property_loc, itr->value1, itr->value2, itr->value3);
			}
			break;
			case MPT_FLOAT4:
			{
				GLint property_loc = glGetUniformLocation(render_scene_prog, (itr->name).c_str());
				glUniform4f(property_loc, itr->value1, itr->value2, itr->value3, itr->value4);
			}
			break;
			default:
				break;
			}
		}

		glUniformMatrix4fv(render_scene_uniforms.render_model_matrix_loc, 1, GL_FALSE, mMatrixModel);
		glUniformMatrix4fv(render_scene_uniforms.render_view_matrix_loc, 1, GL_FALSE, mMatrixView);
		glUniformMatrix4fv(render_scene_uniforms.render_projection_matrix_loc, 1, GL_FALSE, mMatrixProjection);
		glUniformMatrix4fv(render_scene_uniforms.render_mvp_matrix_loc, 1, GL_FALSE, mMatrixProjection * mMatrixView * mMatrixModel);

		// FOG TODO: ��ȡ�����				Uniform Block ����
		GLint property_loc = glGetUniformLocation(render_scene_prog, "useFog");
		GLint nUseFog = HXSceneManager::GetInstance()->fog->useFog;
		glUniform1i(property_loc, nUseFog);
		if (nUseFog == 1)
		{
			property_loc = glGetUniformLocation(render_scene_prog, "fogType");
			GLint nfogType = HXSceneManager::GetInstance()->fog->fogType;
			glUniform1i(property_loc, nfogType);

			property_loc = glGetUniformLocation(render_scene_prog, "fogColor");
			HXCOLOR color = HXSceneManager::GetInstance()->fog->fogColor;
			glUniform3f(property_loc, color.r / 255.0f, color.g / 255.0f, color.b / 255.0f);

			if (nfogType == HXFogType::FOG_Linear)
			{
				HXFogLinear* fogLinear = (HXFogLinear*)HXSceneManager::GetInstance()->fog;

				property_loc = glGetUniformLocation(render_scene_prog, "fogStart");
				GLfloat nfogStart = fogLinear->fogStart;
				glUniform1f(property_loc, nfogStart);

				property_loc = glGetUniformLocation(render_scene_prog, "fogEnd");
				GLfloat nfogEnd = fogLinear->fogEnd;
				glUniform1f(property_loc, nfogEnd);
			}
		}

		property_loc = glGetUniformLocation(render_scene_prog, "ambient");
		HXCOLOR color = HXSceneManager::GetInstance()->ambient;
		glUniform3f(property_loc, color.r / 255.0f, color.g / 255.0f, color.b / 255.0f);

		property_loc = glGetUniformLocation(render_scene_prog, "eyePos");
		HXGLCamera* pCamera = (HXGLCamera*)HXSceneManager::GetInstance()->GetMainCamera();
		HXVector3D eyePos = pCamera->mSrcEye;
		glUniform3f(property_loc, eyePos.x, eyePos.y, eyePos.z);

		// shadow
		// TODO: ����
		HXGLRenderSystem* rs = (HXGLRenderSystem*)HXRoot::GetInstance()->GetRenderSystem();
		HXGLShadowMap* sm = rs->mShadowMap;
		if (sm)
		{
			const vmath::mat4 scale_bias_matrix = vmath::mat4(	vmath::vec4(0.5f, 0.0f, 0.0f, 0.0f),
																vmath::vec4(0.0f, 0.5f, 0.0f, 0.0f),
																vmath::vec4(0.0f, 0.0f, 0.5f, 0.0f),
																vmath::vec4(0.5f, 0.5f, 0.5f, 1.0f));
			glUniformMatrix4fv(render_scene_uniforms.render_shadow_matrix_loc, 1, GL_FALSE, scale_bias_matrix * sm->light_projection_matrix * sm->light_view_matrix);

			GLint tex_uniform_loc = glGetUniformLocation(render_scene_prog, "depth_texture");
			glUniform1i(tex_uniform_loc, nTexIndex);
			glActiveTexture(GL_TEXTURE0 + nTexIndex);
			//glBindTexture(tex->mImageData.target, tex->texId);
			glBindTexture(GL_TEXTURE_2D, sm->depth_texture);
			++nTexIndex;
		}


		glDrawArrays(GL_TRIANGLES, 0, m_pSubMesh->triangleCount * 3);
		
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glUseProgram(0);

		// ��� HXGLRenderSystem �ѵ���
		//glutSwapBuffers();
		//glutPostRedisplay();
	}
}
