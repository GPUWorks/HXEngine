#pragma once
#include "HXCommon.h"
#include "HXVector.h"

namespace HX3D
{
	struct HXPlane3D 
	{
		HXVector4D point;	// ����һ��
		HXVector4D normal;	// ���Ϸ���

		HXPlane3D() {}
		HXPlane3D(const HXVector4D& p, const HXVector4D& n);
		HXPlane3D& operator=(const HXPlane3D& rhs);
	};
}