#pragma once
#include "HXCommon.h"
#include "HXITexture.h"
#include "vgl.h"
#include "HXLoadConfigMat.h"

namespace HX3D
{
	class HXGLTexture : public HXITexture
	{
	public:
		HXGLTexture(EMatPropertyType matType, std::string strTextureFile);
		~HXGLTexture();

		// PNG
		GLuint LoadPNGTexture(const char *filename);
		GLuint LoadPNGTextureCubeMap(const char *filename);
		
		// DDS
		GLuint LoadDDSTexture(const char *filename);
		GLuint LoadDDSTextureCubeMap(const char *filename);

		// 
		GLuint CreateFloat32Texture(int32_t width, int32_t height, bool enableMipmapping = true);

		// �����������ݱ������Դ����ˣ�ϵͳ�ڴ治��Ҫ�ٱ���һ��
		GLuint texId;
	};
}


