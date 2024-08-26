#include "stdafx.h"
#include "JsMouse.h"

#include "V8Wrap.h"

#include "NaMouse.h"

void JsMouse::Init(Isolate *isolate, Local<ObjectTemplate>& /*global_template*/)
{
	HandleScope handle_scope(isolate);

	auto global = isolate->GetCurrentContext()->Global();
	auto mouseObj = GetMouseObject(isolate);

#define ADD_MOUSE_PROPERTY(_prop)		ADD_OBJ_PROPERTY(mouseObj, _prop);
#define ADD_MOUSE_METHOD(_js_func)		ADD_OBJ_METHOD(mouseObj, _js_func);

	// accessors
	ADD_MOUSE_PROPERTY(x);
	ADD_MOUSE_PROPERTY(y);

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

void JsMouse::Release()
{
}

// must called in HandleScope
// return 'system.mouse'
Local<Object> JsMouse::GetMouseObject(Isolate *isolate)
{
	// HandleScope 안에서 호출

	auto systemObj = V8Wrap::GetSystemObject(isolate);
	auto mouseName = String::NewFromUtf8(isolate, "mouse", NewStringType::kNormal).ToLocalChecked();
	auto mouseValue = systemObj->Get(mouseName);
	if (!mouseValue.IsEmpty() && mouseValue->IsUndefined())
	{
		// InitMouse
		mouseValue = Object::New(isolate);

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
		systemObj->Set(mouseName, mouseValue);
	}

	auto mouseObj = mouseValue->ToObject();
	return mouseObj;
}

void JsMouse::get_x(Local<String> /*name*/, const PropertyCallbackInfo<Value>& info)
{
	auto pos = NaMouse::GetPosition();	
	V8_PROP_RET(pos.x);
}

void JsMouse::set_x(Local<String> /*name*/, Local<Value> value, const PropertyCallbackInfo<void>& /*info*/)
{
	auto pos = NaMouse::GetPosition();
	NaMouse::SetPosition(pos.x, value->Int32Value());	
}

void JsMouse::get_y(Local<String> /*name*/, const PropertyCallbackInfo<Value>& info)
{
	auto pos = NaMouse::GetPosition();
	V8_PROP_RET(pos.y);
}

void JsMouse::set_y(Local<String> /*name*/, Local<Value> value, const PropertyCallbackInfo<void>& /*info*/)
{
	auto pos = NaMouse::GetPosition();
	NaMouse::SetPosition(pos.x, value->Int32Value());
}

void JsMouse::method_move(V8_METHOD_ARGS)
{
	auto x = args[0]->Int32Value();
	auto y = args[1]->Int32Value();
	NaMouse::Move(x, y);	
}

void JsMouse::method_click(V8_METHOD_ARGS)
{
	if (args.Length() >= 2)
		method_move(args);

	NaMouse::Click();
}

void JsMouse::method_lbuttonDown(V8_METHOD_ARGS)
{
	if (args.Length() >= 2)
		method_move(args);

	NaMouse::LbuttonDown();
}

void JsMouse::method_lbuttonUp(V8_METHOD_ARGS)
{
	if (args.Length() >= 2)
		method_move(args);

	NaMouse::LbuttonUp();
}

void JsMouse::method_rbuttonDown(V8_METHOD_ARGS)
{
	if (args.Length() >= 2)
		method_move(args);

	NaMouse::RbuttonDown();
}

void JsMouse::method_rbuttonUp(V8_METHOD_ARGS)
{
	if (args.Length() >= 2)
		method_move(args);

	NaMouse::RbuttonUp();
}

void JsMouse::method_wheelDown(V8_METHOD_ARGS)
{
	if (args.Length() >= 2)
		method_move(args);

	NaMouse::WheelDown();
}

void JsMouse::method_wheelUp(V8_METHOD_ARGS)
{
	if (args.Length() >= 2)
		method_move(args);

	NaMouse::WheelUp();
}

void JsMouse::method_postMove(V8_METHOD_ARGS)
{
	if (args.Length() >= 2)
	{
		auto x = args[0]->Int32Value();
		auto y = args[1]->Int32Value();

		NaMouse::PostMove(x, y);
	}
}

void JsMouse::method_postClick(V8_METHOD_ARGS)
{
	if (args.Length() >= 2)
	{
		auto x = args[0]->Int32Value();
		auto y = args[1]->Int32Value();

		NaMouse::PostClick(x, y);
	}
}

void JsMouse::method_postLbuttonDown(V8_METHOD_ARGS)
{
	if (args.Length() >= 2)
	{
		auto x = args[0]->Int32Value();
		auto y = args[1]->Int32Value();

		NaMouse::PostLbuttonDown(x, y);
	}
}

void JsMouse::method_postLbuttonUp(V8_METHOD_ARGS)
{
	if (args.Length() >= 2)
	{
		auto x = args[0]->Int32Value();
		auto y = args[1]->Int32Value();

		NaMouse::PostLbuttonUp(x, y);
	}
}

void JsMouse::method_postRbuttonDown(V8_METHOD_ARGS)
{
	if (args.Length() >= 2)
	{
		auto x = args[0]->Int32Value();
		auto y = args[1]->Int32Value();

		NaMouse::PostRbuttonDown(x, y);
	}
}

void JsMouse::method_postRbuttonUp(V8_METHOD_ARGS)
{
	if (args.Length() >= 2)
	{
		auto x = args[0]->Int32Value();
		auto y = args[1]->Int32Value();

		NaMouse::PostRbuttonUp(x, y);
	}
}
