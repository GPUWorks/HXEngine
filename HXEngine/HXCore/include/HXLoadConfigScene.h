#pragma once
#include "HXCommon.h"
#include "HXVector.h"
#include "HXFogBase.h"
#include "HXLight.h"

namespace HX3D
{
	struct HXPrefabGameObjInfo
	{
		std::string strGameObjName;
		std::string strPrefabFile;
		int nPriority;
		bool bCastShadow;
		HXVector3D position;
		HXVector3D rotation;
		HXVector3D scale;
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
		// TODO: Add Camera etc.
		std::vector<HXPrefabGameObjInfo> vctGameObjInfo;
		HXFogInfo fogInfo;
		HXCOLOR ambient;
		std::vector<HXLightInfo> vctLight;
	};

	class HXLoadConfigScene
	{
	public:
		HXLoadConfigScene();
		~HXLoadConfigScene();

		bool LoadFile(std::string strFileName);

		HXSceneInfo mSceneInfo;
	};
}