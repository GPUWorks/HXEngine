#pragma once
#include "HXCommon.h"

namespace HX3D
{
	class MathUtils3D
	{
	public:

		/**�����ȸ���(float)����ݲ�*/
		static const float zeroTolerance;
		/**������Ĭ�����ֵ*/
		static const float MaxValue;
		/**������Ĭ����Сֵ*/
		static const float MinValue;
	
		MathUtils3D();
		~MathUtils3D();

		/**
		* �Ƿ����ݲ�ķ�Χ�ڽ�����0
		* @param  �ж�ֵ
		* @return  �Ƿ������0
		*/
		static bool isZero(float v)
		{
			return abs(v) < zeroTolerance;
		}

		/**
		* ����ֵ�Ƿ����ݲ�ķ�Χ�ڽ������Sqr Magnitude
		* @param  �ж�ֵ
		* @return  �Ƿ������0
		*/
		static bool nearEqual(float n1, float n2)
		{
			if (isZero(n1 - n2))
				return true;
			return false;
		}

		static float fastInvSqrt(float value)
		{
			if (isZero(value))
				return value;
			return 1.0 / sqrt(value);
		}
	};
}