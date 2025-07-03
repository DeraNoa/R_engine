#pragma once
#include <windows.h>
#include <d2d1.h>
#include <wincodec.h>
#include <dwrite.h>
#include <string>

class Renderer
{
	public:
		Renderer();
		~Renderer();

		bool Initialize(HWND hwnd);
		void Render();
		bool LoadBackgroundImage(const wchar_t* imagePath);
		void Cleanup();



    private:
		/*DrawBox*/
		ID2D1Factory* m_factory = nullptr;
		ID2D1HwndRenderTarget* m_renderTarget = nullptr;
		ID2D1SolidColorBrush* m_brush = nullptr;
		
		/*DrawGroundImage*/
		IWICImagingFactory* m_wicFactory = nullptr;
		ID2D1Bitmap* m_backgroundBitmap = nullptr;

		/*DrawText*/
		// �����o�ϐ��ǉ�
		IDWriteFactory* m_dwriteFactory = nullptr;
		IDWriteTextFormat* m_textFormat = nullptr;
		std::wstring m_dialogText = L"�x���������肷��ۂ̃��[�g��?";
};