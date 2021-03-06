#pragma once
#include "HXCommon.h"
#include "HXVector.h"

namespace HX3D
{
	//class HXGameObject;
	class HXITransform;
	class HXICamera
	{
	public:
		HXICamera() {};
		virtual ~HXICamera() {};

		virtual void Initialize(const HXVector3D& position, const HXVector3D& rotate,
			float nearZ, float farZ) = 0;

		virtual void Update() = 0;
		virtual void OnViewPortResize(int nScreenWidth, int nScreenHeight) = 0;

		//virtual HXVector3D WorldToScreenPoint(HXVector3D position);
		//virtual HXVector3D ScreenToWorldPoint(HXVector3D position);
		//virtual HXVector3D WorldToViewportPoint(HXVector3D position);	//([0,1],[0,1])
		//virtual HXVector3D ViewportToWorldPoint(HXVector3D position);
		virtual HXVector3D WorldToNDCPoint(HXVector3D position) { return position; };	//([-1,1],[-1,1])
		virtual HXVector3D NDCToWorldPoint(HXVector3D position) { return position; };

		// 相机移动
		virtual void move(const HXVector3D& mov) = 0;
		// 绕Y轴旋转
		virtual void yaw(float fDegree) = 0;
		// 绕X轴旋转
		virtual void pitch(float fDegree) = 0;

		/*virtual void YawLockTarget(float fDegree) = 0;
		virtual void PitchLockTarget(float fDegree) = 0;*/

		/*virtual void Forward(float fDistance) = 0;
		virtual void MoveHorizon(float fDistance) = 0;
		virtual void MoveVertical(float fDistance) = 0;*/

		/*inline void AttachGameObject(HXGameObject* go) { followedGO.push_back(go); };
		inline void DetachGameObject(HXGameObject* go) { followedGO.remove(go); };
		std::list<HXGameObject*> followedGO;*/

		virtual bool PreRender() { return true; };
		virtual void PostRender() {};
		virtual bool PreRenderShadowMap() { return true; };
		virtual void PostRenderShadowMap() {};

		HXITransform* transform;

		std::string name;
		bool enable;
		EClearFlag clearFlag;
		HXCOLOR background;
		ECullingMask cullingMask;
		ECameraProjection projection;
		float mField;	// 竖直方向角度
		float mSize;	// 竖直方向高度
		float mNear;
		float mFar;
		float mViewportRectX;
		float mViewportRectY;
		float mViewportRectW;
		float mViewportRectH;
		int depth;
	};
}
