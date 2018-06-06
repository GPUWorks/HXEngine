#pragma once
#include "HXCommon.h"
#include "HXITexture.h"
#include "vgl.h"

namespace HX3D
{
	class gl_texture_t
	{
	public:
		GLsizei width;
		GLsizei height;
		GLenum format;
		GLint internalFormat;
		GLuint id;
		GLubyte* texels;
	};
	class HXGLTexture : public HXITexture
	{
	public:
		HXGLTexture(std::string strTextureFile);
		~HXGLTexture();

		// PNG
		GLuint LoadPNGTexture(const char *filename);
		gl_texture_t * ReadPNGFromFile(const char *filename);
		void GetPNGtextureInfo(int color_type, gl_texture_t *texinfo);

		// DDS
		GLuint LoadDDSTexture(const char *filename);


		// �����������ݱ������Դ����ˣ�ϵͳ�ڴ治��Ҫ�ٱ���һ��
		GLuint texId;
	};
}


