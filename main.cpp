//main.cpp :  アプリケーションのエントリポイント
#include <Windows.h>
#include "Renderer.h"

LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdSHow)
{
	const wchar_t CLASS_NAME[] = L"R_engine";

	WNDCLASS wc = {};
	wc.lpfnWndProc = WndProc;
	wc.hInstance = hInstance;
	wc.lpszClassName = CLASS_NAME;
	RegisterClass(&wc);

	HWND hwnd = CreateWindowEx(0, CLASS_NAME, L"R_engine", WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, 800, 600, nullptr, nullptr, hInstance, nullptr);

	if (!hwnd)
	{
		return 0;
	}

	ShowWindow(hwnd, nCmdSHow);

	Renderer renderer;
	if (!renderer.Initialize(hwnd)) {
		MessageBox(hwnd, L"Renderer initialization failed", L"Error", MB_OK);
		return 0;
	}

	MSG msg = {};
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			renderer.Render();
		}
	}

	renderer.Cleanup();
	return 0;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, msg, wParam, lParam);
	
}