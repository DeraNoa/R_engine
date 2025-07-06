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
		// �����o�ϐ��ǉ�
		IDWriteFactory* m_dwriteFactory = nullptr;
		IDWriteTextFormat* m_textFormat = nullptr;
		//std::wstring m_dialogText = L"�x���������肷��ۂ̃��[�g��?";
		std::wstring m_dialogText;


		struct ScriptLine {
			std::wstring name;
			std::wstring message;
		};

		std::vector<ScriptLine> m_script; // �X�N���v�g�̍s���i�[����x�N�^�[

		std::vector<std::wstring> m_scriptLines;
		int m_textIndex = 0;


		std::wstring m_dialogTextFull; //�\������S��
		std::wstring m_dialogTextPart; //���ݕ\�����̕���
		int m_charIndex = 0; //���ݕ\�����̕�����
		bool m_isTextFullyShown = false; //�e�L�X�g�����S�ɕ\�����ꂽ���ǂ���
		std::chrono::steady_clock::time_point m_lastCharTime; //�Ō�̍X�V����
		int m_charIntervalMs = 50; /* �� �\���Ԋu�ims�j�B�傫������ƒx���Ȃ�i�� : 50 = 0.05�b�j*/

		std::wstring m_nameText = L"���J"; // �L�����N�^�[�̖��O




};