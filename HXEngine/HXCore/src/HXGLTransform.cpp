#include "..\include\HXGLTransform.h"
#include "HXMath.h"

namespace HX3D
{
	HXGLTransform::HXGLTransform()
	{
	}

	HXGLTransform::~HXGLTransform()
	{
	}

	vmath::mat4 HXGLTransform::ConvertMatrix(HXMatrix44& mat)
	{
		vmath::mat4 destMat4;

		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				destMat4[i][j] = mat.m[i][j];
			}
		}
		return destMat4;
	}

	HXMatrix44 HXGLTransform::UnConvertMatrix(vmath::mat4& mat)
	{
		HXMatrix44 destMat;

		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				destMat.m[i][j] = mat[i][j];
			}
		}
		return destMat;
	}

	HXVector3D HXGLTransform::GetGlobalPosition()
	{
		mPostion = GetVector3DMulMatrix44(HXVector3D(0,0,0), mGlobalModelMatrix);
		return mPostion;
	}

	HXVector3D HXGLTransform::GetGlobalRotation()
	{
		mEuler = mRotation.ToEulerDegree();
		return mEuler;

		//return mRotation;
	}

	HXVector3D HXGLTransform::GetGlobalScale()
	{
		return mScale;
	}

	void HXGLTransform::CaculateModelMatrix(HXMatrix44& fatherModelMatrix)
	{
		//// ת������������ϵ
		//vmath::mat4 fatherMatrix = ConvertMatrix(fatherModelMatrix);
		//
		//// ���Ŵ���S
		//vmath::mat4 matS = vmath::scale(mLocalScale.x, mLocalScale.y, mLocalScale.z);
		//// ��ת����Q
		//vmath::mat4 matX = vmath::rotate(mLocalEulerDegree.x, 1.0f, 0.0f, 0.0f);
		//vmath::mat4 matY = vmath::rotate(mLocalEulerDegree.y, 0.0f, 1.0f, 0.0f);
		//vmath::mat4 matZ = vmath::rotate(mLocalEulerDegree.z, 0.0f, 0.0f, 1.0f);
		//// ƽ��һ��Ҫ�����T
		//vmath::mat4 matT = vmath::translate(mLocalPostion.x, mLocalPostion.y, mLocalPostion.z);

		//// ģ�Ϳռ䵽����ռ�ת�� SQT(OPENGL��������ϵ������������)
		//vmath::mat4 localMatrixModel = matT*matX*matZ*matY*matS;
		//vmath::mat4 globalMatrixModel = fatherMatrix * localMatrixModel;

		//// ת������������ϵ����
		//mGlobalModelMatrix = UnConvertMatrix(globalMatrixModel);


		// �м���̲���Ҫת�����ת��GLSL�������;���
		// ���Ŵ���S
		HXMatrix44 matS = GetScaleMatrix44(mLocalScale.x, mLocalScale.y, mLocalScale.z);
		// ��ת����Q
		HXMatrix44 matX = GetRotateMatrix44X(mLocalEuler.x);
		HXMatrix44 matY = GetRotateMatrix44Y(mLocalEuler.y);
		HXMatrix44 matZ = GetRotateMatrix44Z(mLocalEuler.z);
		// ƽ��һ��Ҫ�����T
		HXMatrix44 matT = GetTranslateMatrix44(mLocalPostion.x, mLocalPostion.y, mLocalPostion.z);

		// ģ�Ϳռ䵽����ռ�ת�� SQT
		//mGlobalModelMatrix = matS*matY*matZ*matX*matT * fatherModelMatrix;
		mGlobalModelMatrix = matS*matY*matX*matZ*matT * fatherModelMatrix;


		mScale = mLocalScale * parent->mScale;

		HXQuaternionOld q;
		q.FromEulerDegree(mLocalEuler.x, mLocalEuler.y, mLocalEuler.z);
		mRotation = q * parent->mRotation;
	}

	void HXGLTransform::CaculateModelMatrix()
	{
		//// ���Ŵ���S
		//vmath::mat4 matS = vmath::scale(mLocalScale.x, mLocalScale.y, mLocalScale.z);
		//// ��ת����Q
		//vmath::mat4 matX = vmath::rotate(mLocalEulerDegree.x, 1.0f, 0.0f, 0.0f);
		//vmath::mat4 matY = vmath::rotate(mLocalEulerDegree.y, 0.0f, 1.0f, 0.0f);
		//vmath::mat4 matZ = vmath::rotate(mLocalEulerDegree.z, 0.0f, 0.0f, 1.0f);
		//// ƽ��һ��Ҫ�����T
		//vmath::mat4 matT = vmath::translate(mLocalPostion.x, mLocalPostion.y, mLocalPostion.z);

		//// ģ�Ϳռ䵽����ռ�ת�� SQT(OPENGL��������ϵ������������)
		//vmath::mat4 localMatrixModel = matT*matX*matZ*matY*matS;
		//vmath::mat4 globalMatrixModel = localMatrixModel;

		//// ת������������ϵ����
		//mGlobalModelMatrix = UnConvertMatrix(globalMatrixModel);


		// �м���̲���Ҫת�����ת��GLSL�������;���
		// ���Ŵ���S
		HXMatrix44 matS = GetScaleMatrix44(mLocalScale.x, mLocalScale.y, mLocalScale.z);
		// ��ת����Q
		HXMatrix44 matX = GetRotateMatrix44X(mLocalEuler.x);
		HXMatrix44 matY = GetRotateMatrix44Y(mLocalEuler.y);
		HXMatrix44 matZ = GetRotateMatrix44Z(mLocalEuler.z);
		// ƽ��һ��Ҫ�����T
		HXMatrix44 matT = GetTranslateMatrix44(mLocalPostion.x, mLocalPostion.y, mLocalPostion.z);

		// ģ�Ϳռ䵽����ռ�ת�� SQT
		//mGlobalModelMatrix = matS*matY*matZ*matX*matT;
		mGlobalModelMatrix = matS*matY*matX*matZ*matT;


		mScale = mLocalScale;

		HXQuaternionOld q;
		q.FromEulerDegree(mLocalEuler.x, mLocalEuler.y, mLocalEuler.z);
		mRotation = q;
	}
}
