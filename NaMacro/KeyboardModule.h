#pragma once

#include <v8.h>
#include "V8Wrap.h"
#include "ModuleBase.h"

#include <Windows.h>
#include <map>

class HotkeyKey {
public:
	int keycode;
	int modifier;
	int index;

	bool operator <(const HotkeyKey &other) const
	{
		return index < other.index;
	}
};

class NaKeyboardModule : public ModuleBase
{
public:
	virtual void Init(Isolate *isolate, Local<ObjectTemplate> &global_template);
	virtual void Release();

	Local<Object> GetKeyboardObject(Isolate *isolate);
	
	// static
	static int GetKeycodeFromChar(wchar_t ch);
	static void KeyDown(int code);
	static void KeyUp(int code);

	// for global hotkey event
	static std::map <HotkeyKey, Persistent<Function, CopyablePersistentTraits<Function>>> s_mapKeyEventCallback;
	static void OnHotkey(Isolate *isolate, WPARAM wParam, LPARAM lParam);

	// accessors

	// methods
	DEFINE_CLASS_METHOD(down);
	DEFINE_CLASS_METHOD(up);
	DEFINE_CLASS_METHOD(typeString);
	DEFINE_CLASS_METHOD(on);
};

