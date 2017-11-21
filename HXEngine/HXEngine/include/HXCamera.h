#pragma once
#include "HXCommon.h"

namespace HX3D
{
	struct HXFrustum;

	enum RenderMode
	{
		RENDER_WIRE,	// �߿���Ⱦ
		RENDER_SOILD,	// ʵ����Ⱦ
	};
	// �����ģ���ʹ��HXICamera��
	class HXCamera
	{
	public:
		~HXCamera();

		void setNearZ(float fNearZ);
		void setFarZ(float fFarZ);

		void update();
		// �����������
		void setPosition(const HXVector4D& pos);
		// Ŀ�����������
		void setTargetPos(const HXVector4D& pos);
		// ����ƶ�
		void move(const HXVector4D& mov);
		// ��Y����ת
		void yaw(float fDegree);
		// ��X����ת
		void pitch(float fDegree);

	//protected:
		// TODO: ֻ������Ԫ�� �����������������
		HXCamera();

	//private:
		HXFrustum* mFrustum;
		bool mNeedUpdate;
	};
}
