#include "stdafx.h"
#include "JsProcess.h"

#include <NaLib/NaDebug.h>

Global<ObjectTemplate> JsProcess::s_JsProcessTemplate;

JsProcess::JsProcess()
{
	NaDebug::Out(L"JsProcess(): 0x%08x\n", this);
	m_pNativeProcess = nullptr;
}

JsProcess::~JsProcess()
{
	NaDebug::Out(L"~JsProcess(): 0x%08x\n", this);
}

NaProcess* JsProcess::UnwrapNativeProcess(Local<Object> obj)
{
	JsProcess* pJsProcess = static_cast<JsProcess*>(UnwrapObject(obj));
	if (pJsProcess && pJsProcess->m_pNativeProcess)
	{
		return pJsProcess->m_pNativeProcess;
	}

	return nullptr;
}

Local<ObjectTemplate> JsProcess::MakeObjectTemplate(Isolate * isolate)
{
	EscapableHandleScope handle_scope(isolate);

	Local<ObjectTemplate> templ = ObjectTemplate::New(isolate);
	templ->SetInternalFieldCount(1);

	// bind file methods
#define ADD_PROCESS_ACCESSOR(_prop)	ADD_OBJ_ACCESSOR(templ, _prop);
#define ADD_PROCESS_ACCESSOR_RO(_prop)	ADD_OBJ_ACCESSOR_RO(templ, _prop);
#define ADD_PROCESS_METHOD(_js_func)	ADD_TEMPLATE_METHOD(templ, _js_func);

	// accessor
	ADD_PROCESS_ACCESSOR(name);
	ADD_PROCESS_ACCESSOR_RO(exist);

	// methods
	ADD_PROCESS_METHOD(terminate);

	return handle_scope.Escape(templ);
}

// description: name property getter/setter
void JsProcess::get_name(V8_GETTER_ARGS)
{
	UNUSED_PARAMETER(name);
	
	auto pProcess = UnwrapNativeProcess(info.This());
	if (pProcess)
	{
		V8Wrap::SetReturnValueAsString(
			info.GetReturnValue(),
			pProcess->m_strName.wstr()
		);
	}
}

void JsProcess::set_name(V8_SETTER_ARGS)
{
	UNUSED_PARAMETER(name);
	UNUSED_PARAMETER(value);
	UNUSED_PARAMETER(info);

	// Not Impl
}

// description: exist property getter
void JsProcess::get_exist(V8_GETTER_ARGS)
{
	UNUSED_PARAMETER(name);

	auto pProcess = UnwrapNativeProcess(info.This());
	if (pProcess)
	{
		// #TODO impl
		/*
		Isolate *isolate = info.GetIsolate();
		info.GetReturnValue().Set(
			Boolean::New(isolate, pProcess->IsExist())
		);
		*/
	}
}

void JsProcess::method_constructor(V8_FUNCTION_ARGS)
{
	if (args.Length() >= 1)
	{
		Isolate *isolate = args.GetIsolate();
		JsProcess *pJsProcess = new JsProcess();
		
		// #TODO impl
		//pJsProcess->m_pNativeProcess = NaProcess::Load(strFullPath.wstr(), strMode.cstr());

		Local<Object> obj = WrapObject(isolate, pJsProcess);
		args.GetReturnValue().Set(obj);

		return;
	}

	V8Wrap::SetReturnValueAsNull(args.GetReturnValue());
}

void JsProcess::method_terminate(V8_FUNCTION_ARGS)
{
	Isolate *isolate = args.GetIsolate();
	
	auto pProcess = UnwrapNativeProcess(args.This());
	if (pProcess == nullptr)
	{
		// error
		V8Wrap::SetReturnValueAsInteger(args.GetReturnValue(), -1);
		return;
	}

	pProcess->Terminate();

	// return
	args.GetReturnValue().Set(Boolean::New(isolate, true));
}
