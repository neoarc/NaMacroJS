#pragma once

#include <Windows.h>

#include "NaString.h"

#define NA_MESSAGEBOX_CLASS		L"NaMessageBoxClass"
#define NA_MESSAGEBOX_MARK		0x20160617

class NaMessageBox
{
public:
	NaMessageBox();
	~NaMessageBox();

	NaString DoModal(HWND hParent, wchar_t* message, wchar_t* title, wchar_t* defaultStr);
	NaString m_strMessage;
	NaString m_strRet;
	HWND m_hEdit;
	WNDPROC m_OldEditProc;

	// static
	static bool s_bRegisterClass;
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK EditProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};

