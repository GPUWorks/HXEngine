#pragma once
#include "HXCommon.h"
#include "HXVector.h"
#include "HXMesh.h"

namespace HX3D
{
#define EOBJECT_STATE_CULLED	0x0001

	class HXGameObject;
	struct HXFrustum;
	class HXLight;

	struct HXTriangle
	{		
		HXVertex vertexList[3];
	};

	struct HXRenderList 
	{
		std::list<HXTriangle> triangleList;
		// typedef std::list<HXTriangle>::iterator Itr;
	};
	
	// CullSphere(ֻ������ĵ�ת������ͼ�ռ�����ж�)
	void Frustum_Culling_GameObject(HXGameObject* pGameObject);
	// �����ģ�Ϳռ䵽����ռ�ת�������뵽��Ⱦ�б�
	void Local_To_World_RenderList(HXGameObject* pGameObject, HXRenderList* pRenderList);
	// ����ռ䱳���������޳�
	void Culling_Backface_RenderList(HXRenderList* pRenderList, HXFrustum* pFrustum);
	void World_Space_Vertex_Lighting(HXRenderList* pRenderList, std::vector<HXLight*>* pLightVct);
	void World_To_Camera_RenderList(HXRenderList* pRenderList, HXFrustum* pFrustum);
	void Camera_To_Project_RenderList(HXRenderList* pRenderList, HXFrustum* pFrustum);
	void Project_To_ViewPort_RenderList(HXRenderList* pRenderList, HXFrustum* pFrustum);
	void Draw_RenderList_Wire(HXRenderList* pRenderList);
	void Draw_RenderList_Texture_Solid(HXRenderList* pRenderList);
}


