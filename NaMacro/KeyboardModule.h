#pragma once

#include <v8.h>
#include "Common.h"
#include "ModuleBase.h"

#include "Windows.h"
#include <vector>

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
	static std::vector <Persistent<Function, CopyablePersistentTraits<Function>>> s_vecKeyEventCallback;
	static void ProcessHotkey(Isolate *isolate, WPARAM wParam, LPARAM lParam);

	// accessors

	// methods
	DEFINE_CLASS_METHOD(Down);
	DEFINE_CLASS_METHOD(Up);
	DEFINE_CLASS_METHOD(TypeString);
	DEFINE_CLASS_METHOD(BindEvent);
};

