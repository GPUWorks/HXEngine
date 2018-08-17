#pragma once
#include "HXCommon.h"
#include "HXVector.h"
#include "HXMatrix.h"
#include "HXQuaternionOld.h"

namespace HX3D
{
	class HXGameObject;
	class HXITransform
	{
	public:
		HXITransform();
		virtual ~HXITransform();

		void SetLocalPosition(const HXVector3D& pos);
		const HXVector3D& GetLocalPosition();
		virtual HXVector3D GetGlobalPosition() = 0;
		void Move(const HXVector3D& mov);

		void SetLocalRotation(const HXVector3D& rotation);
		const HXVector3D& GetLocalRotation();
		//virtual HXQuaternionOld GetGlobalRotation() = 0;
		void Pitch(float fDegree);	// ��X����ת
		void Yaw(float fDegree);	// ��Y����ת
		void Roll(float fDegree);	// ��Z����ת

		void SetLocalScale(const HXVector3D& scale);
		const HXVector3D& GetLocalScale();
		virtual HXVector3D GetGlobalScale() = 0;

		virtual void CaculateModelMatrix(HXMatrix44& fatherModelMatrix) = 0;
		virtual void CaculateModelMatrix() = 0;

		void AddChild(HXITransform* child);
		std::vector<HXITransform*>& GetChildren();





		const HXMatrix44& GetLocalMatrix();
		void SetLocalMatrix(const HXMatrix44& mat);


		void UpdateLocalMatrix();









		HXGameObject* gameObject;
		HXITransform* parent;
		std::vector<HXITransform*> vctChildren;

		HXVector3D mLocalPostion;
		HXQuaternionOld mLocalRotation;
		HXVector3D mLocalScale;
		HXVector3D mLocalEuler;
		HXMatrix44 mLocalMatrix;

		HXVector3D mPostion;
		HXQuaternionOld mRotation;
		HXVector3D mScale;
		HXMatrix44 mWorldMatrix;

		bool _localQuaternionUpdate;
		bool _locaEulerlUpdate;
		bool _localUpdate;
		bool _worldUpdate;
		bool _positionUpdate;
		bool _rotationUpdate;
		bool _scaleUpdate;


		HXMatrix44 mGlobalModelMatrix;
	};
}