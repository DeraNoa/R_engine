#include "Renderer.h"
#include <fstream>
#include <sstream>
#include <locale>
#include <codecvt>

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

    LoadScriptFromFile(L"C:\script.txt");

    m_textIndex = 0;
    if (!m_script.empty()) {
        m_nameText = m_script[0].name;
        m_dialogTextFull = m_script[0].message;
        m_dialogText.clear();
        m_charIndex = 0;
        m_isTextFullyShown = false;
        m_lastCharTime = std::chrono::steady_clock::now();
    }

#if 0
    // セリフリスト（仮：あとでスクリプトから読み込む）
    m_scriptLines = { L"攻殻機動隊セリフ",L"Rage your dream 時を駆け抜けてゆく風も　光満ちてゆく",L"Rage your dream 待っていることだけをFeel the wind 忘れ ないでいて" };

	m_textIndex = 0; // 初期化
	m_dialogText = m_scriptLines[m_textIndex]; // 初期のテキストを設定
    m_dialogText.clear();
    m_charIndex = 0;
    m_isTextFullyShown = false;
    m_lastCharTime = std::chrono::steady_clock::now();
#endif
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
    // 名前表示用の箱
    D2D1_RECT_F nameBox = D2D1::RectF(50, 350, 300, 385);
    m_renderTarget->FillRectangle(nameBox, m_brush); // ← 白箱（後で色変更も可）
    //m_renderTarget->FillRectangle(D2D1::RectF(100, 100, 300, 200), m_brush); //DrawBox

    if (m_backgroundBitmap) 
    {
        D2D1_SIZE_F size = m_backgroundBitmap->GetSize();
        m_renderTarget->DrawBitmap(m_backgroundBitmap, D2D1::RectF(0, 0, size.width-500, size.height - 250));

    }

    // 名前文字列（黒文字にするため別ブラシ）
    ID2D1SolidColorBrush* nameBrush = nullptr;
    m_renderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black), &nameBrush);
    m_renderTarget->DrawTextW(
        m_nameText.c_str(),
        (UINT32)m_nameText.length(),
        m_textFormat,
        &nameBox,
        nameBrush
    );
    nameBrush->Release();

    // ★ 文字送り制御（1文字ずつ増やす）
    if (!m_isTextFullyShown) 
    {
        auto now = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - m_lastCharTime);
        if (elapsed.count() >= m_charIntervalMs) {
            if (m_charIndex < (int)m_dialogTextFull.length()) {
                m_charIndex++;
                m_dialogText = m_dialogTextFull.substr(0, m_charIndex);
                m_lastCharTime = now; // ← ここで更新
            }
            else
            {
                m_isTextFullyShown = true;
            }
        }
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
    if (!m_isTextFullyShown) {
        // 文字送り途中なら即全文表示
        m_charIndex = (int)m_dialogTextFull.length();
        m_dialogText = m_dialogTextFull;
        m_isTextFullyShown = true;
        return;
    }

#if 0
    if (m_textIndex == 0) {
        m_nameText = L"リカ";
    }
    else if (m_textIndex == 1) {
        m_nameText = L"ユウキ";
    }
    else {
        m_nameText = L"？？？";
    }
#endif
    // 次の行へ進む
    //if (m_textIndex + 1 < (int)m_scriptLines.size()) {
    if (m_textIndex + 1 < (int)m_script.size()) {
        m_textIndex++;
        //m_dialogTextFull = m_scriptLines[m_textIndex];
        m_nameText = m_script[m_textIndex].name;
        m_dialogTextFull = m_script[m_textIndex].message;
        m_dialogText.clear();
        m_charIndex = 0;
        m_isTextFullyShown = false;
        m_lastCharTime = std::chrono::steady_clock::now();
    }
    else {
        m_dialogTextFull = L"終わりです。";
        m_dialogText = m_dialogTextFull;
        m_isTextFullyShown = true;
    }
}


bool Renderer::LoadScriptFromFile(const wchar_t* filename) {
    std::ifstream file(filename); // ← narrow文字で開く
    if (!file.is_open()) return false;

    std::string line;
    while (std::getline(file, line)) {
        // UTF-8 → wstring に変換
        int wlen = MultiByteToWideChar(CP_UTF8, 0, line.c_str(), -1, nullptr, 0);
        std::wstring wline(wlen, L'\0');
        MultiByteToWideChar(CP_UTF8, 0, line.c_str(), -1, &wline[0], wlen);
        wline.pop_back(); // \0を除去

        size_t sep = wline.find(L'|');
        if (sep != std::wstring::npos) {
            ScriptLine entry;
            entry.name = wline.substr(0, sep);
            entry.message = wline.substr(sep + 1);
            m_script.push_back(entry);
        }
    }

    file.close();
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