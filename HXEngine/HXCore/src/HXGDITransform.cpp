#include "..\include\HXGDITransform.h"
#include "HXMath.h"

namespace HX3D
{
	HXGDITransform::HXGDITransform()
	{
	}

	HXGDITransform::~HXGDITransform()
	{
	}

	void HXGDITransform::CaculateModelMatrix(HXMatrix44& fatherModelMatrix)
	{
		// ���Ŵ���S
		HXMatrix44 matS = GetScaleMatrix44(mLocalScale.x, mLocalScale.y, mLocalScale.z);
		// ��ת����Q
		HXMatrix44 matX = GetRotateMatrix44X(mLocalEulerDegree.x);
		HXMatrix44 matY = GetRotateMatrix44Y(mLocalEulerDegree.y);
		HXMatrix44 matZ = GetRotateMatrix44Z(mLocalEulerDegree.z);
		// ƽ��һ��Ҫ�����T
		HXMatrix44 matT = GetTranslateMatrix44(mLocalPostion.x, mLocalPostion.y, mLocalPostion.z);

		// ģ�Ϳռ䵽����ռ�ת�� SQT
		// HXMatrix44 mat = matS*matX*matY*matZ*matT;
		HXMatrix44 mat = matS*matY*matZ*matX*matT;

		mGlobalModelMatrix = mat * fatherModelMatrix;
	}

	void HXGDITransform::CaculateModelMatrix()
	{
		// ���Ŵ���S
		HXMatrix44 matS = GetScaleMatrix44(mLocalScale.x, mLocalScale.y, mLocalScale.z);
		// ��ת����Q
		HXMatrix44 matX = GetRotateMatrix44X(mLocalEulerDegree.x);
		HXMatrix44 matY = GetRotateMatrix44Y(mLocalEulerDegree.y);
		HXMatrix44 matZ = GetRotateMatrix44Z(mLocalEulerDegree.z);
		// ƽ��һ��Ҫ�����T
		HXMatrix44 matT = GetTranslateMatrix44(mLocalPostion.x, mLocalPostion.y, mLocalPostion.z);

		// ģ�Ϳռ䵽����ռ�ת�� SQT
		// HXMatrix44 mat = matS*matX*matY*matZ*matT;
		HXMatrix44 mat = matS*matY*matZ*matX*matT;

		mGlobalModelMatrix = mat;
	}
}