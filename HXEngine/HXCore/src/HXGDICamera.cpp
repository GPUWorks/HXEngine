#include "..\include\HXGDICamera.h"
#include "HXFrustum.h"
#include "HXMath.h"
#include "HXQuaternion.h"

namespace HX3D
{
	HXGDICamera::HXGDICamera():mFrustum(NULL)
	{
	}


	HXGDICamera::~HXGDICamera()
	{
		if (mFrustum)
		{
			delete mFrustum;
			mFrustum = NULL;
		}
	}

	void HXGDICamera::Initialize(const HXVector3D& eye, const HXVector3D& at, const HXVector3D& up,
		float ffov, float nearZ, float farZ, float viewportWidth, float viewportHeigth,
		float left, float right, float bottom, float top)
	{
		mFrustum = new HXFrustum(HXVector4D(eye.x, eye.y, eye.z, 1), HXVector4D(at.x, at.y, at.z, 1)
			, ffov,nearZ, farZ, viewportWidth, viewportHeigth);
	}

	void HXGDICamera::Update()
	{
		if (mFrustum)
		{
			mFrustum->update();
		}
	}

	void HXGDICamera::OnViewPortResize(int nScreenWidth, int nScreenHeight)
	{

	}

	void HXGDICamera::move(const HXVector3D& mov)
	{
		//mFrustum->camPosition += HXVector4D(mov.x, mov.y, mov.z, 1);
		//mFrustum->lookTarget += HXVector4D(mov.x, mov.y, mov.z, 1);

		HXQuaternion q;
		q.FromEulerDegree(mFrustum->mPitch, mFrustum->mYaw, mFrustum->mRoll);
		HXVector3D v = mov;
		v = q.Transform(v);
		mFrustum->camPosition += HXVector4D(v, 0);
		mFrustum->lookTarget += HXVector4D(v, 0);
	}

	void HXGDICamera::yaw(float fDegree)
	{
		/*HXVector4D distance = mFrustum->lookTarget - mFrustum->camPosition;
		HXMatrix44 matRotate = GetRotateMatrix44Y(fDegree);
		distance = GetVector4DMulMatrix44(distance, matRotate);
		mFrustum->lookTarget = mFrustum->camPosition + distance;*/

		mFrustum->mYaw += fDegree;
	}

	void HXGDICamera::pitch(float fDegree)
	{
		/*HXVector4D distance = mFrustum->lookTarget - mFrustum->camPosition;
		HXMatrix44 matRotate = GetRotateMatrix44X(fDegree);
		distance = GetVector4DMulMatrix44(distance, matRotate);
		mFrustum->lookTarget = mFrustum->camPosition + distance;*/

		mFrustum->mPitch += fDegree;
	}

	void HXGDICamera::Forward(float fDistance)
	{
		/*HXVector4D direction = mFrustum->lookTarget - mFrustum->camPosition;
		direction.normalize();
		HXVector4D forward = direction * fDistance;
		mFrustum->camPosition += forward;
		mFrustum->lookTarget += forward;*/

		HXQuaternion q;
		q.FromEulerDegree(mFrustum->mPitch, mFrustum->mYaw, mFrustum->mRoll);
		HXVector3D v(0, 0, -fDistance);
		v = q.Transform(v);
		mFrustum->camPosition += HXVector4D(v,0);
		mFrustum->lookTarget += HXVector4D(v, 0);
	}

	void HXGDICamera::MoveHorizon(float fDistance)
	{
		HXQuaternion q;
		q.FromEulerDegree(mFrustum->mPitch, mFrustum->mYaw, mFrustum->mRoll);
		HXVector3D v(fDistance, 0, 0);
		v = q.Transform(v);
		mFrustum->camPosition += HXVector4D(v, 0);
		mFrustum->lookTarget += HXVector4D(v, 0);
	}

	void HXGDICamera::MoveVertical(float fDistance)
	{
		HXQuaternion q;
		q.FromEulerDegree(mFrustum->mPitch, mFrustum->mYaw, mFrustum->mRoll);
		HXVector3D v(0, -fDistance, 0);
		v = q.Transform(v);
		mFrustum->camPosition += HXVector4D(v, 0);
		mFrustum->lookTarget += HXVector4D(v, 0);
	}
}