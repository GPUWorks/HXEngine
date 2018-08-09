#pragma once
#include "HXCommon.h"
#include "HXVector.h"
#include "HXFogBase.h"
#include "HXLight.h"
#include "tinyxml.h"
#include "HXLoadConfigPrefab.h"

namespace HX3D
{
	struct HXCameraInfo
	{
		std::string name;
		HXVector3D position;
		HXVector3D rotation;
		float nearZ;
		float farZ;
		EClearFlag clearFlag;
		HXCOLOR background;
		ECullingMask cullingMask;
		ECameraProjection projection;
		int depth;
	};
	struct HXFogInfo
	{
		bool bUse;
		HXFogType type;
		HXCOLOR color;
		float start;
		float end;
	};
	struct HXLightInfo
	{
		std::string name;

		bool isEnable;
		LIGHT_TYPE lightType;
		HXCOLOR lightColor;
		HXVector3D lightDir;
		float shininess;	// �߹����̶�ָ��
		float strength;		// ���ڵ�������ȵ�����

		HXVector3D lightPos;
		float constantAttenuation;	// ˥��ϵ��
		float LinearAttenuation;
		float QuadraticAttenuation;

		HXVector3D ConeDirection;	// �۹��
		float SpotCosCutoff;
		float SpotExponent;
	};
	struct HXSceneInfo
	{
		HXSceneInfo() {};
		~HXSceneInfo()
		{
			// ��������������ݹ�ɾ��
			for (std::vector<HXGameObjectInfo*>::iterator itr = vctGameObjInfo.begin(); itr != vctGameObjInfo.end(); ++itr)
			{
				delete (*itr);
			}
		}
		// TODO: Add other etc.
		std::vector<HXGameObjectInfo*> vctGameObjInfo;
		HXCameraInfo cameraInfo;
		HXFogInfo fogInfo;
		HXCOLOR ambient;
		std::vector<HXLightInfo> vctLight;
		std::vector<HXCameraInfo> vctCamera;
	};

	class HXLoadConfigScene
	{
	public:
		HXLoadConfigScene();
		~HXLoadConfigScene();

		bool ReadGameObjectRecurve(TiXmlElement* gameobjElement, std::vector<HXGameObjectInfo*>& list);
		bool LoadFile(std::string strFileName);

		HXSceneInfo mSceneInfo;
	};
}