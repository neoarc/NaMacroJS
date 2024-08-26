#include "stdafx.h"
#include "JsFile.h"

#include <NaLib/NaFile.h>
#include <NaLib/NaDebug.h>

Global<ObjectTemplate> JsFile::s_JsFileTemplate;

JsFile::JsFile()
{
	NaDebugOut(L"JsFile(): 0x%08x\n", this);
	m_pNativeFile = nullptr;
}

JsFile::~JsFile()
{
	NaDebugOut(L"~JsFile(): 0x%08x\n", this);
}

NaFile * JsFile::UnwrapNativeFile(Local<Object> obj)
{
	JsFile* pJsFile = static_cast<JsFile*>(UnwrapObject(obj));
	if (pJsFile && pJsFile->m_pNativeFile)
	{
		return pJsFile->m_pNativeFile;
	}

	return nullptr;
}

Local<ObjectTemplate> JsFile::MakeObjectTemplate(Isolate * isolate)
{
	EscapableHandleScope handle_scope(isolate);

	Local<ObjectTemplate> templ = ObjectTemplate::New(isolate);
	templ->SetInternalFieldCount(1);

	// bind file methods
#define ADD_FILE_PROPERTY(_prop)	ADD_OBJ_PROPERTY(templ, _prop);
#define ADD_FILE_PROPERTY_RO(_prop)	ADD_OBJ_PROPERTY_RO(templ, _prop);
#define ADD_FILE_METHOD(_js_func)	ADD_TEMPLATE_METHOD(templ, _js_func);

	// accessor
	ADD_FILE_PROPERTY(name);
	ADD_FILE_PROPERTY_RO(exist);

	// methods
	ADD_FILE_METHOD(read);
	ADD_FILE_METHOD(write);
	ADD_FILE_METHOD(close);

	return handle_scope.Escape(templ);
}

// description: name property getter/setter
void JsFile::get_name(V8_GETTER_ARGS)
{
	UNUSED(name);
	
	auto pFile = UnwrapNativeFile(info.This());
	if (pFile)
	{
		V8_PROP_RET(pFile->m_strName.wstr());
	}
}

void JsFile::set_name(V8_SETTER_ARGS)
{
	UNUSED(name);
	UNUSED(value);
	UNUSED(info);

	// Not Impl
}

// description: exist property getter
void JsFile::get_exist(V8_GETTER_ARGS)
{
	UNUSED(name);

	auto pFile = UnwrapNativeFile(info.This());
	if (pFile)
	{
		V8_PROP_RET(pFile->IsExist());
	}
}

// description: constructor function
// syntax:		new Window([x, y[, width, height]]) : windowObj
void JsFile::method_constructor(V8_METHOD_ARGS)
{
	if (args.Length() >= 1)
	{
		Isolate *isolate = args.GetIsolate();

		String::Value filepath(args[0]);

		Local<StackTrace> stack_trace = StackTrace::CurrentStackTrace(isolate, 1, StackTrace::kScriptName);
		Local<StackFrame> cur_frame = stack_trace->GetFrame(0);
		
		// Calculate fullpath based on current javascript path
		NaString strBase(*String::Utf8Value(cur_frame->GetScriptName()));
		NaString strFilePath(*String::Utf8Value(args[0]));

		NaUrl url;
		url.SetBase(strBase);
		url.SetUrl(strFilePath);

		NaString strFullPath(url.GetFullUrl());
		NaString strMode;
		if (args.Length() >= 2)
		{
			String::Value arg_mode(args[1]);
			strMode = (wchar_t*)*arg_mode;
		}
		else
		{
			strMode = L"r";
		}

		JsFile *pJsFile = new JsFile();
		pJsFile->m_pNativeFile = NaFile::Load(strFullPath.wstr(), strMode.cstr());

		Local<Object> obj = WrapObject(isolate, pJsFile);

		args.GetReturnValue().Set(obj);
		return;
	}

	V8Wrap::NullReturnValue(args);
}

// description: read file
// syntax:		fileObj.read() : string
void JsFile::method_read(V8_METHOD_ARGS)
{
	auto pFile = UnwrapNativeFile(args.This());
	if (pFile == nullptr)
	{
		// error
		V8_METHOD_RET(-1);
		return;
	}

	if (pFile->m_hFile == nullptr)
	{
		V8Wrap::NullReturnValue(args);
		return;
	}

	// #TODO smart pointer?
	auto buf = pFile->Read();
	V8_METHOD_RET(buf);
	
	delete[] buf;
}

// description: write file
// syntax:		fileObj.write(text);
void JsFile::method_write(V8_METHOD_ARGS)
{
	auto pFile = UnwrapNativeFile(args.This());
	if (pFile == nullptr || args.Length() < 1)
	{
		// error
		V8_METHOD_RET(-1);
		return;
	}

	if (pFile->m_hFile == nullptr)
	{
		V8Wrap::NullReturnValue(args);
		return;
	}

	String::Value strV8(args[0]);
	NaString str((wchar_t*)*strV8);
	auto ret = pFile->Write(str);

	// return
	V8_METHOD_RET((int)ret);
}

// description: close file
// syntax:		fileObj.close()
void JsFile::method_close(V8_METHOD_ARGS)
{
	auto pFile = UnwrapNativeFile(args.This());
	if (pFile == nullptr)
	{
		// error
		V8_METHOD_RET(-1);
		return;
	}

	pFile->Close();

	// return
	V8_METHOD_RET(true);
}
