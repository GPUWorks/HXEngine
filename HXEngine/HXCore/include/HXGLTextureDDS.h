#pragma once
#include "HXCommon.h"
#include "HXITexture.h"
#include "vermilion.h"

namespace HX3D
{ 
	class HXGLTextureDDS : public HXITexture
	{
	public:
		HXGLTextureDDS(std::string strDDSFile);
		~HXGLTextureDDS();

		// �����������ݱ������Դ����ˣ�ϵͳ�ڴ治��Ҫ�ٱ���һ��
		//vglImageData mImageData;
		GLuint texId;
	};
}


