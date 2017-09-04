#include "stdafx.h"
#include "V8Wrap.h"

#include <stdio.h>
#include <stdarg.h>
#include <vector>
#include <algorithm>

#include <Windows.h>

#include <NaLib/NaDebug.h>
#include <NaLib/NaString.h>

#include "NaMacro.h"

#include "BasicModule.h"
#include "ExtModule.h"
#include "MouseModule.h"
#include "KeyboardModule.h"
#include "ScreenModule.h"

using namespace std;

vector<ModuleBase*> g_ModuleList;

namespace V8Wrap 
{
	bool g_bReportExceptions = true;

	Platform* Initialize()
	{
		V8::InitializeICU();
		V8::InitializeExternalStartupData(__argv[0]);
		Platform* p = platform::CreateDefaultPlatform();
		V8::InitializePlatform(p);
		V8::Initialize();
		return p;
	}

	class ArrayBufferAllocator : public ArrayBuffer::Allocator
	{
	public:
		virtual void* Allocate(size_t length)
		{
			void* data = AllocateUninitialized(length);
			return data == NULL ? data : memset(data, 0, length);
		}
		virtual void* AllocateUninitialized(size_t length) { return malloc(length); }
		virtual void Free(void* data, size_t) { free(data); }
	};

	Isolate* CreateNewIsolate()
	{
		static Isolate::CreateParams create_params;
		static ArrayBufferAllocator allocator;
		create_params.array_buffer_allocator = &allocator;
		return Isolate::New(create_params);
	}

	void TearDown(Platform* p)
	{
		// 16.04.26 check; GlobalHandles broken
		//isolate->Dispose();

		V8::Dispose();
		V8::ShutdownPlatform();
		delete p;
	}

	void CreateDefaultModules(Isolate* isolate, Local<ObjectTemplate>& global_template)
	{
		// Create Default Modules (Register FunctionTemplate)

		ModuleBase *pModule;
		#define CREATE_MODULE(_class) \
			pModule = new _class; \
			pModule->Create(isolate, global_template); \
			g_ModuleList.push_back(pModule);

		CREATE_MODULE(NaBasicModule);
		CREATE_MODULE(NaExtModule);
		CREATE_MODULE(NaMouseModule);
		CREATE_MODULE(NaKeyboardModule);
		CREATE_MODULE(NaScreenModule);
	}

	void InitModules(Isolate* isolate, Local<ObjectTemplate>& global_template)
	{
		for_each(g_ModuleList.begin(), g_ModuleList.end(),
				 [&](ModuleBase* m) { m->Init(isolate, global_template); } );
	}

	void ReleaseModules()
	{
		for_each(g_ModuleList.begin(), g_ModuleList.end(),
				 [&](ModuleBase* m) { m->Release(); delete m; });

		g_ModuleList.clear();
	}

	// Reads a file into a v8 string.
	Local<String> ReadFile(Isolate* isolate, const char* name)
	{
		FILE* file = fopen(name, "rb");
		if (file == NULL)
			return Local<String>();

		fseek(file, 0, SEEK_END);
		int size = ftell(file);
		rewind(file);

		char* chars = new char[size + 1];
		chars[size] = '\0';
		for (int i = 0; i < size;)
		{
			int read = static_cast<int>(fread(&chars[i], 1, size - i, file));
			i += read;
		}

		fclose(file);
		Local<String> result = String::NewFromUtf8(isolate, chars, NewStringType::kNormal/*, size*/).ToLocalChecked();
		delete[] chars;
		return result;
	}

	Local<String> ReadScript(Isolate* isolate, const std::string& scriptFilePath)
	{
		Local<String> script = V8Wrap::ReadFile(isolate, scriptFilePath.c_str());
		if (script.IsEmpty())
		{
			NaString str;
			str.Format("Error reading:\n%s", scriptFilePath.c_str());

			NaDebug::Out(L"==========================================\n");
			NaDebug::Out(str);
			NaDebug::Out(L"\n==========================================\n");
			::MessageBox(nullptr, str.wstr(), nullptr, MB_OK);
		}

		return script;
	}

	Local<Script> Compile(Isolate* isolate, Local<Context>& context,
		const std::string& scriptPath,
		Local<String>& scriptSource)
	{
		TryCatch try_catch(isolate);

		MaybeLocal<String> script_name
			= String::NewFromUtf8(isolate, scriptPath.c_str(), NewStringType::kNormal);
		ScriptOrigin origin(script_name.ToLocalChecked());
		Local<Script> script;
		Script::Compile(context, scriptSource, &origin).ToLocal(&script);
		if (script.IsEmpty())
		{
			// Print errors that happened during compilation.
			if (V8Wrap::g_bReportExceptions)
			{
				NaString str, strE;
				strE = V8Wrap::ReportException(isolate, &try_catch);
				str.Format(L"Script Compile Error:\n%s", strE.wstr());

				NaDebug::Out(L"==========================================\n");
				NaDebug::Out(str);
				NaDebug::Out(L"==========================================\n");
				::MessageBox(nullptr, str.wstr(), nullptr, MB_OK);
			}
		}

		return script;
	}

	bool RunScript(Isolate* isolate, Local<Context>& context, Local<Script>& script)
	{
		TryCatch try_catch(isolate);

		script->Run(context);
		if (try_catch.HasCaught())
		{
			if (V8Wrap::g_bReportExceptions)
			{
				NaString str, strE;
				strE = V8Wrap::ReportException(isolate, &try_catch);
				str.Format(L"Script Runtime Error:\n%s", strE.wstr());

				NaDebug::Out(L"==========================================\n");
				NaDebug::Out(str);
				NaDebug::Out(L"==========================================\n");
				::MessageBox(nullptr, str.wstr(), nullptr, MB_OK);
			}
			return false;
		}

		return true;
	}

	bool RunMainFunc(Isolate* isolate)
	{
		// Find main()
		Local<Object> global = isolate->GetCurrentContext()->Global();
		Local<String> main_name = String::NewFromUtf8(isolate, "main",
										NewStringType::kNormal).ToLocalChecked();
		Local<Value> main_value = global->Get(main_name);
		if (main_value.IsEmpty() || main_value->IsUndefined())
		{
			NaString str = L"Cannot find main function!";

			NaDebug::Out(L"==========================================\n");
			NaDebug::Out(str);
			NaDebug::Out(L"==========================================\n");
			::MessageBox(nullptr, str.wstr(), nullptr, MB_OK);
			return false;
		}

		// Run main function
		Local<Function> main_fn = Local<Function>::Cast(main_value);
		{
			TryCatch try_catch(isolate);

			// main arguments
			const int js_argc = 1;
			Local<Value> js_argv[js_argc] = { String::NewFromUtf8(isolate, "hello :)", String::kNormalString) };
			main_fn->Call(isolate->GetCurrentContext()->Global(), js_argc, js_argv);

			if (try_catch.HasCaught())
			{
				if (V8Wrap::g_bReportExceptions)
				{
					NaString str, strE;
					strE = V8Wrap::ReportException(isolate, &try_catch);
					str.Format(L"Script Exception Occurred:\n%s", strE.wstr());

					NaDebug::Out(L"==========================================\n");
					NaDebug::Out(str);
					NaDebug::Out(L"==========================================\n");
					::MessageBox(nullptr, str.wstr(), nullptr, MB_OK);
				}
				return false;
			}
		}

		return true;
	}

	Local<Object> GetSystemObject(Isolate* isolate)
	{
		// call inside of HandleScope

		Local<Object> global = isolate->GetCurrentContext()->Global();
		Local<String> system_name = String::NewFromUtf8(isolate, "system", NewStringType::kNormal).ToLocalChecked();
		Local<Value> system_value = global->Get(system_name);
		if (!system_value.IsEmpty() && system_value->IsUndefined())
		{
			// InitSystem
			system_value = Object::New(isolate);
			global->Set(system_name, system_value);
		}

		Local<Object> system_obj = system_value->ToObject();
		return system_obj;
	}

	NaString ReportException(Isolate* isolate, TryCatch* try_catch)
	{
		NaString str = L"";

		HandleScope handle_scope(isolate);
		String::Utf8Value exception(try_catch->Exception());
		const char* exception_string = *exception;
		Local<Message> message = try_catch->Message();
		if (message.IsEmpty())
		{
			// V8 didn't provide any extra information about this error; just
			// print the exception.
			str.Format("%s\n", exception_string);
		}
		else
		{
			// Print (filename):(line number): (message).
			String::Utf8Value filename(message->GetScriptResourceName());
			const char* filename_string = *filename;
			int linenum = message->GetLineNumber();
			str.AppendFormat("%s:%i: %s\n", filename_string, linenum, exception_string);

			// Print line of source code.
			String::Utf8Value sourceline(message->GetSourceLine());
			const char* sourceline_string = *sourceline;
			str.AppendFormat("%s\n", sourceline_string);

			// Print wavy underline (GetUnderline is deprecated).
			int start = message->GetStartColumn();
			for (int i = 0; i < start; i++)
				str += L" ";

			int end = message->GetEndColumn();
			for (int i = start; i < end; i++)
				str += L"^";

			str += L"\n";
		}

		return str;
	}

	Local<Value> GetObjectProperty(Local<Object>& obj, NaString strPropName)
	{
		Isolate *isolate = obj->GetIsolate();
		Local<String> _prop_name = String::NewFromUtf8(isolate, strPropName.cstr(), NewStringType::kNormal).ToLocalChecked();
		Local<Value> _prop_value = obj->Get(_prop_name);

		return _prop_value;
		
	}
	
	void SetReturnValueAsNull(ReturnValue<Value> returnValue)
	{
		returnValue.Set(Null(returnValue.GetIsolate()));
	}

	void SetReturnValueAsBoolean(ReturnValue<Value> returnValue, bool value)
	{
		returnValue.Set(
			Boolean::New(
				returnValue.GetIsolate(),
				value)
		);
	}

	void SetReturnValueAsInteger(ReturnValue<Value> returnValue, const int value)
	{
		returnValue.Set(
			Integer::New(
				returnValue.GetIsolate(),
				value)
		);
	}

	void SetReturnValueAsString(ReturnValue<Value> returnValue, const char * sz)
	{
		returnValue.Set(
			String::NewFromUtf8(
				returnValue.GetIsolate(),
				sz,
				NewStringType::kNormal
			).ToLocalChecked()
		);
	}

	void SetReturnValueAsString(ReturnValue<Value> returnValue, const std::wstring& txt)
	{
		returnValue.Set(
			String::NewFromTwoByte(
				returnValue.GetIsolate(),
				(const uint16_t*)(&txt[0]),
				NewStringType::kNormal
			).ToLocalChecked()
		);
	}

	void SetReturnValueAsObject(ReturnValue<Value> returnValue, const Local<Object>& obj)
	{
		returnValue.Set(obj);
	}
}
