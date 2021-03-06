#pragma once
#include "HXCommon.h"
namespace HX3D
{
	class HXStatus
	{
	private:
		static HXStatus* pInstance;
		HXStatus();
	public:
		static HXStatus* GetInstance()
		{
			if (NULL == pInstance)
			{
				pInstance = new HXStatus();
			}
			return pInstance;
		}
		~HXStatus();

		void ResetStatus();
		void ShowStatusInfo();

		int nTriangleCount;
		int nVertexCount;

		int nDrawCall;
		int nBatchCall;


		bool showStatus;
	private:
		// 计算并显现上一秒帧率
		float CalcLastFPS();

	private:
		// 上一秒FPS
		double mLastFPS;
		double mLastTime;
		int	mFrameCount;
		
	};
}

