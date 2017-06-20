#include "stdafx.h"
#include "NaMacroCommon.h"

#include <Windows.h>
#include <lmcons.h>

#include "BasicModule.h"
#include "KeyboardModule.h"

namespace NaMacroCommon 
{
	bool g_bExit = false;

	void MessageLoopTillExit(Isolate* isolate)
	{
		// MessageLoop after main()
		MSG msg = { 0 };
		while (!g_bExit)
		{
			bool bRet = (GetMessage(&msg, NULL, 0, 0) == TRUE);
			if (msg.message == 0)
				continue;
			if (bRet == 0)
				break;
			switch (msg.message)
			{
			case WM_HOTKEY:
				NaKeyboardModule::OnHotkey(isolate, msg.wParam, msg.lParam);
				break;
			case WM_TIMER:
				NaBasicModule::OnTimer(isolate, (int)msg.wParam);
				break;
			}

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	std::wstring GetSystemInfoStringByAPI(std::function<int(wchar_t*, unsigned long*)> f)
	{
		const DWORD buffLen = 512;
		wchar_t str[buffLen];
		DWORD size = buffLen;

		f(str, &size);

		return str;
	}
}
