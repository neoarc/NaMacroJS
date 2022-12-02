#include "stdafx.h"
#include "MouseModule.h"

#include "V8Wrap.h"

#define MOUSECLICK_SLEEP	10

void NaMouseModule::Init(Isolate *isolate, Local<ObjectTemplate>& /*global_template*/)
{
	HandleScope handle_scope(isolate);

	Local<Object> global = isolate->GetCurrentContext()->Global();
	Local<Object> mouse_obj = GetMouseObject(isolate);

#define ADD_MOUSE_ACCESSOR(_prop)		ADD_OBJ_ACCESSOR(mouse_obj, _prop);
#define ADD_MOUSE_METHOD(_js_func)		ADD_OBJ_METHOD(mouse_obj, _js_func);

	// accessors
	ADD_MOUSE_ACCESSOR(x);
	ADD_MOUSE_ACCESSOR(y);

	// methods
	ADD_MOUSE_METHOD(move);
	ADD_MOUSE_METHOD(click);
	ADD_MOUSE_METHOD(lbuttonDown);
	ADD_MOUSE_METHOD(lbuttonUp);
	ADD_MOUSE_METHOD(rbuttonDown);
	ADD_MOUSE_METHOD(rbuttonUp);
	ADD_MOUSE_METHOD(wheelDown);
	ADD_MOUSE_METHOD(wheelUp);

	// Note: postXXX methods can be replaced
	//       VirtualMouse.move(0, 0);
	//       VirtualMouse.click(0, 0);
	ADD_MOUSE_METHOD(postMove);
	ADD_MOUSE_METHOD(postClick);
	ADD_MOUSE_METHOD(postLbuttonDown);
	ADD_MOUSE_METHOD(postLbuttonUp);
	ADD_MOUSE_METHOD(postRbuttonDown);
	ADD_MOUSE_METHOD(postRbuttonUp);
}

void NaMouseModule::Release()
{
}

// description: return 'system.mouse'
Local<Object> NaMouseModule::GetMouseObject(Isolate *isolate)
{
	// HandleScope �ȿ��� ȣ��

	Local<Object> system_obj = V8Wrap::GetSystemObject(isolate);
	Local<String> mouse_name = String::NewFromUtf8(isolate, "mouse", NewStringType::kNormal).ToLocalChecked();
	Local<Value> mouse_value = system_obj->Get(mouse_name);
	if (!mouse_value.IsEmpty() && mouse_value->IsUndefined())
	{
		// InitMouse
		mouse_value = Object::New(isolate);

		// temp; using template example
		/*
		Local<ObjectTemplate> templ = ObjectTemplate::New(isolate);

		templ->SetAccessor(
			String::NewFromUtf8(isolate, "x", NewStringType::kInternalized).ToLocalChecked(),
			GetX,
			SetX
			);

		mouse_value = templ->NewInstance(isolate->GetCurrentContext()).ToLocalChecked();
		*/
		system_obj->Set(mouse_name, mouse_value);
	}

	Local<Object> mouse_obj = mouse_value->ToObject();
	return mouse_obj;
}

// description: x getter 
void NaMouseModule::get_x(Local<String> /*name*/, const PropertyCallbackInfo<Value>& info)
{
	POINT pt;
	::GetCursorPos(&pt);

	V8Wrap::SetReturnValue(info, pt.x);
}

// description: x setter 
void NaMouseModule::set_x(Local<String> /*name*/, Local<Value> value, const PropertyCallbackInfo<void>& /*info*/)
{
	POINT pt;
	::GetCursorPos(&pt);

	pt.x = value->Int32Value();
	::SetCursorPos(pt.x, pt.y);
}

// description: y getter 
void NaMouseModule::get_y(Local<String> /*name*/, const PropertyCallbackInfo<Value>& info)
{
	POINT pt;
	::GetCursorPos(&pt);

	V8Wrap::SetReturnValue(info, pt.y);
}

// description: y setter 
void NaMouseModule::set_y(Local<String> /*name*/, Local<Value> value, const PropertyCallbackInfo<void>& /*info*/)
{
	POINT pt;
	::GetCursorPos(&pt);

	pt.y = value->Int32Value();
	::SetCursorPos(pt.x, pt.y);
}

// description: move mouse cursor
void NaMouseModule::method_move(V8_FUNCTION_ARGS)
{
	int x = args[0]->Int32Value();
	int y = args[1]->Int32Value();

	::SetCursorPos(x, y);
}

// description: click mouse lbutton
void NaMouseModule::method_click(V8_FUNCTION_ARGS)
{
	if (args.Length() >= 2)
		method_move(args);

	INPUT input;
	ZeroMemory(&input, sizeof(INPUT));
	input.type = INPUT_MOUSE;
	input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
	::SendInput(1, &input, sizeof(INPUT));
	::Sleep(MOUSECLICK_SLEEP);

	input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
	::SendInput(1, &input, sizeof(INPUT));
}

// description: 
void NaMouseModule::method_lbuttonDown(V8_FUNCTION_ARGS)
{
	if (args.Length() >= 2)
		method_move(args);

	INPUT input;
	ZeroMemory(&input, sizeof(INPUT));
	input.type = INPUT_MOUSE;
	input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
	::SendInput(1, &input, sizeof(INPUT));
}

// description: 
void NaMouseModule::method_lbuttonUp(V8_FUNCTION_ARGS)
{
	if (args.Length() >= 2)
		method_move(args);

	INPUT input;
	ZeroMemory(&input, sizeof(INPUT));
	input.type = INPUT_MOUSE;
	input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
	::SendInput(1, &input, sizeof(INPUT));
}

// description: 
void NaMouseModule::method_rbuttonDown(V8_FUNCTION_ARGS)
{
	if (args.Length() >= 2)
		method_move(args);

	INPUT input;
	ZeroMemory(&input, sizeof(INPUT));
	input.type = INPUT_MOUSE;
	input.mi.dwFlags = MOUSEEVENTF_RIGHTDOWN;
	::SendInput(1, &input, sizeof(INPUT));
}


// description: 
void NaMouseModule::method_rbuttonUp(V8_FUNCTION_ARGS)
{
	if (args.Length() >= 2)
		method_move(args);

	INPUT input;
	ZeroMemory(&input, sizeof(INPUT));
	input.type = INPUT_MOUSE;
	input.mi.dwFlags = MOUSEEVENTF_RIGHTUP;
	::SendInput(1, &input, sizeof(INPUT));
}


// description: 
void NaMouseModule::method_wheelDown(V8_FUNCTION_ARGS)
{
	if (args.Length() >= 2)
		method_move(args);

	INPUT input;
	ZeroMemory(&input, sizeof(INPUT));
	input.type = INPUT_MOUSE;
	input.mi.dwFlags = MOUSEEVENTF_WHEEL;
	// A positive value indicates that the wheel was rotated forward, away from the user;
	// a negative value indicates that the wheel was rotated backward, toward the user.
	// One wheel click is defined as WHEEL_DELTA, which is 120.
	input.mi.mouseData = (DWORD)-WHEEL_DELTA;
	::SendInput(1, &input, sizeof(INPUT));
}

// description: 
void NaMouseModule::method_wheelUp(V8_FUNCTION_ARGS)
{
	if (args.Length() >= 2)
		method_move(args);

	INPUT input;
	ZeroMemory(&input, sizeof(INPUT));
	input.type = INPUT_MOUSE;
	input.mi.dwFlags = MOUSEEVENTF_WHEEL;
	// A positive value indicates that the wheel was rotated forward, away from the user;
	// a negative value indicates that the wheel was rotated backward, toward the user.
	// One wheel click is defined as WHEEL_DELTA, which is 120.
	input.mi.mouseData = (DWORD)WHEEL_DELTA; 
	::SendInput(1, &input, sizeof(INPUT));
}

// description: 
void NaMouseModule::method_postMove(V8_FUNCTION_ARGS)
{
	if (args.Length() >= 2)
	{
		int x = args[0]->Int32Value();
		int y = args[1]->Int32Value();

		POINT pt = { x, y };
		HWND hWnd = WindowFromPoint(pt);
		::ScreenToClient(hWnd, &pt);

		PostMessage(hWnd, WM_MOUSEMOVE, MK_LBUTTON, MAKELPARAM(pt.x, pt.y));
	}
}

// description: 
void NaMouseModule::method_postClick(V8_FUNCTION_ARGS)
{
	if (args.Length() >= 2)
	{
		method_postLbuttonDown(args);
		::Sleep(MOUSECLICK_SLEEP);

		method_postLbuttonUp(args);
	}
}

// description: 
void NaMouseModule::method_postLbuttonDown(V8_FUNCTION_ARGS)
{
	if (args.Length() >= 2)
	{
		int x = args[0]->Int32Value();
		int y = args[1]->Int32Value();

		POINT pt = { x, y };
		HWND hWnd = WindowFromPoint(pt);
		::ScreenToClient(hWnd, &pt);

		PostMessage(hWnd, WM_LBUTTONDOWN, MK_LBUTTON, MAKELPARAM(pt.x, pt.y));
	}
}

// description: 
void NaMouseModule::method_postLbuttonUp(V8_FUNCTION_ARGS)
{
	if (args.Length() >= 2)
	{
		int x = args[0]->Int32Value();
		int y = args[1]->Int32Value();

		POINT pt = { x, y };
		HWND hWnd = WindowFromPoint(pt);
		::ScreenToClient(hWnd, &pt);

		PostMessage(hWnd, WM_LBUTTONUP, MK_LBUTTON, MAKELPARAM(pt.x, pt.y));
	}
}

// description: 
void NaMouseModule::method_postRbuttonDown(V8_FUNCTION_ARGS)
{
	if (args.Length() >= 2)
	{
		int x = args[0]->Int32Value();
		int y = args[1]->Int32Value();

		POINT pt = { x, y };
		HWND hWnd = WindowFromPoint(pt);
		::ScreenToClient(hWnd, &pt);

		PostMessage(hWnd, WM_RBUTTONDOWN, MK_RBUTTON, MAKELPARAM(pt.x, pt.y));
	}
}

// description: 
void NaMouseModule::method_postRbuttonUp(V8_FUNCTION_ARGS)
{
	if (args.Length() >= 2)
	{
		int x = args[0]->Int32Value();
		int y = args[1]->Int32Value();

		POINT pt = { x, y };
		HWND hWnd = WindowFromPoint(pt);
		::ScreenToClient(hWnd, &pt);

		PostMessage(hWnd, WM_RBUTTONUP, MK_RBUTTON, MAKELPARAM(pt.x, pt.y));
	}
}
