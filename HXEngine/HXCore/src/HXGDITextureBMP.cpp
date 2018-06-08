#include "..\include\HXGDITextureBMP.h"

namespace HX3D
{
	HXGDITextureBMP::HXGDITextureBMP(std::string strTextureFile)
	{
		// ��PNG  DDS ת ΪBMP��β���ļ���
		size_t size = strTextureFile.length();
		strTextureFile = strTextureFile.substr(0, size - 3) + "bmp";

		mBitmap = new HXBitmap;
		if (!mBitmap->LoadBitmapFile(strTextureFile))
		{
			delete mBitmap;
		}
	}

	HXGDITextureBMP::~HXGDITextureBMP()
	{
		if (mBitmap)
		{
			delete mBitmap;
			mBitmap = NULL;
		}
	}
}