#pragma once
#include <windows.h>
#include <d2d1.h>

class Renderer
{
	public:
		Renderer();
		~Renderer();

		bool Initialize(HWND hwnd);
		void Render();
		void Cleanup();


    private:
		ID2D1Factory* m_factory = nullptr;
		ID2D1HwndRenderTarget* m_renderTarget = nullptr;
		ID2D1SolidColorBrush* m_brush = nullptr;
};