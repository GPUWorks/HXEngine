#pragma once
#include "HXCommon.h"
#include "HXMatrix.h"
#include "HXVector.h"

namespace HX3D
{
	template<class T>
	const T& Min(const T& a, const T& b)
	{
		return (a < b) ? a : b;
	}

	template<class T>
	const T& Max(const T& a, const T& b)
	{
		return (a > b) ? a : b;
	}

	template<class T>
	const T& Clamp(const T& val, const T& maxv, const T& minv)
	{
		return Max(Min(val, maxv), minv);
	}

	HXMatrix44 GetTranslateMatrix44(float tx, float ty, float tz);

	HXMatrix44 GetScaleMatrix44(float sx, float sy, float sz);

	// TODO: �Ժ�����Ԫ������ת �����Ա仯���ڴ�ռ���٣�
	// pitch ��X����ת
	HXMatrix44 GetRotateMatrix44X(float fDegree);

	// yaw ��Y����ת
	HXMatrix44 GetRotateMatrix44Y(float fDegree);

	// roll ��Z����ת
	HXMatrix44 GetRotateMatrix44Z(float fDegree);

	// ����������ת
	HXMatrix44 GetRotateMatrix44(const HXVector4D& axis, float fDegree);

	// 3D���� X 44����
	HXVector3D GetVector3DMulMatrix44(const HXVector3D& vec, const HXMatrix44& mat);

	// 4D���� X 44����
	HXVector4D GetVector4DMulMatrix44(const HXVector4D& vec, const HXMatrix44& mat);

	// 3D���� X 33����
	HXVector3D GetVector3DMulMatrix33(const HXVector3D& vec, const HXMatrix33& mat);
}

