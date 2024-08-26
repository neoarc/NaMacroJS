#include "stdafx.h"
#include "JsProcess.h"

#include <algorithm>

#include <NaLib/NaDebug.h>
#include <NaLib/NaProcess.h>

Global<ObjectTemplate> JsProcess::s_JsProcessTemplate;

JsProcess::JsProcess()
{
	NaDebugOut(L"JsProcess(): 0x%08x\n", this);
	m_pNativeProcess = nullptr;
}

JsProcess::~JsProcess()
{
	NaDebugOut(L"~JsProcess(): 0x%08x\n", this);
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

void JsProcess::FindProcesses(Isolate * isolate, const wchar_t * name, Local<Array>& results)
{
	NaProcess::FindProcessInfo info;
	info.name = const_cast<wchar_t*>(name);
	NaProcess::FindProcesses(name, info);

	// Wrap HANDLE to V8Object
	int nIndex = 0;
	for_each(info.foundlist.begin(), info.foundlist.end(), 
		[&](HANDLE h)
		{
			JsProcess *pJsProcess = new JsProcess();
			pJsProcess->m_pNativeProcess = NaProcess::GetProcess(h);

			Local<Value> obj = JsProcess::WrapObject(isolate, pJsProcess);

			// Fill V8Object Array
			results->Set(nIndex++, obj);
		}
	);
}

Local<ObjectTemplate> JsProcess::MakeObjectTemplate(Isolate * isolate)
{
	EscapableHandleScope handle_scope(isolate);

	Local<ObjectTemplate> templ = ObjectTemplate::New(isolate);
	templ->SetInternalFieldCount(1);

	// bind file methods
#define ADD_PROCESS_PROPERTY(_prop)	ADD_OBJ_PROPERTY(templ, _prop);
#define ADD_PROCESS_PROPERTY_RO(_prop)	ADD_OBJ_PROPERTY_RO(templ, _prop);
#define ADD_PROCESS_METHOD(_js_func)	ADD_TEMPLATE_METHOD(templ, _js_func);

	// accessor
	ADD_PROCESS_PROPERTY(name);
	ADD_PROCESS_PROPERTY_RO(exist);

	// methods
	ADD_PROCESS_METHOD(terminate);

	return handle_scope.Escape(templ);
}

// description: name property getter/setter
void JsProcess::get_name(V8_GETTER_ARGS)
{
	UNUSED(name);
	
	auto pProcess = UnwrapNativeProcess(info.This());
	if (pProcess)
	{
		V8_PROP_RET(pProcess->m_strName.wstr());
	}
}

void JsProcess::set_name(V8_SETTER_ARGS)
{
	UNUSED(name);
	UNUSED(value);
	UNUSED(info);

	// Not Impl
}

// description: exist property getter
void JsProcess::get_exist(V8_GETTER_ARGS)
{
	UNUSED(name);

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

void JsProcess::method_constructor(V8_METHOD_ARGS)
{
	if (args.Length() >= 1)
	{
		Isolate *isolate = args.GetIsolate();
		JsProcess *pJsProcess = new JsProcess();
		
		// #TODO impl
		//pJsProcess->m_pNativeProcess = NaProcess::Load(strFullPath.wstr(), strMode.cstr());

		Local<Object> obj = WrapObject(isolate, pJsProcess);

		V8_METHOD_RET(obj);
		return;
	}

	V8Wrap::NullReturnValue(args);
}

void JsProcess::method_terminate(V8_METHOD_ARGS)
{
	auto pProcess = UnwrapNativeProcess(args.This());
	if (pProcess == nullptr)
	{
		// error
		V8_METHOD_RET(-1);
		return;
	}

	pProcess->Terminate();

	// return
	V8_METHOD_RET(true);
}
