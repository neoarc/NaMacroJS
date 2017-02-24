#include "NaFile.h"

Global<ObjectTemplate> NaFile::s_NaFileTemplate;

NaFile::NaFile()
{
	m_hFile = NULL;
	NaDebugOut(L"NaFile(): 0x%08x\n", this);
}

NaFile::~NaFile()
{
	NaDebugOut(L"~NaFile(): 0x%08x\n", this);

	if (m_hFile)
	{
		fclose(m_hFile);
	}
}

Local<ObjectTemplate> NaFile::MakeObjectTemplate(Isolate * isolate)
{
	EscapableHandleScope handle_scope(isolate);

	Local<ObjectTemplate> templ = ObjectTemplate::New(isolate);
	templ->SetInternalFieldCount(1);

	// bind image methods
#define ADD_IMAGE_ACCESSOR(_prop)	ADD_OBJ_ACCESSOR(templ, _prop);
#define ADD_IMAGE_METHOD(_js_func)	ADD_TEMPLATE_METHOD(templ, _js_func);

	// accessor
	ADD_IMAGE_ACCESSOR(name);

	// methods
	ADD_IMAGE_METHOD(read);
	ADD_IMAGE_METHOD(write);
	ADD_IMAGE_METHOD(close);

	return handle_scope.Escape(templ);
}

// description: load file for read/write
NaFile * NaFile::Load(const wchar_t * filename, const char * mode)
{
	NaFile *pFile = new NaFile;

	pFile->m_strName = filename;
	pFile->m_hFile = fopen(pFile->m_strName.cstr(), mode);

	return pFile;
}

// description: width property getter/setter
void NaFile::get_name(V8_GETTER_ARGS)
{
	NaFile *pFile = reinterpret_cast<NaFile*>(UnwrapObject(info.This()));
	Isolate *isolate = info.GetIsolate();
	if (pFile)
	{
		info.GetReturnValue().Set(
			String::NewFromUtf8(isolate, pFile->m_strName.cstr(), NewStringType::kNormal).ToLocalChecked()
		);
	}
}

void NaFile::set_name(V8_SETTER_ARGS)
{
	NaFile *pFile = reinterpret_cast<NaFile*>(UnwrapObject(info.This()));
	if (pFile)
	{
		// Not Impl
	}
}

// description: constructor function
// syntax:		new Window([x, y[, width, height]]) : windowObj
void NaFile::method_constructor(V8_FUNCTION_ARGS)
{
	if (args.Length() >= 1)
	{
		String::Value filepath(args[0]);

		Local<StackTrace> stack_trace = StackTrace::CurrentStackTrace(
			args.GetIsolate(), 1, StackTrace::kScriptName);
		Local<StackFrame> cur_frame = stack_trace->GetFrame(0);
		NaString strBase(cur_frame->GetScriptName());
		NaString strFilePath(filepath);

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

		NaFile *pFile = NaFile::Load(strFullPath.wstr(), strMode.cstr());
		Local<Object> obj = WrapObject(args.GetIsolate(), pFile);

		args.GetReturnValue().Set(obj);
		return;
	}

	args.GetReturnValue().Set(Null(args.GetIsolate()));
}

// description: read file
// syntax:		fileObj.read() : string
void NaFile::method_read(V8_FUNCTION_ARGS)
{
	Isolate *isolate = args.GetIsolate();
	Local<Object> obj = args.This();
	NaFile *pFile = reinterpret_cast<NaFile*>(UnwrapObject(obj));
	if (pFile == nullptr)
	{
		// error
		args.GetReturnValue().Set(Integer::New(isolate, -1));
		return;
	}

	if (pFile->m_hFile == nullptr)
	{
		args.GetReturnValue().Set(Null(isolate));
		return;
	}

	fseek(pFile->m_hFile, 0, SEEK_END);
	int nSize = ftell(pFile->m_hFile);
	rewind(pFile->m_hFile);

	char *buf = new char[nSize + 1];
	fread(buf, nSize, 1, pFile->m_hFile);
	buf[nSize] = '\0';

	// return
	args.GetReturnValue().Set(
		String::NewFromUtf8(isolate, buf, NewStringType::kNormal).ToLocalChecked()
	);

	delete[] buf;
}

// description: write file
// syntax:		fileObj.write(text);
void NaFile::method_write(V8_FUNCTION_ARGS)
{
	Isolate *isolate = args.GetIsolate();
	Local<Object> obj = args.This();
	NaFile *pFile = reinterpret_cast<NaFile*>(UnwrapObject(obj));
	if (pFile == nullptr || args.Length() < 1)
	{
		// error
		args.GetReturnValue().Set(Integer::New(isolate, -1));
		return;
	}

	if (pFile->m_hFile == nullptr)
	{
		args.GetReturnValue().Set(Null(isolate));
		return;
	}

	String::Value strV8(args[0]);
	NaString str((wchar_t*)*strV8);
	size_t ret = fwrite(str.cstr(), str.GetLength(), 1, pFile->m_hFile);

	// return
	args.GetReturnValue().Set(Integer::New(isolate, ret));
}

// description: close file
// syntax:		fileObj.close()
void NaFile::method_close(V8_FUNCTION_ARGS)
{
	Isolate *isolate = args.GetIsolate();
	Local<Object> obj = args.This();
	NaFile *pFile = reinterpret_cast<NaFile*>(UnwrapObject(obj));
	if (pFile == nullptr)
	{
		// error
		args.GetReturnValue().Set(Integer::New(isolate, -1));
		return;
	}

	if (pFile->m_hFile != nullptr)
	{
		fclose(pFile->m_hFile);
		pFile->m_hFile = nullptr;
	}

	// return
	args.GetReturnValue().Set(Boolean::New(isolate, true));
}
