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
    CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);

    D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_factory);
	
    RECT rc;
    GetClientRect(hwnd, &rc);
    D2D1_SIZE_U size = D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top);

    D2D1_RENDER_TARGET_PROPERTIES rtProps = D2D1::RenderTargetProperties(); //DrawBox
    D2D1_HWND_RENDER_TARGET_PROPERTIES hwndProps = D2D1::HwndRenderTargetProperties(hwnd, size); //DrawBox


    if (FAILED(m_factory->CreateHwndRenderTarget(rtProps, hwndProps, &m_renderTarget))) {
        return false;
    }


    m_factory->CreateHwndRenderTarget(D2D1::RenderTargetProperties(),D2D1::HwndRenderTargetProperties(hwnd, size),&m_renderTarget);

    m_renderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::SkyBlue), &m_brush);
   
    // ”wŒi‰æ‘œ“Ç‚Ýž‚Ýi‘Š‘ÎƒpƒX OKj
    LoadBackgroundImage(L"C:/Users/deran/Desktop/koukaku.png");

	return true; // Return true if successful
}

/*DrawBox*/
void Renderer::Render() {
    m_renderTarget->BeginDraw();

    m_renderTarget->Clear(D2D1::ColorF(D2D1::ColorF::Black));

    m_renderTarget->FillRectangle(D2D1::RectF(100, 100, 300, 200), m_brush); //DrawBox

    if (m_backgroundBitmap) 
    {
        D2D1_SIZE_F size = m_backgroundBitmap->GetSize();
        m_renderTarget->DrawBitmap(m_backgroundBitmap, D2D1::RectF(0, 0, size.width-500, size.height - 300));

    }
    m_renderTarget->EndDraw();
}

/*DrawBackGroundImage*/
bool Renderer::LoadBackgroundImage(const wchar_t* filename)
{
	IWICBitmapDecoder* decoder = nullptr;
    IWICBitmapFrameDecode* frame = nullptr;
    IWICFormatConverter* converter = nullptr;

    HRESULT hr = CoCreateInstance(CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER,IID_PPV_ARGS(&m_wicFactory));
    if (FAILED(hr))
    {
        return false;
    }

    hr = m_wicFactory->CreateDecoderFromFilename(filename,nullptr,GENERIC_READ,WICDecodeMetadataCacheOnLoad, &decoder);
    if (FAILED(hr)) 
    {
        return false;
    }
	decoder->GetFrame(0, &frame);

    hr = m_wicFactory->CreateFormatConverter(&converter);
    converter->Initialize(frame, GUID_WICPixelFormat32bppPBGRA, WICBitmapDitherTypeNone, nullptr, 0.0f, WICBitmapPaletteTypeCustom);

    m_renderTarget->CreateBitmapFromWicBitmap(converter, nullptr, &m_backgroundBitmap);
    
    // Use the converted image in your rendering logic
    // ...
    if (converter)
    {
        converter->Release();
    }
    if (frame)
    {
        frame->Release();
    }
    if (decoder)
    {
        decoder->Release();
    }
    
	return true;
}



void Renderer::Cleanup() {
    if (m_backgroundBitmap)
    {
        m_backgroundBitmap->Release();
        m_backgroundBitmap = nullptr;
    }
    if (m_brush)
    {
        m_brush->Release();
        m_brush = nullptr;
    }
    if (m_renderTarget)
    {
        m_renderTarget->Release();
        m_renderTarget = nullptr;
    }
    if (m_factory)
    {
        m_factory->Release();
        m_factory = nullptr;
    }
    if (m_wicFactory)
    {
        m_wicFactory->Release();
        m_wicFactory = nullptr;
    }
    CoUninitialize();
}