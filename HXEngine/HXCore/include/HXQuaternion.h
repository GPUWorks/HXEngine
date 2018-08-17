#pragma once
#include "HXCommon.h"
#include "HXVector3.h"

namespace HX3D
{
	class HXMatrix4x4;
	class HXQuaternion
	{
	public:
		HXQuaternion();
		~HXQuaternion();

		/**
		*  ��ŷ����������Ԫ����˳��ΪYaw��Pitch��Roll��
		* @param	yaw yawֵ
		* @param	pitch pitchֵ
		* @param	roll rollֵ
		* @param	out �����Ԫ��
		*/
		static void createFromYawPitchRoll(float yaw, float pitch, float roll, HXQuaternion& out);

		/**
		*������Ԫ��Ϊ��λ����
		* @param out  �����Ԫ��
		*/
		void identity();

		/**
		*  ����ת���������Ԫ��
		* @param	mat ��ת����
		* @param	out  �����Ԫ��
		*/
		static void createFromMatrix4x4(const HXMatrix4x4& mat, HXQuaternion& out);

		/**��Ԫ��Ԫ������*/
		float elements[4];
	};
}