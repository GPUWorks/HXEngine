#pragma once
#include "HXCommon.h"

namespace HX3D
{
	class HXVector3
	{
	public:
		HXVector3();
		~HXVector3();

		float x() { return elements[0]; }
		void x(float v) { elements[0] = v; }
		float y() { return elements[1]; }
		void y(float v) { elements[1] = v; }
		float z() { return elements[2]; }
		void z(float v) { elements[2] = v; }


		/**
		* ��һ����ά������
		* @param	s Դ��ά������
		* @param	out �����ά������
		*/
		static void normalize(const HXVector3& s, HXVector3& out);

		/**
		* ��������ά�����Ĳ
		* @param	a  left��ά������
		* @param	b  right��ά������
		* @param	o out ���������
		*/
		static void subtract(const HXVector3& a, const HXVector3& b, HXVector3& o);

		/**
		* ��������ά�����Ĳ�ˡ�
		* @param	a left������
		* @param	b right������
		* @param	o ���������
		*/
		static void cross(const HXVector3& a, const  HXVector3& b, HXVector3& o);

		/**
		* ��������ά�����ĵ����
		* @param	a left������
		* @param	b right������
		* @return   �����
		*/
		static float dot(const HXVector3& a, const HXVector3& b);





		/**��ά����Ԫ������*/
		float elements[3];
	};
}