#include "stdafx.h"

#include "KeyboardModule.h"

std::vector <Persistent<Function, CopyablePersistentTraits<Function>>> NaKeyboardModule::s_vecKeyEventCallback;

void NaKeyboardModule::Init(Isolate *isolate, Local<ObjectTemplate>& global_template)
{
	HandleScope handle_scope(isolate);

	Local<Object> global = isolate->GetCurrentContext()->Global();
	Local<Object> keyboard_obj = GetKeyboardObject(isolate);

#define ADD_KEYBOARD_METHOD(_js_func, _c_func)			ADD_OBJ_METHOD(keyboard_obj, _js_func, _c_func);
#define ADD_KEYBOARD_ACCESSOR(_prop, _getter, _setter)	ADD_OBJ_ACCESSOR(keyboard_obj, _prop, _getter, _setter);
	
	// accessors

	// methods
	ADD_KEYBOARD_METHOD(down,		Down);
	ADD_KEYBOARD_METHOD(up,			Up);
	ADD_KEYBOARD_METHOD(typeString, TypeString);
	ADD_KEYBOARD_METHOD(on,			BindEvent);
}

void NaKeyboardModule::Release()
{
	int nSize = NaKeyboardModule::s_vecKeyEventCallback.size();
	std::vector <Persistent<Function, CopyablePersistentTraits<Function>>>::iterator it = s_vecKeyEventCallback.begin();
	for (int i=0; it != NaKeyboardModule::s_vecKeyEventCallback.end(); it++, i++)
	{
		::UnregisterHotKey(NULL, i);
	}

	// TODO check Clear Persistent?

	NaKeyboardModule::s_vecKeyEventCallback.clear();
}

// description: return 'system.keyboard'
Local<Object> NaKeyboardModule::GetKeyboardObject(Isolate *isolate)
{
	// HandleScope 안에서 호출

	Local<Object> system_obj = GetSystemObject(isolate);
	Local<String> keyboard_name = String::NewFromUtf8(isolate, "keyboard", NewStringType::kNormal).ToLocalChecked();
	Local<Value> keyboard_value = system_obj->Get(keyboard_name);
	if (!keyboard_value.IsEmpty() && keyboard_value->IsUndefined())
	{
		// Init keyboard
		keyboard_value = Object::New(isolate);
		system_obj->Set(keyboard_name, keyboard_value);
	}

	Local<Object> keyboard_obj = keyboard_value->ToObject();
	return keyboard_obj;
}

int NaKeyboardModule::GetKeycodeFromChar(wchar_t ch)
{
	int ret = VkKeyScan(ch);

	return ret;
}

void NaKeyboardModule::KeyDown(int code)
{
	INPUT input;
	ZeroMemory(&input, sizeof(INPUT));
	input.type = INPUT_KEYBOARD;
	input.ki.wVk = code;
	input.ki.dwFlags = KEYEVENTF_EXTENDEDKEY;
	::SendInput(1, &input, sizeof(INPUT));
}

void NaKeyboardModule::KeyUp(int code)
{
	INPUT input;
	ZeroMemory(&input, sizeof(INPUT));
	input.type = INPUT_KEYBOARD;
	input.ki.wVk = code;
	input.ki.dwFlags = KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP;
	::SendInput(1, &input, sizeof(INPUT));
}

// description: key event handler
void NaKeyboardModule::ProcessHotkey(Isolate *isolate, WPARAM wParam, LPARAM lParam)
{
	int nIdx = (int)wParam;
	int nMod = (int)lParam; // MOD_ALT=1 MOD_CONTROL=2 MOD_SHIFT=4 MOD_WIN=8

	std::vector <Persistent<Function, CopyablePersistentTraits<Function>>>::iterator it = s_vecKeyEventCallback.begin();
	for (int i = 0; it != NaKeyboardModule::s_vecKeyEventCallback.end(); it++, i++)
	{
		if (i == nIdx)
		{
			Persistent<Function, CopyablePersistentTraits<Function>> percy = *it;
			Local<Function> callback = Local<Function>::New(isolate, percy);
			if (!callback.IsEmpty())
			{
				Local<Value> recv = isolate->GetCurrentContext()->Global();
				callback->Call(
					isolate->GetCurrentContext(),
					recv,
					0,
					NULL
				);
			}

			break;
		}
	}
}

// description: make key to down
// syntax:		system.keyboard.down(keycode)
void NaKeyboardModule::Down(V8_FUNCTION_ARGS)
{
	if (args.Length() <= 0)
		return;

	int nKey = args[0]->Int32Value();
	KeyDown(nKey);
}

// description: make key to up
// syntax:		system.keyboard.up(keycode)
void NaKeyboardModule::Up(V8_FUNCTION_ARGS)
{
	if (args.Length() <= 0)
		return;

	int nKey = args[0]->Int32Value();
	KeyUp(nKey);
}

// description: type string
// syntax:		system.keyboard.typeString(string)
void NaKeyboardModule::TypeString(V8_FUNCTION_ARGS)
{
	if (args.Length() <= 0)
		return;

	String::Value str(args[0]);
	int nDelay = 0;
	if (args.Length() >= 2)
		nDelay = args[1]->Int32Value();

	wchar_t *wstr = (wchar_t*)*str;
	int nLen = wcslen(wstr);
	for (int i = 0; i < nLen; i++)
	{
		int nKeyCode = NaKeyboardModule::GetKeycodeFromChar(*wstr);
		if (nKeyCode == -1)
			continue;
		*wstr++;

		int nModifier = nKeyCode >> 8;
		nKeyCode = nKeyCode & 0xff;

		if (nModifier & 1) KeyDown(VK_LSHIFT);
		if (nModifier & 2) KeyDown(VK_LCONTROL);
		if (nModifier & 4) KeyDown(VK_LMENU);

		KeyDown(nKeyCode);

		if (nDelay)
			::Sleep(nDelay);

		KeyUp(nKeyCode);

		if (nModifier & 1) KeyUp(VK_LSHIFT);
		if (nModifier & 2) KeyUp(VK_LCONTROL);
		if (nModifier & 4) KeyUp(VK_LMENU);
	}
}

// description: bind key event
// syntax:		system.keyboard.on(keycode, function)
void NaKeyboardModule::BindEvent(V8_FUNCTION_ARGS)
{
	if (args.Length() < 2)
		return;

	// TODO if args[0] is not keycode then parse it
	//      ex) "ctrl+z"

	int nKeycode = args[0]->Int32Value();
	Local<Object> callback = args[1]->ToObject();
	if (callback->IsFunction())
	{
		int nIdx = NaKeyboardModule::s_vecKeyEventCallback.size();

		Isolate *isolate = args.GetIsolate();
		Local<Function> callback_func = Local<Function>::Cast(args[1]);
		Persistent<Function, CopyablePersistentTraits<Function>> percy(isolate, callback_func);
		
		NaKeyboardModule::s_vecKeyEventCallback.push_back(percy);
		
		RegisterHotKey(NULL, nIdx, 0, nKeycode);
	}

	return;
}
