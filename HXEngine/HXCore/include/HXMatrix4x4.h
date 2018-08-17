#pragma once
#include "HXCommon.h"
#include "HXVector3.h"
#include "HXQuaternion.h"

namespace HX3D
{
	class HXMatrix4x4
	{
	public:
		HXMatrix4x4();
		~HXMatrix4x4();

		/**
		* ��X����ת
		* @param	rad  ��ת�Ƕ�
		* @param	out �������
		*/
		static void createRotationX(float rad, HXMatrix4x4& out);

		/**
		*
		* ��Y����ת
		* @param	rad  ��ת�Ƕ�
		* @param	out �������
		*/
		static void createRotationY(float rad, HXMatrix4x4& out);

		/**
		* ��Z����ת
		* @param	rad  ��ת�Ƕ�
		* @param	out �������
		*/
		static void createRotationZ(float rad, HXMatrix4x4& out);

		/**
		* ͨ��yaw pitch roll��ת������ת����
		* @param	yaw
		* @param	pitch
		* @param	roll
		* @param	result
		*/
		static void createRotationYawPitchRoll(float yaw, float pitch, float roll, HXMatrix4x4& result);

		/**
		* ͨ����ת��axis����ת�Ƕ�angle������ת����
		* @param	axis ��ת��,�ٶ��Ѿ���һ����
		* @param	angle ��ת�Ƕȡ�
		* @param	result �������
		*/
		static void createRotationAxis(const HXVector3& axis, float angle, HXMatrix4x4& result);

		/**
		* ͨ����Ԫ��������ת����
		* @param	rotation ��ת��Ԫ����
		* @param	result �����ת����
		*/
		static void createRotationQuaternion(const HXQuaternion& rotation, HXMatrix4x4& result);

		/**
		* ����ƽ�Ƽ����������
		* @param	trans  ƽ������
		* @param	out �������
		*/
		static void createTranslate(const HXVector3& trans, HXMatrix4x4& out);

		/**
		* �������ż����������
		* @param	scale  ����ֵ
		* @param	out �������
		*/
		static void createScaling(const HXVector3& scale, HXMatrix4x4& out);

		/**
		* ������������ĳ˷�
		* @param	left left����
		* @param	right  right����
		* @param	out  �������
		*/
		static void multiply(HXMatrix4x4& left, HXMatrix4x4& right, HXMatrix4x4& out);

		/**
		* ����Ԫ��������ת����
		* @param	rotation ��Ԫ��
		* @param	out �������
		*/
		static void createFromQuaternion(const HXQuaternion& rotation, HXMatrix4x4& out);

		/**
		* ����������
		* @param	trans ƽ��
		* @param	rot ��ת
		* @param	scale ����
		* @param	out �������
		*/
		static void createAffineTransformation(const HXVector3& trans, const HXQuaternion& rot, const HXVector3& scale, HXMatrix4x4& out);
		
		/**
		*  ����۲����
		* @param	eye �ӵ�λ��
		* @param	center �ӵ�Ŀ��
		* @param	up ��������
		* @param	out �������
		*/
		static void createLookAt(const HXVector3& eye, const HXVector3& target, const HXVector3& up, HXMatrix4x4& out);

		/**
		* ����͸��ͶӰ����
		* @param	fov  �ӽǡ�
		* @param	aspect ���ݱȡ�
		* @param	near �����档
		* @param	far Զ���档
		* @param	out �������
		*/
		static void createPerspective(float fov, float aspect, float near, float far, HXMatrix4x4& out);

		/**
		* ��������ͶӰ����
		* @param	left ��׵��߽硣
		* @param	right ��׵�ұ߽硣
		* @param	bottom ��׵�ױ߽硣
		* @param	top ��׵���߽硣
		* @param	near ��׵���߽硣
		* @param	far ��׵Զ�߽硣
		* @param	out �������
		*/
		static void createOrthoOffCenterRH(float left, float right, float bottom, float top, float near, float far, HXMatrix4x4& out);

		/**����Ԫ������*/
		float elements[16];

		/**
		* ����һ�� <code>Matrix4x4</code> ʵ����
		* @param	4x4����ĸ�Ԫ��
		*/
		HXMatrix4x4(float m11 = 1, float m12 = 0, float m13 = 0, float m14 = 0
			, float m21 = 0, float m22 = 1, float m23 = 0, float m24 = 0
			, float m31 = 0, float m32 = 0, float m33 = 1, float m34 = 0
			, float m41 = 0, float m42 = 0, float m43 = 0, float m44 = 1);

		float getElementByRowColumn(int row, int column);

		void setElementByRowColumn(int row, int column, float value);

		/**
		* �ж�����4x4�����ֵ�Ƿ���ȡ�
		* @param	other 4x4����
		*/
		bool equalsOtherMatrix(const HXMatrix4x4& other) const;

		/**
		* �ֽ����Ϊƽ����������ת��Ԫ��������������
		* @param	translation ƽ��������
		* @param	rotation ��ת��Ԫ����
		* @param	scale ����������
		* @return �Ƿ�ֽ�ɹ���
		*/
		bool decomposeTransRotScale(HXVector3& translation, HXQuaternion& rotation, HXVector3& scale);

		/**
		* �ֽ����Ϊƽ����������ת��������������
		* @param	translation ƽ��������
		* @param	rotationMatrix ��ת����
		* @param	scale ����������
		* @return �Ƿ�ֽ�ɹ���
		*/
		bool decomposeTransRotMatScale(HXVector3& translation, HXMatrix4x4& rotationMatrix, HXVector3& scale);

		/**
		* �ֽ���ת�������תΪYawPitchRollŷ���ǡ�
		* @param	out float yaw
		* @param	out float pitch
		* @param	out float roll
		* @return
		*/
		void decomposeYawPitchRoll(HXVector3& yawPitchRoll);

		/**��һ������ */
		void normalize();

		/**��������ת�þ���*/
		HXMatrix4x4& transpose();

		/**
		* ����һ������������
		* @param	out �������
		*/
		void invert(HXMatrix4x4& out);

		/**���þ���Ϊ��λ����*/
		void identity();

		static void translation(const HXVector3& v3, HXMatrix4x4& out);

		/**
		* ��ȡƽ��������
		* @param	out ƽ��������
		*/
		void getTranslationVector(HXVector3& out) const;
		
		/**
		* ����ƽ��������
		* @param	translate ƽ��������
		*/
		void setTranslationVector(const HXVector3& translate);

		/**
		* ��ȡǰ������
		* @param	out ǰ������
		*/
		void getForward(HXVector3& out) const;

		/**
		* ����ǰ������
		* @param	forward ǰ������
		*/
		void setForward(const HXVector3& forward);


	private:
		static HXMatrix4x4 _tempMatrix4x4;
		static HXVector3 _tempVector0;
		static HXVector3 _tempVector1;
		static HXVector3 _tempVector2;
		static HXQuaternion _tempQuaternion;
	};
}