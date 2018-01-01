#pragma once
#include "HXCommon.h"
#include "HXICamera.h"
#include "vgl.h"
#include "vmath.h"
#include "HXVector.h"
#include "HXQuaternion.h"

namespace HX3D
{
	class HXGLCamera : public HXICamera
	{
	public:
		HXGLCamera();
		~HXGLCamera();

		virtual void Initialize(const HXVector3D& eye, const HXVector3D& at, const HXVector3D& up,
			float ffov, float nearZ, float farZ, float viewportWidth, float viewportHeigth,
			float left, float right, float bottom, float top);

		virtual void Update();
		void OnViewPortResize(int nScreenWidth, int nScreenHeight);
	
		// ����ƶ�
		virtual void move(const HXVector3D& mov);
		// ��Y����ת
		virtual void yaw(float fDegree);
		// ��X����ת
		virtual void pitch(float fDegree);

		// Obsolete/////////////////////////////////////
		//virtual void YawLockTarget(float fDegree);
		//virtual void PitchLockTarget(float fDegree);
		////////////////////////////////////////////////

		virtual void Forward(float fDistance);
		virtual void MoveHorizon(float fDistance);
		virtual void MoveVertical(float fDistance);

		vmath::mat4 mMatrixView;
		vmath::mat4 mMatrixProjection;

	
		void UpdateViewMatrix(const HXVector3D& eye, const HXVector3D& at, const HXVector3D& up);
		void UpdateProjectionMatrix(float left, float right, float bottom, float top, float n, float f);

		// ��������ת
		HXVector3D mSrcEye;
		HXVector3D mSrcAt;
		HXVector3D mSrcUp;
		// ��ת��������
		float mYaw;
		float mPitch;
		float mRoll;

		float mLeft;
		float mRight;
		float mBottom; 
		float mTop;
		float mNear;
		float mFar;

		
	};
}


