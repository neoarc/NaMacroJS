#include "stdafx.h"
#include "NaKeyboard.h"

#include <Windows.h>

int NaKeyboard::GetKeycodeFromChar(wchar_t ch)
{
	int ret = VkKeyScan(ch);

	return ret;
}

void NaKeyboard::KeyDown(int code)
{
	INPUT input;
	ZeroMemory(&input, sizeof(INPUT));
	input.type = INPUT_KEYBOARD;
	input.ki.wVk = static_cast<WORD>(code);
	input.ki.dwFlags = KEYEVENTF_EXTENDEDKEY;
	::SendInput(1, &input, sizeof(INPUT));
}

void NaKeyboard::KeyUp(int code)
{
	INPUT input;
	ZeroMemory(&input, sizeof(INPUT));
	input.type = INPUT_KEYBOARD;
	input.ki.wVk = static_cast<WORD>(code);
	input.ki.dwFlags = KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP;
	::SendInput(1, &input, sizeof(INPUT));
}

void NaKeyboard::TypeString(const wchar_t* str, int delay)
{
	wchar_t* str2 = (wchar_t*)str;
	int len = wcslen(str2);
	for (int i = 0; i < len; i++)
	{
		int keyCode = NaKeyboard::GetKeycodeFromChar(*str2);
		if (keyCode == -1)
			continue;
		*str2++;

		int modifier = keyCode >> 8;
		keyCode = keyCode & 0xff;

		if (modifier & 1) NaKeyboard::KeyDown(VK_LSHIFT);
		if (modifier & 2) NaKeyboard::KeyDown(VK_LCONTROL);
		if (modifier & 4) NaKeyboard::KeyDown(VK_LMENU);

		NaKeyboard::KeyDown(keyCode);

		if (delay)
			::Sleep(delay);

		NaKeyboard::KeyUp(keyCode);

		if (modifier & 1) NaKeyboard::KeyUp(VK_LSHIFT);
		if (modifier & 2) NaKeyboard::KeyUp(VK_LCONTROL);
		if (modifier & 4) NaKeyboard::KeyUp(VK_LMENU);
	}
}
