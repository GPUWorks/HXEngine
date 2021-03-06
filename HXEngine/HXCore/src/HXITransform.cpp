#include "..\include\HXITransform.h"
#include "HXGameObject.h"

namespace HX3D
{
	HXITransform::HXITransform()
	{
		mLocalPostion = HXVector3D(0,0,0);
		mLocalEuler = HXVector3D(0,0,0);
		mLocalScale = HXVector3D(1,1,1);
	}

	HXITransform::~HXITransform()
	{
	}

	void HXITransform::SetLocalPosition(const HXVector3D& pos)
	{
		mLocalPostion = pos;
	}

	const HXVector3D& HXITransform::GetLocalPosition()
	{
		return mLocalPostion;
	}

	void HXITransform::Move(const HXVector3D& mov)
	{
		mLocalPostion += mov;
	}

	void HXITransform::SetLocalRotation(const HXVector3D& rotation)
	{
		mLocalEuler = rotation;
	}

	const HXVector3D& HXITransform::GetLocalRotation()
	{
		return mLocalEuler;
	}

	void HXITransform::Pitch(float fDegree)
	{
		mLocalEuler.x = fDegree;
	}

	void HXITransform::Yaw(float fDegree)
	{
		mLocalEuler.y = fDegree;
	}

	void HXITransform::Roll(float fDegree)
	{
		mLocalEuler.z = fDegree;
	}

	void HXITransform::SetLocalScale(const HXVector3D& scale)
	{
		mLocalScale = scale;
	}

	const HXVector3D& HXITransform::GetLocalScale()
	{
		return mLocalScale;
	}

	void HXITransform::AddChild(HXITransform* child)
	{
		vctChildren.push_back(child);
		child->parent = this;
	}

	void HXITransform::RemoveChild(HXITransform* child)
	{
		for (std::vector<HXITransform*>::iterator itr = vctChildren.begin(); itr != vctChildren.end(); ++itr)
		{
			if ((*itr) == child)
			{
				vctChildren.erase(itr);
				child->parent = NULL;
				return;
			}
		}
	}

	std::vector<HXITransform*>& HXITransform::GetChildren()
	{
		return vctChildren;
	}


	// New
	const HXMatrix44& HXITransform::GetLocalMatrix()
	{
		if (_localUpdate)
		{
			UpdateLocalMatrix();
			_localUpdate = false;
		}
		return mLocalMatrix;
	}

	void HXITransform::SetLocalMatrix(const HXMatrix44& mat)
	{
		mLocalMatrix = mat;
		mLocalMatrix.DecomposeTransRotScale(mLocalPostion, mLocalRotation, mLocalScale);
		_localUpdate = false;
		OnWorldTransform();
	}

	void HXITransform::UpdateLocalMatrix()
	{
		mLocalMatrix = HXMatrix44::CreateAffineTransformation(mLocalPostion, mLocalRotation, mLocalScale);
	}

	void HXITransform::OnWorldTransform()
	{
		if (!_worldUpdate || !_positionUpdate || !_rotationUpdate || !_scaleUpdate) {
			_worldUpdate = _positionUpdate = _rotationUpdate = _scaleUpdate = true;
			// event(Event.WORLDMATRIX_NEEDCHANGE);
			for (std::vector<HXITransform*>::iterator itr = vctChildren.begin(); itr != vctChildren.end(); ++itr)
			{
				(*itr)->OnWorldTransform();
			}
		}
	}
}