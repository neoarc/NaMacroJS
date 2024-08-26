#include "stdafx.h"
#include "NaScreen.h"

#include <NaLib/NaDesktop.h>
#include <NaLib/NaImage.h>

int NaScreen::GetWidth()
{
    int metrics = GetSystemMetrics(SM_CXSCREEN);
    return metrics;
}

int NaScreen::GetHeight()
{
	int metrics = GetSystemMetrics(SM_CYSCREEN);
	return metrics;
}

int NaScreen::GetPixel(int x, int y)
{
	HDC hDC = NaDesktop::GetDC();

	// get pixel from point
#define USE_FAST_GETPIXEL
#ifdef USE_FAST_GETPIXEL
	HDC hMemoryDC = ::CreateCompatibleDC(hDC);
	if (hMemoryDC == nullptr)
	{
// 		DWORD dwError = ::GetLastError();
		// TODO error handling
		return -1;
	}

	HBITMAP hBmp = ::CreateCompatibleBitmap(hDC, 1, 1);
	::SelectObject(hMemoryDC, hBmp);
	::BitBlt(hMemoryDC, 0, 0, 1, 1, hDC, x, y, SRCCOPY);

	COLORREF color = ::GetPixel(hMemoryDC, 0, 0);
	::DeleteDC(hMemoryDC);
#else
	COLORREF color = ::GetPixel(hDC, x, y);
#endif
	// TODO check GetPixel Failed when ReleaseDC
	//::ReleaseDC(JsScreen::GetDesktopHWND(), hDC);

	return color;
}

int NaScreen::SetAero(bool bEnable)
{
	WCHAR strFullPath[MAX_PATH * 2];
	int nLen = ::GetSystemDirectory(strFullPath, MAX_PATH * 2);
	strFullPath[nLen] = L'\\';
	wcscpy(strFullPath + nLen + 1, L"dwmapi.dll");

	HMODULE hModule = LoadLibrary(strFullPath);
	HRESULT(WINAPI * fnDwnEnableComposition)(int) = (HRESULT(__stdcall*)(int))::GetProcAddress(hModule, "DwmEnableComposition");

	HRESULT hr = S_OK;
	if (fnDwnEnableComposition != NULL)
	{
		// DWM_EC_DISABLECOMPOSITION 0
		// DWM_EC_ENABLECOMPOSITION 1
		if (bEnable == FALSE)
			hr = fnDwnEnableComposition(0);
		else
			hr = fnDwnEnableComposition(1);
	}

	/*
	if (bAeroOn == false)
		hr = DwmEnableComposition(DWM_EC_DISABLECOMPOSITION);
	else
		hr = DwmEnableComposition(DWM_EC_ENABLECOMPOSITION);
	*/

	return (int)hr;
}
