#pragma once
#include <windows.h>
#include <d2d1.h>
#include <wincodec.h>
#include <dwrite.h>
#include <string>
#include <vector>
#include <chrono>

class Renderer
{
	public:
		Renderer();
		~Renderer();

		bool Initialize(HWND hwnd);
		void Render();
		bool LoadBackgroundImage(const wchar_t* imagePath);
		void NextText();
		bool LoadScriptFromFile(const wchar_t* filename);
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
		// メンバ変数追加
		IDWriteFactory* m_dwriteFactory = nullptr;
		IDWriteTextFormat* m_textFormat = nullptr;
		//std::wstring m_dialogText = L"ベルリン入りする際のルートは?";
		std::wstring m_dialogText;


		struct ScriptLine {
			std::wstring name;
			std::wstring message;
		};

		std::vector<ScriptLine> m_script; // スクリプトの行を格納するベクター

		std::vector<std::wstring> m_scriptLines;
		int m_textIndex = 0;


		std::wstring m_dialogTextFull; //表示する全文
		std::wstring m_dialogTextPart; //現在表示中の部分
		int m_charIndex = 0; //現在表示中の文字数
		bool m_isTextFullyShown = false; //テキストが完全に表示されたかどうか
		std::chrono::steady_clock::time_point m_lastCharTime; //最後の更新時間
		int m_charIntervalMs = 50; /* ← 表示間隔（ms）。大きくすると遅くなる（例 : 50 = 0.05秒）*/

		std::wstring m_nameText = L"リカ"; // キャラクターの名前




};