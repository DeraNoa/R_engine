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

	DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), reinterpret_cast<IUnknown**>(&m_dwriteFactory));
	
    RECT rc;
    GetClientRect(hwnd, &rc);
    D2D1_SIZE_U size = D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top);

    D2D1_RENDER_TARGET_PROPERTIES rtProps = D2D1::RenderTargetProperties(); //DrawBox
    D2D1_HWND_RENDER_TARGET_PROPERTIES hwndProps = D2D1::HwndRenderTargetProperties(hwnd, size); //DrawBox


    if (FAILED(m_factory->CreateHwndRenderTarget(rtProps, hwndProps, &m_renderTarget))) {
        return false;
    }

    // セリフリスト（仮：あとでスクリプトから読み込む）
    m_scriptLines = { L"攻殻機動隊セリフ",L"ベルリン入りする際のルートは?",L"草薙素子" };

	m_textIndex = 0; // 初期化
	m_dialogText = m_scriptLines[m_textIndex]; // 初期のテキストを設定

    m_factory->CreateHwndRenderTarget(D2D1::RenderTargetProperties(),D2D1::HwndRenderTargetProperties(hwnd, size),&m_renderTarget);
    m_dwriteFactory->CreateTextFormat(L"Meiryo",nullptr,DWRITE_FONT_WEIGHT_NORMAL,DWRITE_FONT_STYLE_NORMAL,DWRITE_FONT_STRETCH_NORMAL,24.0f,L"ja-jp",&m_textFormat);

    m_renderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::WhiteSmoke), &m_brush);
   
    // 背景画像読み込み（相対パス OK）
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
        m_renderTarget->DrawBitmap(m_backgroundBitmap, D2D1::RectF(0, 0, size.width-500, size.height - 250));

    }

    if (!m_dialogText.empty()) {
        D2D1_RECT_F layoutRect = D2D1::RectF(200, 500, 750, 580);
        m_renderTarget->DrawTextW(m_dialogText.c_str(),(UINT32)m_dialogText.length(),m_textFormat,&layoutRect,m_brush);
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

void Renderer::NextText() {
    if (m_textIndex + 1 < (int)m_scriptLines.size()) {
        m_textIndex++;
        m_dialogText = m_scriptLines[m_textIndex];
    }
    else {
        // 最後まで表示したら止める or ループ
        m_dialogText = L"終わりです。";
    }
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
    if (m_textFormat) {
        m_textFormat->Release();
        m_textFormat = nullptr;
    }
    if (m_dwriteFactory) {
        m_dwriteFactory->Release();
        m_dwriteFactory = nullptr;
    }
    CoUninitialize();
}