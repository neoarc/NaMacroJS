#pragma once

#include "V8Wrap.h"
#include "JsObjectBase.h"

#include <Windows.h>
#include <map>

#include "NaFile.h"

class JsFile : public JsObjectBase
{
public:
	JsFile();
	virtual ~JsFile();

	// members
	NaFile* m_pNativeFile;

	// static
	static NaFile* UnwrapNativeFile(Local<Object> obj);
	
	// wrap object
	virtual Local<ObjectTemplate> MakeObjectTemplate(Isolate *isolate);
	virtual Global<ObjectTemplate>& GetObjectTemplate() { return s_JsFileTemplate; }
	static Global<ObjectTemplate> s_JsFileTemplate;

	// accessors
	DEFINE_CLASS_ACCESSOR(name);
	DEFINE_CLASS_ACCESSOR_RO(exist);

	// methods
	DEFINE_CLASS_METHOD(constructor);
	DEFINE_CLASS_METHOD(read);
	DEFINE_CLASS_METHOD(write);
	DEFINE_CLASS_METHOD(close);
};