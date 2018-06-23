#pragma once
#include "HXCommon.h"
#include "HXICamera.h"

namespace HX3D
{
	struct HXFrustum;
	// TODO: Frustum Cull  nearClip farClip etc.
	class HXGDICamera : public HXICamera
	{
	public:
		HXGDICamera();
		~HXGDICamera();

		virtual void Initialize(const HXVector3D& position, const HXVector3D& rotate,
			float nearZ, float farZ);

		virtual void Update();
		virtual void OnViewPortResize(int nScreenWidth, int nScreenHeight);

		// ����ƶ�
		virtual void move(const HXVector3D& mov);
		// ��Y����ת
		virtual void yaw(float fDegree);
		// ��X����ת
		virtual void pitch(float fDegree);

		HXFrustum* mFrustum;
	};
}
