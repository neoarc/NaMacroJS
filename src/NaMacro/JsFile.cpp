#include "stdafx.h"
#include "JsFile.h"

#include <NaLib/NaDebug.h>

Global<ObjectTemplate> JsFile::s_JsFileTemplate;

JsFile::JsFile()
{
	NaDebug::Out(L"JsFile(): 0x%08x\n", this);
	m_pNativeFile = nullptr;
}

JsFile::~JsFile()
{
	NaDebug::Out(L"~JsFile(): 0x%08x\n", this);
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
#define ADD_FILE_ACCESSOR(_prop)	ADD_OBJ_ACCESSOR(templ, _prop);
#define ADD_FILE_ACCESSOR_RO(_prop)	ADD_OBJ_ACCESSOR_RO(templ, _prop);
#define ADD_FILE_METHOD(_js_func)	ADD_TEMPLATE_METHOD(templ, _js_func);

	// accessor
	ADD_FILE_ACCESSOR(name);
	ADD_FILE_ACCESSOR_RO(exist);

	// methods
	ADD_FILE_METHOD(read);
	ADD_FILE_METHOD(write);
	ADD_FILE_METHOD(close);

	return handle_scope.Escape(templ);
}

// description: name property getter/setter
void JsFile::get_name(V8_GETTER_ARGS)
{
	UNUSED_PARAMETER(name);
	
	auto pFile = UnwrapNativeFile(info.This());
	if (pFile)
	{
		V8Wrap::SetReturnValueAsString(
			info.GetReturnValue(),
			pFile->m_strName.wstr()
		);
	}
}

void JsFile::set_name(V8_SETTER_ARGS)
{
	UNUSED_PARAMETER(name);
	UNUSED_PARAMETER(value);
	UNUSED_PARAMETER(info);

	// Not Impl
}

// description: exist property getter
void JsFile::get_exist(V8_GETTER_ARGS)
{
	UNUSED_PARAMETER(name);

	auto pFile = UnwrapNativeFile(info.This());
	if (pFile)
	{
		Isolate *isolate = info.GetIsolate();
		info.GetReturnValue().Set(
			Boolean::New(isolate, pFile->IsExist())
		);
	}
}

// description: constructor function
// syntax:		new Window([x, y[, width, height]]) : windowObj
void JsFile::method_constructor(V8_FUNCTION_ARGS)
{
	if (args.Length() >= 1)
	{
		Isolate *isolate = args.GetIsolate();

		String::Value filepath(args[0]);

		Local<StackTrace> stack_trace = StackTrace::CurrentStackTrace(
			isolate, 1, StackTrace::kScriptName);
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

	V8Wrap::SetReturnValueAsNull(args.GetReturnValue());
}

// description: read file
// syntax:		fileObj.read() : string
void JsFile::method_read(V8_FUNCTION_ARGS)
{
	auto pFile = UnwrapNativeFile(args.This());
	if (pFile == nullptr)
	{
		// error
		V8Wrap::SetReturnValueAsInteger(args.GetReturnValue(), -1);
		return;
	}

	if (pFile->m_hFile == nullptr)
	{
		V8Wrap::SetReturnValueAsNull(args.GetReturnValue());
		return;
	}

	// #TODO smart pointer?
	auto buf = pFile->Read();
	V8Wrap::SetReturnValueAsString(args.GetReturnValue(), buf);
	
	delete[] buf;
}

// description: write file
// syntax:		fileObj.write(text);
void JsFile::method_write(V8_FUNCTION_ARGS)
{
	Isolate *isolate = args.GetIsolate();

	auto pFile = UnwrapNativeFile(args.This());
	if (pFile == nullptr || args.Length() < 1)
	{
		// error
		V8Wrap::SetReturnValueAsInteger(args.GetReturnValue(), -1);
		return;
	}

	if (pFile->m_hFile == nullptr)
	{
		V8Wrap::SetReturnValueAsNull(args.GetReturnValue());
		return;
	}

	String::Value strV8(args[0]);
	NaString str((wchar_t*)*strV8);
	auto ret = pFile->Write(str);

	// return
	args.GetReturnValue().Set(Integer::New(isolate, ret));
}

// description: close file
// syntax:		fileObj.close()
void JsFile::method_close(V8_FUNCTION_ARGS)
{
	Isolate *isolate = args.GetIsolate();
	
	auto pFile = UnwrapNativeFile(args.This());
	if (pFile == nullptr)
	{
		// error
		V8Wrap::SetReturnValueAsInteger(args.GetReturnValue(), -1);
		return;
	}

	pFile->Close();

	// return
	args.GetReturnValue().Set(Boolean::New(isolate, true));
}
