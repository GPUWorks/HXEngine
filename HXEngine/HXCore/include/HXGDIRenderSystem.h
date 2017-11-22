#pragma once
#include "HXCommon.h"
#include "HXRenderSystem.h"
#include <windows.h>

namespace HX3D
{
	class HXGDIRenderSystem : public HXRenderSystem
	{
	public:
		HXGDIRenderSystem();
		~HXGDIRenderSystem();

		virtual void CreateRenderWindow(std::string strName, int nWidth, int nHeight, bool bFullScreen);
		virtual void MainLoop();
		static void Display(HDC hdc);
		static void Reshape(int nWidth, int nHeight);

		virtual HXRenderable* GenerateRenderable(HXSubMesh* pSubMesh);
		virtual void RenderSingle(HXRenderable* pRenderable);

		virtual HXICamera* CreateCamera(const HXVector3D& eye, const HXVector3D& at, const HXVector3D& up,
			float ffov, float nearZ, float farZ, float viewportWidth, float viewportHeigth,
			float left, float right, float bottom, float top);

		//virtual void BeginText();
		virtual void _DrawText(float x, float y, std::string text, const HXCOLOR& color = HXCOLOR(255,255,255,255));
		//virtual void EndText();

	private:
		// 2�����ڹ��̺���(����Ҫ��ȫ�ֺ���)
		static LRESULT CALLBACK MyWindowProc(
			_In_ HWND   hwnd,
			_In_ UINT   uMsg,
			_In_ WPARAM wParam,
			_In_ LPARAM lParam
		);
		// 3��ע�ᴰ������
		BOOL RegisterMyWindow(LPCSTR lpcWndName, HINSTANCE hInstance);
		// 4���������ڣ�lpClassName һ�����Ѿ�ע����Ĵ������ͣ�
		HWND CreateMyWindow(LPCTSTR lpClassName, LPCTSTR lpWinName, HINSTANCE hInstance);
		// 5�� ��ʾ����
		void DisplayMyWnd(HWND hWnd);
		// ˢ�´���
		void UpdateMyWnd(HWND hWnd);

		HWND m_hWnd;
	};
}


