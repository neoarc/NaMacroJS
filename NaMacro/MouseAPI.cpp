#include "stdafx.h"

#include "MouseAPI.h"

#include "Windows.h"

void InitMouseAPI(v8::Handle<v8::ObjectTemplate>& global_template)
{
	v8::HandleScope handle_scope;

	v8::Handle<v8::Object> global = v8::Context::GetCurrent()->Global();
	v8::Local<v8::String> system_name = v8::String::New("system");

	// GetSystem
	v8::Local<v8::Value> system_value = global->Get(system_name);
	if (!system_value.IsEmpty() && system_value->IsUndefined())
	{
		// InitSystem
		system_value = v8::Object::New();
		global->Set(system_name, system_value);
	}

	// GetMouse
	v8::Local<v8::Object> system_obj = system_value->ToObject();
	v8::Local<v8::String> mouse_name = v8::String::New("mouse");
	v8::Local<v8::Value> mouse_value = system_obj->Get(mouse_name);
	if (!mouse_value.IsEmpty() && mouse_value->IsUndefined())
	{
		// InitSystem
		mouse_value = v8::Object::New();
		system_obj->Set(mouse_name, mouse_value);
	}

	v8::Local<v8::Object> mouse_obj = mouse_value->ToObject();

#define ADD_MOUSE_API(_js_func, _c_func) mouse_obj->Set(v8::String::New(#_js_func), v8::FunctionTemplate::New(_c_func)->GetFunction());
	ADD_MOUSE_API(move, MouseMove);

}

v8::Handle<v8::Value> MouseMove(const v8::Arguments& args)
{
	int x = args[0]->Int32Value();
	int y = args[0]->Int32Value();

	::SetCursorPos(x, y);

	return v8::Undefined();
}