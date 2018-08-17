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

	HXVector3D HXGDITransform::GetGlobalPosition()
	{
		// TODO: �����,�ο�HXGLTransform
		return GetVector3DMulMatrix44(mLocalPostion, mGlobalModelMatrix);
	}

	HXVector3D HXGDITransform::GetGlobalRotation()
	{
		// TODO: �����,�ο�HXGLTransform
		return GetVector3DMulMatrix44(mLocalEuler, mGlobalModelMatrix);
	}

	HXVector3D HXGDITransform::GetGlobalScale()
	{
		// TODO: �����,�ο�HXGLTransform
		return GetVector3DMulMatrix44(mLocalScale, mGlobalModelMatrix);
	}

	void HXGDITransform::CaculateModelMatrix(HXMatrix44& fatherModelMatrix)
	{
		// ���Ŵ���S
		HXMatrix44 matS = GetScaleMatrix44(mLocalScale.x, mLocalScale.y, mLocalScale.z);
		// ��ת����Q
		HXMatrix44 matX = GetRotateMatrix44X(mLocalEuler.x);
		HXMatrix44 matY = GetRotateMatrix44Y(mLocalEuler.y);
		HXMatrix44 matZ = GetRotateMatrix44Z(mLocalEuler.z);
		// ƽ��һ��Ҫ�����T
		HXMatrix44 matT = GetTranslateMatrix44(mLocalPostion.x, mLocalPostion.y, mLocalPostion.z);

		// ģ�Ϳռ䵽����ռ�ת�� SQT
		HXMatrix44 mat = matS*matY*matZ*matX*matT;

		mGlobalModelMatrix = mat * fatherModelMatrix;
	}

	void HXGDITransform::CaculateModelMatrix()
	{
		// ���Ŵ���S
		HXMatrix44 matS = GetScaleMatrix44(mLocalScale.x, mLocalScale.y, mLocalScale.z);
		// ��ת����Q
		HXMatrix44 matX = GetRotateMatrix44X(mLocalEuler.x);
		HXMatrix44 matY = GetRotateMatrix44Y(mLocalEuler.y);
		HXMatrix44 matZ = GetRotateMatrix44Z(mLocalEuler.z);
		// ƽ��һ��Ҫ�����T
		HXMatrix44 matT = GetTranslateMatrix44(mLocalPostion.x, mLocalPostion.y, mLocalPostion.z);

		// ģ�Ϳռ䵽����ռ�ת�� SQT
		HXMatrix44 mat = matS*matY*matZ*matX*matT;

		mGlobalModelMatrix = mat;
	}
}