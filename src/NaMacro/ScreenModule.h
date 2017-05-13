#pragma once

#include "V8Wrap.h"
#include "ModuleBase.h"

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
	DEFINE_CLASS_ACCESSOR_RO(width);
	DEFINE_CLASS_ACCESSOR_RO(height);

	// methods
	DEFINE_CLASS_METHOD(capture);
	DEFINE_CLASS_METHOD(findColor);
	DEFINE_CLASS_METHOD(getPixel);
	DEFINE_CLASS_METHOD(setAero);
};

