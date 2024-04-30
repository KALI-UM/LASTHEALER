// D2DWork.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"

#include "D2DWork.h"
#include "Input.h"
#include "MPen.h"
#include "MFigure.h"

#include <commdlg.h>

#define MAX_LOADSTRING 100
#define IMGUI_ENABLE_FREETYPE

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
HWND hWnd;
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

D2DEngine* pD2DEngine;
Input* pInput;
Mode* pCurrMode;
MPen* pPen;
MFigure* pFigure;

CHOOSECOLORW cc;
D2D1::ColorF Colors[3] = { NULL,NULL,NULL };
Obj::Rectangle ColorRect[3] = { Obj::Rectangle(D2D1::ColorF::Black, D2D1::ColorF::Black, 1.0f, {10, 10}, {40, 40}),Obj::Rectangle(D2D1::ColorF::Black, D2D1::ColorF::Black, 1.0f, {50, 10}, {80, 40}),Obj::Rectangle(D2D1::ColorF::Black, D2D1::ColorF::Black, 1.0f, {90, 10}, {120, 40}) };

POINT tempp;
float temps;

OPENFILENAME ofn;


// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    ImagePos(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    Sprite(HWND, UINT, WPARAM, LPARAM);

void DrawHelper(RECT rc)
{
	pD2DEngine->DrawRect(rc.right - 255, rc.right - 10, rc.top + 5, rc.top + 135, D2D1::ColorF::Black, D2D1::ColorF::White, 1.0f);

	ColorRect[COLOR_BACKGROUND].rect = D2D1::Rect(rc.right - 250, rc.top + 10, rc.right - 220, rc.top + 40);
	ColorRect[COLOR_PEN].rect = D2D1::Rect(rc.right - 210, rc.top + 10, rc.right - 180, rc.top + 40);
	ColorRect[COLOR_FILL].rect = D2D1::Rect(rc.right - 170, rc.top + 10, rc.right - 140, rc.top + 40);
	pD2DEngine->Draw(&ColorRect[COLOR_BACKGROUND]);
	pD2DEngine->Draw(&ColorRect[COLOR_PEN]);
	pD2DEngine->Draw(&ColorRect[COLOR_FILL]);

	pD2DEngine->WriteText({ rc.right - 250, rc.top + 10 }, { rc.right - 220, rc.top + 40 }, L"B", D2D1::ColorF::Black, 1.f);
	pD2DEngine->WriteText({ rc.right - 210, rc.top + 10 }, { rc.right - 180, rc.top + 40 }, L"P", D2D1::ColorF::Black, 1.f);
	pD2DEngine->WriteText({ rc.right - 170, rc.top + 10 }, { rc.right - 140, rc.top + 40 }, L"F", D2D1::ColorF::Black, 1.f);
	pD2DEngine->WriteText({ rc.right - 250, rc.top + 50 }, { rc.right, rc.top + 70 }, L"파일 > 이미지 파일 열기", L"Verdana", 15, D2D1::ColorF::Black, 1.f);
	pD2DEngine->WriteText({ rc.right - 250, rc.top + 70 }, { rc.right, rc.top + 90 }, L"파일 > 스프라이트 파일 열기", L"Verdana", 15, D2D1::ColorF::Black, 1.f);
	pD2DEngine->WriteText({ rc.right - 250, rc.top + 90 }, { rc.right, rc.top + 110 }, L"모드 > 그림판", L"Verdana", 15, D2D1::ColorF::Black, 1.f);
	pD2DEngine->WriteText({ rc.right - 250, rc.top + 110 }, { rc.right, rc.top + 130 }, L"모드 > 도형", L"Verdana", 15, D2D1::ColorF::Black, 1.f);
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: 여기에 코드를 입력합니다.

	// 전역 문자열을 초기화합니다.
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_D2DWORK, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);


	// 애플리케이션 초기화를 수행합니다:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_D2DWORK));
	MSG msg;

	pD2DEngine = new D2DEngine();
	pD2DEngine->Initialize(hWnd);

	pInput = new Input();

	pPen = new MPen();
	pFigure = new MFigure();
	pCurrMode = pPen;
	pCurrMode->Initialize();

	// Initialize CHOOSECOLOR 
	static DWORD rgbCurrent;
	static COLORREF acrCustClr[16];
	ZeroMemory(&cc, sizeof(cc));
	cc.lStructSize = sizeof(cc);
	cc.hwndOwner = hWnd;
	cc.rgbResult = rgbCurrent;
	cc.lpCustColors = (LPDWORD)acrCustClr;
	cc.Flags = CC_FULLOPEN | CC_RGBINIT;

	Colors[COLOR_BACKGROUND] = D2D1::ColorF::WhiteSmoke;
	Colors[COLOR_PEN] = D2D1::ColorF::DarkSlateBlue;
	Colors[COLOR_FILL] = D2D1::ColorF::DarkSeaGreen;
	ColorRect[COLOR_BACKGROUND].fillColor = Colors[COLOR_BACKGROUND];
	ColorRect[COLOR_PEN].fillColor = Colors[COLOR_PEN];
	ColorRect[COLOR_FILL].fillColor = Colors[COLOR_FILL];
	pD2DEngine->SetBackgroundColor(Colors[COLOR_BACKGROUND]);




	RECT rc;

	LARGE_INTEGER periodFrequency;
	LARGE_INTEGER last;
	LARGE_INTEGER tick;
	LARGE_INTEGER elapsed;
	QueryPerformanceCounter(&last);
	QueryPerformanceFrequency(&periodFrequency);
	float timeScale = 1.0f / periodFrequency.QuadPart;

	// 기본 메시지 루프입니다:
	while (true)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT) break;

			DispatchMessage(&msg);

		}
		else
		{
			QueryPerformanceCounter(&tick);
			elapsed.QuadPart = (tick.QuadPart - last.QuadPart) * 1000000;
			elapsed.QuadPart /= periodFrequency.QuadPart;
			if (elapsed.QuadPart > (1000000 / 60))
			{
				last.QuadPart = tick.QuadPart;

				//메인
				GetClientRect(hWnd, &rc);
				pD2DEngine->BeginRender();
				pCurrMode->Draw();
				DrawHelper(rc);
				pD2DEngine->EndRender();
				pInput->Update();
			}
		}
	}

	return (int)msg.wParam;
}



//
//  함수: MyRegisterClass()
//
//  용도: 창 클래스를 등록합니다.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_D2DWORK));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_D2DWORK);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

//
//   함수: InitInstance(HINSTANCE, int)
//
//   용도: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   주석:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

	hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  용도: 주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 애플리케이션 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// 메뉴 선택을 구문 분석합니다:
		switch (wmId)
		{
		case ID_OPENFILE:
		{
			// Initialize OPENFILENAME
			wchar_t szFile[260];       // buffer for file name
			ZeroMemory(&ofn, sizeof(ofn));
			ofn.lStructSize = sizeof(ofn);
			ofn.hwndOwner = hWnd;
			ofn.lpstrFile = szFile;
			ofn.lpstrFile[0] = '\0';
			ofn.nMaxFile = sizeof(szFile);
			ofn.lpstrFilter = L"Image\0*.PNG;*.JPG;*.JPEG;*.BMP\0";
			ofn.nFilterIndex = 1;
			ofn.lpstrFileTitle = NULL;
			ofn.nMaxFileTitle = 0;
			ofn.lpstrInitialDir = NULL;
			ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
			GetOpenFileName(&ofn);
			std::wstring str(ofn.lpstrFile);

			if (str.length() != 0)
			{
				DialogBox(hInst, MAKEINTRESOURCE(IDD_SETIMAGEPOS), hWnd, ImagePos);

				Obj::Image* i = pD2DEngine->Filename2Image(ofn.lpstrFile);
				i->start = { tempp.x, tempp.y };
				temps == 0 ? i->scale = { 1,1 } : i->scale = { temps,temps };
				pD2DEngine->SaveDrawing(i);
			}
		}
		break;
		case ID_OPENSPRITEFILE:
		{
			// Initialize OPENFILENAME
			wchar_t szFile[260];       // buffer for file name
			ZeroMemory(&ofn, sizeof(ofn));
			ofn.lStructSize = sizeof(ofn);
			ofn.hwndOwner = hWnd;
			ofn.lpstrFile = szFile;
			ofn.lpstrFile[0] = '\0';
			ofn.nMaxFile = sizeof(szFile);
			ofn.lpstrFilter = L"Image\0*.PNG;*.JPG;*.JPEG;*.BMP\0";
			ofn.nFilterIndex = 1;
			ofn.lpstrFileTitle = NULL;
			ofn.nMaxFileTitle = 0;
			ofn.lpstrInitialDir = NULL;
			ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
			GetOpenFileName(&ofn);
			std::wstring str(ofn.lpstrFile);

			if (str.length() > ofn.nFileOffset)
			{
				DialogBox(hInst, MAKEINTRESOURCE(IDD_SPRITEFRAME), hWnd, Sprite);
			}
		}
		break;
		case ID_GRIMPAN:
		{
			pCurrMode = pPen;
			pCurrMode->Initialize();
		}
		break;
		case ID_FIGURE:
		{
			pCurrMode = pFigure;
			pCurrMode->Initialize();
		}
		break;
		case ID_FRECT:
		{
			pCurrMode = pFigure;
			pCurrMode->Initialize();
			pFigure->bRect = true;
		}
		break;
		case ID_FELLIPSE:
		{
			pCurrMode = pFigure;
			pCurrMode->Initialize();
			pFigure->bRect = false;
		}
		break;
		case ID_CBG:
		{
			ChooseColor(&cc);
			Colors[COLOR_BACKGROUND] = RGB(GetBValue(cc.rgbResult), GetGValue(cc.rgbResult), GetRValue(cc.rgbResult));
			pD2DEngine->SetBackgroundColor(Colors[COLOR_BACKGROUND]);
			ColorRect[COLOR_BACKGROUND].fillColor = Colors[COLOR_BACKGROUND];
		}
		break;
		case ID_CPEN:
		{
			ChooseColor(&cc);
			Colors[COLOR_PEN] = RGB(GetBValue(cc.rgbResult), GetGValue(cc.rgbResult), GetRValue(cc.rgbResult));
			ColorRect[COLOR_PEN].fillColor = Colors[COLOR_PEN];
		}
		break;
		case ID_CFILL:
		{
			ChooseColor(&cc);
			Colors[COLOR_FILL] = RGB(GetBValue(cc.rgbResult), GetGValue(cc.rgbResult), GetRValue(cc.rgbResult));
			ColorRect[COLOR_FILL].fillColor = Colors[COLOR_FILL];
		}
		break;

		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		// TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...
		EndPaint(hWnd, &ps);
	}
	break;
	case WM_MOUSEMOVE:
	{
		pInput->SetMousePoint(lParam);
	}
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_RBUTTONDOWN:
	case WM_RBUTTONUP:
	{
		pInput->SetMouseState(wParam, lParam);
	}
	break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// 정보 대화 상자의 메시지 처리기입니다.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

INT_PTR CALLBACK ImagePos(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		switch (wParam)
		{
		case IDOK:
		{
			tempp.x = GetDlgItemInt(hDlg, IDC_EDIT1, NULL, false);
			tempp.y = GetDlgItemInt(hDlg, IDC_EDIT2, NULL, false);
			temps = GetDlgItemInt(hDlg, IDC_EDIT3, NULL, false);
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
		case IDCANCEL:
		{
			tempp.x = 0;
			tempp.y = 0;
			temps = 1;
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		}
		break;
	}
	return (INT_PTR)FALSE;
}

INT_PTR CALLBACK Sprite(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		switch (wParam)
		{
		case IDOK:
		{
			DialogBox(hInst, MAKEINTRESOURCE(IDD_SETIMAGEPOS), hWnd, ImagePos);

			Obj::Sprite* s = pD2DEngine->Filename2Sprite(ofn.lpstrFile, GetDlgItemInt(hDlg, IDC_EDIT1, NULL, false), GetDlgItemInt(hDlg, IDC_EDIT2, NULL, false), 1);
			s->start = { tempp.x, tempp.y };

			float bunmo = GetDlgItemInt(hDlg, IDC_EDIT3, NULL, false);
			if (bunmo == 0)bunmo = 1;
			s->speed = 1 / bunmo;
			temps == 0 ? s->scale = { 1,1 } : s->scale = { temps,temps };
			pD2DEngine->SaveDrawing(s);
		}
		case IDCANCEL:
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
