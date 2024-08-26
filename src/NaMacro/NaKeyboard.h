#pragma once

class NaKeyboard
{
public:
	static int GetKeycodeFromChar(wchar_t ch);
	static void KeyDown(int code);
	static void KeyUp(int code);

	static void TypeString(const wchar_t* str, int delay = 0);
};

