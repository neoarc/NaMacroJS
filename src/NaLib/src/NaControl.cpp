#include "stdafx.h"
#include "NaControl.h"

#include "NaDebug.h"
#include "NaImage.h"

#include "NaWindow.h"

NaControl::NaControl()
	: NaWindow()
{
	m_enType = NA_CONTROL_UNKNOWN;
	m_hWnd = nullptr;
	m_hParentWnd = nullptr;
	m_x = 0;
	m_y = 0;
	m_width = 0;
	m_height = 0;

	NaDebugOut(L"NaControl(): 0x%08x\n", this);
}


NaControl::~NaControl()
{
	NaDebugOut(L"~NaControl(): 0x%08x\n", this);
}

HWND NaControl::Create()
{
	NaString strClass;
	switch (m_enType)
	{
	case NA_CONTROL_BUTTON:
		strClass = L"Button";
		break;
	case NA_CONTROL_EDIT:
		strClass = L"Edit";
		break;
	case NA_CONTROL_STATIC:
	default:
		strClass = L"Static";
		break;
	}

	HINSTANCE hInstance = GetModuleHandle(NULL);
	DWORD dwStyle = WS_CHILD;
	m_hWnd = ::CreateWindow(
		strClass.wstr(),
		L"", // _In_opt_ LPCTSTR   lpWindowName,
		dwStyle, // _In_     DWORD     dwStyle,
		m_x,
		m_y,
		m_width,
		m_height,
		m_hParentWnd, // _In_opt_ HWND      hWndParent,
		NULL, // _In_opt_ HMENU     hMenu,
		hInstance,
		NULL // _In_opt_ LPVOID    lpParam
	);
	::SetMenu(m_hWnd, (HMENU)m_hWnd);

	return m_hWnd;
}

void NaControl::Destroy()
{

}

void NaControl::SetFocus()
{
	::SetFocus(m_hWnd);
}

void NaControl::SetVisible(bool bVisible)
{
	if (m_hWnd)
		::ShowWindow(m_hWnd, bVisible ? SW_SHOW : SW_HIDE);
}