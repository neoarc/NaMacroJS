//
// Custom NotifyWindow
// 2017.01.21 neoarc
//

#include "NaNotifyWindow.h"

#include "../NaMacro/resource.h"
#include "../NaMacro/Common.h"

#include <thread>

bool NaNotifyWindow::s_bRegisterClass = false;
HWND NaNotifyWindow::s_hMasterWindow = nullptr;
POINT NaNotifyWindow::s_ptLastNotifyWindow = { -1, -1 };

NaNotifyWindow::NaNotifyWindow()
{
	m_strMessage = L"";
	m_strTitle = L"";
}

NaNotifyWindow::~NaNotifyWindow()
{
}

void NaNotifyWindow::Create(NaString strMessage, NaString strTitle)
{
	HINSTANCE hInstance = GetModuleHandle(NULL);

	m_strMessage = strMessage;
	m_strTitle = strTitle;

	// Move to right-bottom
	RECT rcWork;
	SystemParametersInfo(SPI_GETWORKAREA, 0, &rcWork, 0);
	int nScreenW = rcWork.right - rcWork.left;	 //GetSystemMetrics(SM_CXSCREEN);
	int nScreenH = rcWork.bottom - rcWork.top; //GetSystemMetrics(SM_CYSCREEN);
	int nW = 300;
	int nH = 150;

	// Calc next position
	POINT pt;
	if (s_ptLastNotifyWindow.x == -1 && s_ptLastNotifyWindow.y == -1)
	{
		pt = {
			nScreenW - nW,
			nScreenH - nH
		};
	}
	else
	{
		pt = {
			s_ptLastNotifyWindow.x,
			s_ptLastNotifyWindow.y - nH - 2
		};

		if (pt.y < 0)
		{
			pt = {
				s_ptLastNotifyWindow.x - nW - 10,
				nScreenH - nH
			};
		}
	}

	// I'm the last
	s_ptLastNotifyWindow = pt;

	DWORD dwStyle = WS_POPUP;
	HWND hWnd = ::CreateWindow(
		NA_NOTIFYWINDOW_CLASS,
		strTitle.wstr(), // _In_opt_ LPCTSTR   lpWindowName,
		dwStyle, // _In_     DWORD     dwStyle,
		pt.x, pt.y, nW, nH,
		s_hMasterWindow, // _In_opt_ HWND      hWndParent,
		NULL, // _In_opt_ HMENU     hMenu,
		hInstance,
		NULL // _In_opt_ LPVOID    lpParam
	);

	NaDebugOut(L"NaNotifyWindow::Create, 0x%08x (parent: 0x%08x)\n", hWnd, s_hMasterWindow);
	SetWindowLong(hWnd, GWL_USERDATA, (LONG)this);
	ShowWindow(hWnd, SW_SHOW);

	::SetWindowPos(
		hWnd,
		HWND_TOPMOST,
		0, 0, 0, 0,
		SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOREPOSITION | SWP_NOSIZE
	);

	SetTimer(hWnd, 0, 3000, nullptr);
	UpdateWindow(hWnd);

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

LRESULT NaNotifyWindow::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	NaNotifyWindow *pThis = (NaNotifyWindow*)GetWindowLong(hWnd, GWL_USERDATA);

	switch (message)
	{
	case WM_SHOWWINDOW:
		{
			RECT rcClient;
			GetClientRect(hWnd, &rcClient);
			int nWidth = rcClient.right - rcClient.left;
			int nHeight = rcClient.bottom - rcClient.top;

			HWND hCaption = CreateWindow(L"Static", pThis->m_strTitle.wstr(),
				WS_VISIBLE | WS_CHILD | SS_CENTER,
				0, 0, nWidth, 26,
				hWnd, 0, GetModuleHandle(NULL), NULL);

			HWND hMessage = CreateWindow(L"Static", pThis->m_strMessage.wstr(),
				WS_VISIBLE | WS_CHILD | SS_CENTER,
				0, 30, nWidth, nHeight - 30,
				hWnd, 0, GetModuleHandle(NULL), NULL);

			LONG lStyle = GetWindowLong(hMessage, GWL_EXSTYLE);
			lStyle = lStyle | WS_EX_TRANSPARENT;
			SetWindowLong(hMessage, GWL_EXSTYLE, lStyle);
		}
		break;
	case WM_TIMER:
		{
			RECT rc;
			GetWindowRect(hWnd, &rc);
			if (s_ptLastNotifyWindow.x == rc.left &&
				s_ptLastNotifyWindow.y == rc.top)
				s_ptLastNotifyWindow = { -1, -1 };
		}
		DestroyWindow(hWnd);
		break;
	case WM_COMMAND:
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

void NaNotifyWindow::AddNotifyWindow(NaString strMessage, NaString strTitle)
{
	HINSTANCE hInstance = GetModuleHandle(NULL);

	if (!s_bRegisterClass)
	{
		WNDCLASS WndClass;
		WndClass.cbClsExtra = 0;
		WndClass.cbWndExtra = 0;
		WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
		WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
		WndClass.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MAIN_ICON));
		WndClass.hInstance = hInstance;
		WndClass.lpfnWndProc = NaNotifyWindow::WndProc;
		WndClass.lpszClassName = NA_NOTIFYWINDOW_CLASS;
		WndClass.lpszMenuName = NULL;
		WndClass.style = CS_HREDRAW | CS_VREDRAW;
		RegisterClass(&WndClass);

		s_bRegisterClass = true;

		// Create master window
		if (s_hMasterWindow == nullptr)
		{
			// Hidden window for hide from taskbar
			s_hMasterWindow = ::CreateWindow(
				NA_NOTIFYWINDOW_CLASS,
				L"", 0,	0, 0, 0, 0, nullptr, NULL, hInstance, NULL
			);
		}
	}

	std::thread([strMessage, strTitle]// () -> void
	{
		NaNotifyWindow *pWindow = new NaNotifyWindow;
		pWindow->Create(strMessage, strTitle);

		::Sleep(1);
	}
	);
}
