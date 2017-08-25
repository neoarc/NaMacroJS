#pragma once

#include "V8Wrap.h"
#include "JsObjectBase.h"

#include <Windows.h>
#include <map>

#include <NaLib/NaProcess.h>

class JsProcess : public JsObjectBase
{
public:
	JsProcess();
	virtual ~JsProcess();

	// members
	NaProcess* m_pNativeProcess;

	// static
	static NaProcess* UnwrapNativeProcess(Local<Object> obj);

	static void FindProcesses(Isolate *isolate, const wchar_t *name, Local<Array> &results);
	
	// wrap object
	virtual Local<ObjectTemplate> MakeObjectTemplate(Isolate *isolate);
	virtual Global<ObjectTemplate>& GetObjectTemplate() { return s_JsProcessTemplate; }
	static Global<ObjectTemplate> s_JsProcessTemplate;

	// accessors
	DEFINE_CLASS_ACCESSOR(name);
	DEFINE_CLASS_ACCESSOR_RO(exist);

	// methods
	DEFINE_CLASS_METHOD(constructor);
	DEFINE_CLASS_METHOD(terminate);
};