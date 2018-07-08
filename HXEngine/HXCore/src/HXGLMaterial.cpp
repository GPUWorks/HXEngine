#include "..\include\HXGLMaterial.h"
#include "HXGLTexture.h"
#include "LoadShaders.h"
#include "HXResourceManager.h"
#include "HXSceneManager.h"
#include "HXFogLinear.h"
#include "HXGLCamera.h"
#include "HXGLTransform.h"
#include "HXRoot.h"
#include "HXGLRenderSystem.h"
#include "HXGLShadowMap.h"
#include "HXGLRenderable.h"

namespace HX3D
{
	HXGLMaterial::HXGLMaterial(HXMaterialInfo* pMatInfo)
	{
		HXMaterial::SetMaterialInfo(pMatInfo);

		std::string strVertShaderFile = pMatInfo->strShaderFile + ".vert";
		std::string strFragShaderFile = pMatInfo->strShaderFile + ".frag";

		ShaderInfo shaders[] = {
			{ GL_VERTEX_SHADER, strVertShaderFile.c_str() },
			{ GL_FRAGMENT_SHADER, strFragShaderFile.c_str() },
			{ GL_NONE, NULL }
		};

		render_scene_prog = LoadShaders(shaders);
		glUseProgram(render_scene_prog);

		// ÿ֡���µı���
		render_scene_uniforms.render_eye_pos_loc = glGetUniformLocation(render_scene_prog, "eyePos");
		render_scene_uniforms.render_model_matrix_loc = glGetUniformLocation(render_scene_prog, "model_matrix");
		render_scene_uniforms.render_view_matrix_loc = glGetUniformLocation(render_scene_prog, "view_matrix");
		render_scene_uniforms.render_projection_matrix_loc = glGetUniformLocation(render_scene_prog, "projection_matrix");
		render_scene_uniforms.render_mvp_matrix_loc = glGetUniformLocation(render_scene_prog, "mvp_matrix");
		render_scene_uniforms.render_shadow_matrix_loc = glGetUniformLocation(render_scene_prog, "shadow_matrix");

		// ����Ҫÿ֡���µı��������༭��ģʽ�£�
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
				if (property_loc != -1)
				{
					glUniform1f(property_loc, itr->value1);
				}
			}
			break;
			case MPT_FLOAT2:
			{
				GLint property_loc = glGetUniformLocation(render_scene_prog, (itr->name).c_str());
				if (property_loc != -1)
				{
					glUniform2f(property_loc, itr->value1, itr->value2);
				}
			}
			break;
			case MPT_FLOAT3:
			{
				GLint property_loc = glGetUniformLocation(render_scene_prog, (itr->name).c_str());
				if (property_loc != -1)
				{
					glUniform3f(property_loc, itr->value1, itr->value2, itr->value3);
				}
			}
			break;
			case MPT_FLOAT4:
			{
				GLint property_loc = glGetUniformLocation(render_scene_prog, (itr->name).c_str());
				if (property_loc != -1)
				{
					glUniform4f(property_loc, itr->value1, itr->value2, itr->value3, itr->value4);
				}
			}
			break;
			default:
				break;
			}
		}

		// FOG TODO: Uniform Block ����
		GLint property_loc = glGetUniformLocation(render_scene_prog, "useFog");
		if (property_loc != -1)
		{
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
		}
		
		property_loc = glGetUniformLocation(render_scene_prog, "ambient");
		HXCOLOR color = HXSceneManager::GetInstance()->ambient;
		glUniform3f(property_loc, color.r / 255.0f, color.g / 255.0f, color.b / 255.0f);

		// light
		for (int index = 0; index < HXSceneManager::GetInstance()->lightVct.size(); ++index)
		{
			HXLight* light = HXSceneManager::GetInstance()->lightVct[index];
			if (light && light->enable)
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

		// shadow
		// ÿ֡��Ҫ����
		//GLint tex_uniform_loc = glGetUniformLocation(render_scene_prog, "depth_texture");
		//if (tex_uniform_loc != -1)
		//{
		//	HXGLRenderSystem* rs = (HXGLRenderSystem*)HXRoot::GetInstance()->GetRenderSystem();
		//	HXGLShadowMap* sm = rs->mShadowMap;
		//	if (sm)
		//	{
		//		const vmath::mat4 scale_bias_matrix = vmath::mat4(vmath::vec4(0.5f, 0.0f, 0.0f, 0.0f),
		//		vmath::vec4(0.0f, 0.5f, 0.0f, 0.0f),
		//		vmath::vec4(0.0f, 0.0f, 0.5f, 0.0f),
		//		vmath::vec4(0.5f, 0.5f, 0.5f, 1.0f));
		//		glUniformMatrix4fv(render_scene_uniforms.render_shadow_matrix_loc, 1, GL_FALSE, scale_bias_matrix * sm->light_projection_matrix * sm->light_view_matrix);

		//		glUniform1i(tex_uniform_loc, nTexIndex);
		//		glActiveTexture(GL_TEXTURE0 + nTexIndex);
		//		//glBindTexture(tex->mImageData.target, tex->texId);
		//		glBindTexture(GL_TEXTURE_2D, sm->depth_texture);
		//		++nTexIndex;
		//	}
		//}

		glUseProgram(0);
	}

	HXGLMaterial::~HXGLMaterial()
	{
		glDeleteProgram(render_scene_prog);
	}

	void HXGLMaterial::SetMaterialRenderState(HXGLRenderable* renderable)
	{
		// TODO:��ȡ��material�����ļ���
		glEnable(GL_CULL_FACE);
		glFrontFace(GL_CCW);
		//glFrontFace(GL_CW);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_TEXTURE_2D);

		glUseProgram(render_scene_prog);

		HXGLCamera* pCamera = (HXGLCamera*)HXSceneManager::GetInstance()->GetMainCamera();
		HXVector3D eyePos = pCamera->transform->mPostion;
		glUniform3f(render_scene_uniforms.render_eye_pos_loc, eyePos.x, eyePos.y, eyePos.z);
		glUniformMatrix4fv(render_scene_uniforms.render_model_matrix_loc, 1, GL_FALSE, renderable->mMatrixModel);
		glUniformMatrix4fv(render_scene_uniforms.render_view_matrix_loc, 1, GL_FALSE, renderable->mMatrixView);
		glUniformMatrix4fv(render_scene_uniforms.render_projection_matrix_loc, 1, GL_FALSE, renderable->mMatrixProjection);
		glUniformMatrix4fv(render_scene_uniforms.render_mvp_matrix_loc, 1, GL_FALSE, renderable->mMatrixProjection * renderable->mMatrixView * renderable->mMatrixModel);

		// ÿ����Ⱦ��״̬��Ҫ���¸�ֵ TODO: ����״̬��ȡ
		HXMaterialInfo* pMatInfo = mMatInfo;
		int nTexIndex = 0;
		for (std::vector<HXMaterialProperty>::iterator itr = pMatInfo->vctMatProperty.begin(); itr != pMatInfo->vctMatProperty.end(); ++itr)
		{
			switch (itr->type)
			{
			case MPT_TEXTURE:
			{
				//ÿ֡��Ⱦʱshader uniform��texture������Ҫ���´�����ֵ��ֻ������󶨵㣬����󶨵�󶨵�������ܱ��ĵ��ˣ�
				GLint tex_uniform_loc = glGetUniformLocation(render_scene_prog, (itr->name).c_str());
				if (tex_uniform_loc == -1)
				{
					// δ�α�ʵ�ʵ��õı��������ᱻ�Զ�ɾ��
					continue;
				}

				HXGLTexture* tex = (HXGLTexture*)HXResourceManager::GetInstance()->GetTexture("GL_" + itr->value);

				glUniform1i(tex_uniform_loc, nTexIndex);
				glActiveTexture(GL_TEXTURE0 + nTexIndex);
				glBindTexture(GL_TEXTURE_2D, tex->texId);

				if (HXRoot::GetInstance()->IsEditorMode())
				{
					GLint property_loc = glGetUniformLocation(render_scene_prog, (itr->name + "_ST").c_str());
					glUniform4f(property_loc, itr->value1, itr->value2, itr->value3, itr->value4);
				}
				++nTexIndex;
			}
			break;
			if (HXRoot::GetInstance()->IsEditorMode())
			{
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
			}
			default:
				break;
			}
		}

		// shadow
		GLint tex_uniform_loc = glGetUniformLocation(render_scene_prog, "depth_texture");
		if (tex_uniform_loc != -1)
		{
			HXGLRenderSystem* rs = (HXGLRenderSystem*)HXRoot::GetInstance()->GetRenderSystem();
			HXGLShadowMap* sm = rs->mShadowMap;
			if (sm)
			{
				// ÿ֡��Ҫ����
				const vmath::mat4 scale_bias_matrix = vmath::mat4(vmath::vec4(0.5f, 0.0f, 0.0f, 0.0f),
																  vmath::vec4(0.0f, 0.5f, 0.0f, 0.0f),
																  vmath::vec4(0.0f, 0.0f, 0.5f, 0.0f),
																  vmath::vec4(0.5f, 0.5f, 0.5f, 1.0f));
				glUniformMatrix4fv(render_scene_uniforms.render_shadow_matrix_loc, 1, GL_FALSE, scale_bias_matrix * sm->light_projection_matrix * sm->light_view_matrix);
				
				glUniform1i(tex_uniform_loc, nTexIndex);
				glActiveTexture(GL_TEXTURE0 + nTexIndex);
				glBindTexture(GL_TEXTURE_2D, sm->depth_texture);
				++nTexIndex;
			}
		}

		if (HXRoot::GetInstance()->IsEditorMode())
		{
			// FOG TODO: Uniform Block ����
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

			// light
			for (int index = 0; index < HXSceneManager::GetInstance()->lightVct.size(); ++index)
			{
				HXLight* light = HXSceneManager::GetInstance()->lightVct[index];
				if (light && light->enable)
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

			property_loc = glGetUniformLocation(render_scene_prog, "ambient");
			HXCOLOR color = HXSceneManager::GetInstance()->ambient;
			glUniform3f(property_loc, color.r / 255.0f, color.g / 255.0f, color.b / 255.0f);
		}
	}

	void HXGLMaterial::SetShadowMapMaterialRenderState(HXGLRenderable* renderable)
	{
		HXGLRenderSystem* rs = (HXGLRenderSystem*)HXRoot::GetInstance()->GetRenderSystem();
		HXGLShadowMap* sm = rs->mShadowMap;

		glUniformMatrix4fv(sm->render_light_uniforms.model_view_projection_matrix, 1, GL_FALSE, sm->light_projection_matrix * sm->light_view_matrix * renderable->mMatrixModel);
		
		// Alpha test
		int nTexIndex = 0;
		for (std::vector<HXMaterialProperty>::iterator itr = mMatInfo->vctMatProperty.begin(); itr != mMatInfo->vctMatProperty.end(); ++itr)
		{
			switch (itr->type)
			{
			case MPT_TEXTURE:
			{
				GLint tex_uniform_loc = glGetUniformLocation(sm->render_light_prog, (itr->name).c_str());
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

				GLint property_loc = glGetUniformLocation(sm->render_light_prog, (itr->name + "_ST").c_str());
				glUniform4f(property_loc, itr->value1, itr->value2, itr->value3, itr->value4);
				
				return;
			}
			break;
			default:
				break;
			}
		}
	}
}
