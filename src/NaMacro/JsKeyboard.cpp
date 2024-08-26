#include "stdafx.h"
#include "JsKeyboard.h"
#include "NaKeyboard.h"

std::map <HotkeyKey, Persistent<Function, CopyablePersistentTraits<Function>>> JsKeyboard::s_mapKeyEventCallback;

void JsKeyboard::Init(Isolate *isolate, Local<ObjectTemplate>& /*global_template*/)
{
	HandleScope handle_scope(isolate);

	auto global = isolate->GetCurrentContext()->Global();
	auto keyboardObj = GetKeyboardObject(isolate);

#define ADD_KEYBOARD_METHOD(_js_func)			ADD_OBJ_METHOD(keyboardObj, _js_func);
#define ADD_KEYBOARD_PROPERTY(_prop, _getter, _setter)	ADD_OBJ_PROPERTY(keyboardObj, _prop, _getter, _setter);
	
	// accessors

	// methods
	ADD_KEYBOARD_METHOD(down);
	ADD_KEYBOARD_METHOD(up);
	ADD_KEYBOARD_METHOD(typeString);
	ADD_KEYBOARD_METHOD(on);
}

void JsKeyboard::Release()
{
	auto it = s_mapKeyEventCallback.begin();
	for (; it != JsKeyboard::s_mapKeyEventCallback.end(); it++)
	{
		HotkeyKey key = it->first;
		::UnregisterHotKey(NULL, key.index);
	}

	// TODO check Clear Persistent?

	JsKeyboard::s_mapKeyEventCallback.clear();
}

// must called in HandleScope
// return 'system.keyboard'
Local<Object> JsKeyboard::GetKeyboardObject(Isolate *isolate)
{
	auto systemObj = V8Wrap::GetSystemObject(isolate);
	auto keyboardName = V8_STRING("keyboard");
	auto keyboardValue = systemObj->Get(keyboardName);
	if (!keyboardValue.IsEmpty() && keyboardValue->IsUndefined())
	{
		// Init keyboard
		keyboardValue = Object::New(isolate);
		systemObj->Set(keyboardName, keyboardValue);
	}

	auto keyboardObj = keyboardValue->ToObject();
	return keyboardObj;
}

// description: key event handler
void JsKeyboard::OnHotkey(Isolate *isolate, WPARAM wParam, LPARAM /*lParam*/)
{
	auto index = (int)wParam;
	auto it = s_mapKeyEventCallback.begin();
	for (; it != JsKeyboard::s_mapKeyEventCallback.end(); it++)
	{
		HotkeyKey key = it->first;
		if (key.index == index)
		{
			Persistent<Function, CopyablePersistentTraits<Function>> percy = it->second;
			Local<Function> callback = Local<Function>::New(isolate, percy);
			if (!callback.IsEmpty())
			{
				Local<Value> recv = isolate->GetCurrentContext()->Global();

				::UnregisterHotKey(NULL, key.index);

				callback->Call(
					isolate->GetCurrentContext(),
					recv,
					0,
					NULL
				);

				if (it._Getcont() != JsKeyboard::s_mapKeyEventCallback.end()._Getcont())
				{
					// user unregistered this hotkey!
					break;
				}

				::RegisterHotKey(NULL, key.index, key.modifier, key.keycode);
			}

			break;
		}
	}
}

// description: make key to down
// syntax:		system.keyboard.down(keycode)
void JsKeyboard::method_down(V8_METHOD_ARGS)
{
	if (args.Length() <= 0)
		return;

	auto key = args[0]->Int32Value();
	NaKeyboard::KeyDown(key);
}

// description: make key to up
// syntax:		system.keyboard.up(keycode)
void JsKeyboard::method_up(V8_METHOD_ARGS)
{
	if (args.Length() <= 0)
		return;

	auto key = args[0]->Int32Value();
	NaKeyboard::KeyUp(key);
}

// description: type string
// syntax:		system.keyboard.typeString(string)
void JsKeyboard::method_typeString(V8_METHOD_ARGS)
{
	if (args.Length() <= 0)
		return;

	String::Value str(args[0]);
	int delay = 0;
	if (args.Length() >= 2)
		delay = args[1]->Int32Value();

	auto wstr = (wchar_t*)*str;
	NaKeyboard::TypeString(wstr, delay);
}

// description: bind key event
// syntax:		system.keyboard.on(keycode, function)
void JsKeyboard::method_on(V8_METHOD_ARGS)
{
	if (args.Length() < 2)
		return;

	// TODO if args[0] is not keycode then parse it
	//      ex) "ctrl+z"

	int keycode = args[0]->Int32Value();
	int modifier = 0;
	if (args[1]->IsNull())
	{
		auto it = s_mapKeyEventCallback.begin();
		for (; it != JsKeyboard::s_mapKeyEventCallback.end(); it++)
		{
			HotkeyKey key = it->first;
			if (key.keycode == keycode &&
				key.modifier == modifier)
			{
				auto unregisterResult = UnregisterHotKey(NULL, key.index);
				if (!unregisterResult)
				{
					auto dwError = GetLastError();
					if (dwError == 1419)
					{
						//DebugBreak();
					}
				}
				JsKeyboard::s_mapKeyEventCallback.erase(it);
				break;
			}
		}

	}
	else 
	{
		auto callback = args[1]->ToObject();
		if (callback->IsFunction())
		{
			auto index = JsKeyboard::s_mapKeyEventCallback.size();
			auto isolate = args.GetIsolate();
			auto callbackFunc = Local<Function>::Cast(args[1]);
			Persistent<Function, CopyablePersistentTraits<Function>> percy(isolate, callbackFunc);

			HotkeyKey key;
			key.keycode = keycode;
			key.modifier = 0;
			key.index = index;

			JsKeyboard::s_mapKeyEventCallback.insert(std::make_pair(key, percy));
			RegisterHotKey(NULL, index, 0, keycode);
		}
	}	
}
