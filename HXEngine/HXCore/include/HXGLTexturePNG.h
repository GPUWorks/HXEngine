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
	class HXGLTexturePNG : public HXITexture
	{
	public:
		HXGLTexturePNG(std::string strPNGFile);
		~HXGLTexturePNG();


		gl_texture_t * ReadPNGFromFile(const char *filename);
		void GetPNGtextureInfo(int color_type, gl_texture_t *texinfo);
		GLuint loadPNGTexture(const char *filename);

		// �����������ݱ������Դ����ˣ�ϵͳ�ڴ治��Ҫ�ٱ���һ��
		//gl_texture_t mImageData;
		GLuint texId;
	};
}