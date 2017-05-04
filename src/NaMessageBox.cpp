#include "stdafx.h"
#include "NaMessageBox.h"
#include "NaCommon.h"

bool NaMessageBox::s_bRegisterClass = false;

NaMessageBox::NaMessageBox()
{
	m_hEdit = nullptr;
	m_OldEditProc = nullptr;
}

NaMessageBox::~NaMessageBox()
{
}

NaString NaMessageBox::DoModal(HWND hParent, wchar_t* message, wchar_t* title, wchar_t* defaultStr)
{
	HINSTANCE hInstance = GetModuleHandle(NULL);

	if (!s_bRegisterClass)
	{
		WNDCLASS WndClass;
		WndClass.cbClsExtra = 0;
		WndClass.cbWndExtra = 0;
		WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
		WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
		WndClass.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(102)); // #FIXME: hard coded res no.
// 		WndClass.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MAIN_ICON));
		WndClass.hInstance = hInstance;
		WndClass.lpfnWndProc = NaMessageBox::WndProc;
		WndClass.lpszClassName = NA_MESSAGEBOX_CLASS;
		WndClass.lpszMenuName = NULL;
		WndClass.style = CS_HREDRAW | CS_VREDRAW;
		RegisterClass(&WndClass);

		s_bRegisterClass = true;
	}

	m_strMessage = message;
	m_strRet = defaultStr;

	// Move to Center
	int nScreenW = GetSystemMetrics(SM_CXSCREEN);
	int nScreenH = GetSystemMetrics(SM_CYSCREEN);
	int nW = 251;
	int nH = 154;

	DWORD dwStyle = WS_CAPTION | WS_SYSMENU;
	HWND hWnd = ::CreateWindow(
		NA_MESSAGEBOX_CLASS,
		title, // _In_opt_ LPCTSTR   lpWindowName,
		dwStyle, // _In_     DWORD     dwStyle,
		nScreenW/2 - nW/2,
		nScreenH/2 - nH/2,
		nW,
		nH,
		hParent, // _In_opt_ HWND      hWndParent,
		NULL, // _In_opt_ HMENU     hMenu,
		hInstance,
		NULL // _In_opt_ LPVOID    lpParam
	);

	SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)this);
	ShowWindow(hWnd, SW_SHOW);
	UpdateWindow(hWnd);

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return m_strRet;
}

LRESULT NaMessageBox::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	NaMessageBox *pThis = (NaMessageBox*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
	switch (message)
	{
	case WM_SHOWWINDOW:
		{
			RECT rcClient;
			GetClientRect(hWnd, &rcClient);
			int nWidth = rcClient.right - rcClient.left;
			int nHeight = rcClient.bottom - rcClient.top;

			HWND hStatic = CreateWindow(L"Static", pThis->m_strMessage.wstr(),
				WS_VISIBLE | WS_CHILD | SS_CENTER,
				10, 10, nWidth - 20, 26,
				hWnd, 0, GetModuleHandle(NULL), NULL);
			UNUSED_VAR(hStatic);

			pThis->m_hEdit = CreateWindow(L"Edit", pThis->m_strRet.wstr(),
				WS_VISIBLE | WS_CHILD | WS_BORDER | WS_TABSTOP | ES_LEFT | ES_AUTOHSCROLL,
				10, 40, nWidth - 20, 26,
				hWnd, 0, GetModuleHandle(NULL), NULL);
			pThis->m_OldEditProc = (WNDPROC)SetWindowLongPtr(pThis->m_hEdit, GWLP_WNDPROC, (LONG_PTR)&NaMessageBox::EditProc);

			HWND hBtnOK = CreateWindow(L"Button", L"OK",
				WS_VISIBLE | WS_CHILD | WS_TABSTOP | BS_PUSHBUTTON | BS_DEFPUSHBUTTON,
				42, nHeight - 40, 88, 26,
				hWnd, (HMENU)IDOK, GetModuleHandle(NULL), NULL);
			UNUSED_VAR(hBtnOK);

			HWND hBtnCancel = CreateWindow(L"Button", L"Cancel",
				WS_VISIBLE | WS_CHILD | WS_TABSTOP,
				138, nHeight - 40, 88, 26,
				hWnd, (HMENU)IDCANCEL, GetModuleHandle(NULL), NULL);
			UNUSED_VAR(hBtnCancel);

			::SetFocus(pThis->m_hEdit);
		}
		break;
	
	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK) 
		{
			wchar_t str[65535];
			GetWindowText(pThis->m_hEdit, str, 65535);
			pThis->m_strRet = NaString(str);

			DestroyWindow(hWnd);
			return 0;
		}
		if (LOWORD(wParam) == IDCANCEL)
		{
			DestroyWindow(hWnd);
			return 0;
		}
		break;
	case WM_CLOSE:
		DestroyWindow(hWnd);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}

LRESULT NaMessageBox::EditProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HWND hParent = GetParent(hWnd);
	NaMessageBox *pThis = (NaMessageBox*)GetWindowLongPtr(hParent, GWLP_USERDATA);
	switch (message)
	{
	case WM_CHAR:
		switch (wParam ) 
		{
		case 0x0d: // vk_return
			SendMessage(hParent, WM_COMMAND, (WPARAM)IDOK, 0);
			return 0;
		case 0x1b: // vk_escape
			SendMessage(hParent, WM_COMMAND, (WPARAM)IDCANCEL, 0);
			return 0;
		}
		break;
	}

	return CallWindowProc((WNDPROC)pThis->m_OldEditProc, hWnd, message, wParam, lParam);
}
