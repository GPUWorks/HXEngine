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
		HXMatrix44 matS = GetScaleMatrix44(mScale.x, mScale.y, mScale.z);
		// ��ת����Q
		HXMatrix44 matX = GetRotateMatrix44X(mEulerDegree.x);
		HXMatrix44 matY = GetRotateMatrix44Y(mEulerDegree.y);
		HXMatrix44 matZ = GetRotateMatrix44Z(mEulerDegree.z);
		// ƽ��һ��Ҫ�����T
		HXMatrix44 matT = GetTranslateMatrix44(mPostion.x, mPostion.y, mPostion.z);

		// ģ�Ϳռ䵽����ռ�ת�� SQT
		// HXMatrix44 mat = matS*matX*matY*matZ*matT;
		HXMatrix44 mat = matS*matY*matZ*matX*matT;

		mCurModelMatrix = mat * fatherModelMatrix;
	}

	void HXGDITransform::CaculateModelMatrix()
	{
		// ���Ŵ���S
		HXMatrix44 matS = GetScaleMatrix44(mScale.x, mScale.y, mScale.z);
		// ��ת����Q
		HXMatrix44 matX = GetRotateMatrix44X(mEulerDegree.x);
		HXMatrix44 matY = GetRotateMatrix44Y(mEulerDegree.y);
		HXMatrix44 matZ = GetRotateMatrix44Z(mEulerDegree.z);
		// ƽ��һ��Ҫ�����T
		HXMatrix44 matT = GetTranslateMatrix44(mPostion.x, mPostion.y, mPostion.z);

		// ģ�Ϳռ䵽����ռ�ת�� SQT
		// HXMatrix44 mat = matS*matX*matY*matZ*matT;
		HXMatrix44 mat = matS*matY*matZ*matX*matT;

		mCurModelMatrix = mat;
	}
}