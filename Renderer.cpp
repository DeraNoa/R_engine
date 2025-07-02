#include "Renderer.h"

Renderer::Renderer()
{
	// Initialize the renderer
}

Renderer::~Renderer()
{
    Cleanup();
	// Cleanup the renderer
}

bool Renderer::Initialize(HWND hwnd)
{
    D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_factory);
	
    RECT rc;
    GetClientRect(hwnd, &rc);
    D2D1_SIZE_U size = D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top);

    D2D1_RENDER_TARGET_PROPERTIES rtProps = D2D1::RenderTargetProperties();
    D2D1_HWND_RENDER_TARGET_PROPERTIES hwndProps = D2D1::HwndRenderTargetProperties(hwnd, size);

    if (FAILED(m_factory->CreateHwndRenderTarget(rtProps, hwndProps, &m_renderTarget))) {
        return false;
    }

    m_renderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::SkyBlue), &m_brush);
    return true;

	return true; // Return true if successful
}

void Renderer::Render() {
    m_renderTarget->BeginDraw();

    m_renderTarget->Clear(D2D1::ColorF(D2D1::ColorF::Black));
    m_renderTarget->FillRectangle(D2D1::RectF(100, 100, 300, 200), m_brush);

    m_renderTarget->EndDraw();
}

void Renderer::Cleanup() {
    if (m_brush) m_brush->Release();
    if (m_renderTarget) m_renderTarget->Release();
    if (m_factory) m_factory->Release();
}