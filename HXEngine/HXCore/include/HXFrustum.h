#pragma once
#include "HXCommon.h"
#include "HXVector.h"
#include "HXPlane.h"
#include "HXMatrix.h"

namespace HX3D
{
	// UVN���
	struct HXFrustum
	{
		HXFrustum(const HXVector4D& pos, const HXVector4D& target
			, float ffov, float nearZ, float farZ
			, float viewportWidth, float viewportHeigth);

		void update();

		HXVector4D	camPosition;	// �����������
		HXVector4D	lookTarget;	// ���ע�ӵ�

		HXVector4D	camU;	// �һ�����
		HXVector4D	camV;	// �ϻ�����
		HXVector4D	camN;	// ��������ϵ��OpenGl����N�Ǻ󷽣���������ϵ��D3D����N��ǰ��      (Z�᷽��).

		float	view_dist;	// �ӵ㵽ͶӰƽ��ľ��루�� fov �� viewplane_width ���������
		float	fov;	// ˮƽ������Ұ�Ƕ�
		float	clip_z_near;	// ���ý���
		float	clip_z_far;		// Զ�ý���
		HXPlane3D	clip_plane_R;	// �Ҳý���
		HXPlane3D	clip_plane_L;	// ��ý���
		HXPlane3D	clip_plane_T;	// �ϲý���
		HXPlane3D	clip_plane_B;	// �²ý���

		float	viewplane_width;	// ͶӰƽ���	// �̶�Ϊ2
		float	viewplane_height;	// ͶӰƽ���	// ���ڹ�һ��ͶӰ, Ϊ2X2�� �����߱�����Ļ���ڿ�߱���ͬ

		float	viewport_width;		// ��Ϸ��Ļ��
		float	viewport_heigth;	// ��Ϸ��Ļ��
		float	viewport_center_X;	// ��Ϸ��Ļ��������
		float	viewport_center_Y;	// ��Ϸ��Ļ��������
		float	aspect_ratio;		// ��Ϸ��Ļ��߱�

		HXMatrix44	matWorldToCamera;	// ��������ϵ���������ϵת������
	};
}


