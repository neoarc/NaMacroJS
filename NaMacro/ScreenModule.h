#pragma once

#include <v8.h>
#include "Common.h"
#include "ModuleBase.h"

#include "Windows.h"

class NaScreenModule : public ModuleBase
{
public:
	virtual void Init(Isolate *isolate, Local<ObjectTemplate> &global_template);
	virtual void Release();

	Local<Object> GetScreenObject(Isolate *isolate);

	static HWND GetDesktopHWND()
	{
		if (m_hDesktopWnd)
			return m_hDesktopWnd;
		m_hDesktopWnd = ::GetDesktopWindow();
		return m_hDesktopWnd;
	}
	static HDC GetDesktopDC()
	{
		if (m_hDesktopDC)
			return m_hDesktopDC;
		m_hDesktopDC = ::GetWindowDC(m_hDesktopWnd);
		return m_hDesktopDC;
	}
	static HWND m_hDesktopWnd;
	static HDC m_hDesktopDC;
	static bool m_bAeroStatus;

	// accessors

	// methods
	DEFINE_CLASS_METHOD(CaptureScreen);
	DEFINE_CLASS_METHOD(GetPixel);
	DEFINE_CLASS_METHOD(SetAero);
};

