// NaMacro.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

//#include <v8.h>
#include "libplatform/libplatform.h"
#include "v8.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "Common.h"

#include "BasicModule.h"
#include "MouseModule.h"
#include "KeyboardModule.h"
#include "ExtModule.h"
#include "ScreenModule.h"

#include "NaMacro.h"

using namespace v8;

// Main
int __stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, char*, int nShowCmd)
{
	// Initialize V8
	V8::InitializeICU();
	V8::InitializeExternalStartupData(__argv[0]);
	Platform* platform = platform::CreateDefaultPlatform();
	V8::InitializePlatform(platform);
	V8::Initialize();

	// 16.03.31 disabled
	//SetFlagsFromCommandLine(&argc, argv, true);

	// Create a new Isolate and make it the current one.
	Isolate::CreateParams create_params;
	ArrayBufferAllocator allocator;
	create_params.array_buffer_allocator = &allocator;
	Isolate* isolate = Isolate::New(create_params);

	// Creates a new execution environment containing the built-in functions.
	{
		Isolate::Scope isolate_scope(isolate);
		HandleScope handle_scope(isolate);
	
		// Create a template for the global object.
		Local<ObjectTemplate> global_template = ObjectTemplate::New(isolate);

		// Load Default js
		Local<String> script_source;
		MaybeLocal<String> script_name;

		char *scriptfile = "NaMacro.js";
		if (__argc > 1)
			scriptfile = __argv[1];

		script_source = ReadFile(isolate, scriptfile);
		script_name = String::NewFromUtf8(isolate, scriptfile, NewStringType::kNormal);
		if (script_source.IsEmpty()) 
		{
			NaString str;
			str.Format("Error reading:\n%s", scriptfile);

			NaDebugOut(L"==========================================\n");
			NaDebugOut(str);
			NaDebugOut(L"\n==========================================\n");
			::MessageBox(nullptr, str.wstr(), nullptr, MB_OK);
			return 1;
		}

		// Create Default Modules (Register FunctionTemplate)
		CreateDefaultModules(isolate, global_template);

		// Enter the newly created execution environment.
		Local<Context> context = Context::New(isolate, NULL, global_template);
		Context::Scope context_scope(context);

		// Initialize Modules (Access global context)
		InitModules(isolate, global_template);

		Local<Script> script;
		{
			// Compile script in try/catch context.
			TryCatch try_catch(isolate);

			ScriptOrigin origin(script_name.ToLocalChecked());
			Script::Compile(context, script_source, &origin).ToLocal(&script);
			if (script.IsEmpty())
			{
				// Print errors that happened during compilation.
				if (g_bReportExceptions)
				{
					NaString str, strE;
					strE = ReportException(isolate, &try_catch);
					str.Format(L"Script Compile Error:\n%s", strE.wstr());

					NaDebugOut(L"==========================================\n");
					NaDebugOut(str);
					NaDebugOut(L"==========================================\n");
					::MessageBox(nullptr, str.wstr(), nullptr, MB_OK);
				}
				return 1;
			}
		}

		{
			TryCatch try_catch(isolate);

			script->Run(context);
			if (try_catch.HasCaught())
			{
				if (g_bReportExceptions)
				{
					NaString str, strE;
					strE = ReportException(isolate, &try_catch);
					str.Format(L"Script Runtime Error:\n%s", strE.wstr());

					NaDebugOut(L"==========================================\n");
					NaDebugOut(str);
					NaDebugOut(L"==========================================\n");
					::MessageBox(nullptr, str.wstr(), nullptr, MB_OK);
				}
				return 1;
			}
		}

		// Find main()
		Local<Object> global = isolate->GetCurrentContext()->Global();
		Local<String> main_name = String::NewFromUtf8(isolate, "main", NewStringType::kNormal).ToLocalChecked();
		Local<Value> main_value = global->Get(main_name);
		if (main_value.IsEmpty() || main_value->IsUndefined())
		{
			NaString str = L"Cannot find main function!";

			NaDebugOut(L"==========================================\n");
			NaDebugOut(str);
			NaDebugOut(L"==========================================\n");
			::MessageBox(nullptr, str.wstr(), nullptr, MB_OK);
			return 0;
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
				if (g_bReportExceptions)
				{
					NaString str, strE;
					strE = ReportException(isolate, &try_catch);
					str.Format(L"Script Exception Occurred:\n%s", strE.wstr());

					NaDebugOut(L"==========================================\n");
					NaDebugOut(str);
					NaDebugOut(L"==========================================\n");
					::MessageBox(nullptr, str.wstr(), nullptr, MB_OK);
				}
				return 1;
			}
		}

		// MessageLoop after main()
		MSG msg = { 0 };
		while (!g_bExit)
		{
			bool bRet;
			bRet = GetMessage(&msg, NULL, 0, 0);
			if (msg.message == 0)
				continue;
			if (bRet == 0)
				break;
			switch (msg.message)
			{
			case WM_HOTKEY:
				NaKeyboardModule::OnHotkey(isolate, msg.wParam, msg.lParam);
				break;
			case WM_TIMER:
				NaBasicModule::OnTimer(isolate, (int)msg.wParam);
				break;
			}

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		// Collect Garbage
		isolate->IdleNotification(1000);

		// Release Modules
		ReleaseModules();
	}

	// Dispose the isolate and tear down V8.

	// 16.04.26 check; GlobalHandles broken
	//isolate->Dispose();

	V8::Dispose();
	V8::ShutdownPlatform();
	delete platform;

	return 0;
}

void CreateDefaultModules(Isolate * isolate, Local<ObjectTemplate>& global_template)
{
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

void InitModules(Isolate *isolate, Local<ObjectTemplate> &global_template)
{
	vector<ModuleBase*>::iterator it = g_ModuleList.begin();
	for (; it != g_ModuleList.end(); ++it)
	{
		ModuleBase *pModule = *it;
		pModule->Init(isolate, global_template);
	}
}

void ReleaseModules()
{
	vector<ModuleBase*>::iterator it = g_ModuleList.begin();
	for (; it != g_ModuleList.end(); ++it)
	{
		ModuleBase *pModule = *it;
		pModule->Release();
		delete pModule;
	}

	g_ModuleList.clear();
}
