#pragma once
#include "HXCommon.h"
#include "HXITexture.h"
#include "vgl.h"

namespace HX3D
{
	class HXGLTexture : public HXITexture
	{
	public:
		HXGLTexture(std::string strTextureFile);
		~HXGLTexture();

		// PNG
		GLuint LoadPNGTexture(const char *filename);
		
		// DDS
		GLuint LoadDDSTexture(const char *filename);


		// �����������ݱ������Դ����ˣ�ϵͳ�ڴ治��Ҫ�ٱ���һ��
		GLuint texId;
	};
}


