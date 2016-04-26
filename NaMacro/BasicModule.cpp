#include "stdafx.h"

#include "Common.h"
#include "BasicModule.h"

#include "Windows.h"
#include "NaWindow.h"

#include <iostream>

void NaBasicModule::Create(Isolate * isolate, Local<ObjectTemplate>& global_template)
{
#define ADD_GLOBAL_METHOD(_js_func, _c_func)	ADD_TEMPLATE_METHOD(global_template, _js_func, _c_func);

	// methods
	ADD_GLOBAL_METHOD(sleep,		Sleep);
	ADD_GLOBAL_METHOD(alert,		Alert);
	ADD_GLOBAL_METHOD(print,		Print);
	ADD_GLOBAL_METHOD(trace,		Print);
	ADD_GLOBAL_METHOD(exit,			Exit);
	ADD_GLOBAL_METHOD(getWindow,	GetWindow);
	ADD_GLOBAL_METHOD(findWindows,	FindWindows);
	ADD_GLOBAL_METHOD(findProcesses, FindProcesses);
	ADD_GLOBAL_METHOD(findTrays,	FindTrays);
}

void NaBasicModule::Init(Isolate * isolate, Local<ObjectTemplate>& global_template)
{
	// add global object
	Local<Object> global = isolate->GetCurrentContext()->Global();
	{
		Local<String> consolewindow_name = String::NewFromUtf8(isolate, "consolewindow", NewStringType::kNormal).ToLocalChecked();
		Local<Value> consolewindow_value = global->Get(consolewindow_name);
		if (!consolewindow_value.IsEmpty() && consolewindow_value->IsUndefined())
		{
			// InitConsoleWindow

			// create empty handle window
			//Local<Object> consolewindow_object = NaWindow::ConvertHWNDToV8WindowObject(isolate, NULL);
			NaWindow *pWindow = new NaWindow(0, NA_WINDOW_CONSOLE);
			Local<Object> consolewindow_object = NaWindow::WrapObject(isolate, pWindow);
			consolewindow_value = consolewindow_object;
			
			consolewindow_object->Set(
				String::NewFromUtf8(isolate, "_type", NewStringType::kNormal).ToLocalChecked(),
				Number::New(isolate, (double)(long)NA_WINDOW_CONSOLE)
				);
			global->Set(consolewindow_name, consolewindow_value);
		}
	}
}

void NaBasicModule::Release()
{
	
}

// description: Print message to console
// syxtax:		print(message) 
void NaBasicModule::Print(V8_FUNCTION_ARGS)
{
	bool first = true;
	for (int i = 0; i < args.Length(); i++)
	{
		HandleScope handle_scope(args.GetIsolate());
		if (first) {
			first = false;
		}
		else {
			printf(" ");
		}

		String::Value str(args[i]);
		wprintf(_T("%s"), (const wchar_t*)*str);
	}
	printf("\n");
	fflush(stdout);
}

// description: show MessageBox with message
// syntax:		alert(message, title, type)
void NaBasicModule::Alert(V8_FUNCTION_ARGS)
{
	String::Value msg(args[0]);
	String::Value title(args[1]);
	int nType = args[2]->Int32Value();

	::MessageBoxW(NULL,
		(const wchar_t*)*msg,
		args.Length() >= 2 ? (const wchar_t*)*title : L"Alert",
		args.Length() >= 3 ? nType : MB_OK
		);
}

void NaBasicModule::Sleep(V8_FUNCTION_ARGS)
{
	int nTime = args[0]->Int32Value();

	::Sleep(args.Length() > 0 ? nTime : 1);
}

void NaBasicModule::Exit(V8_FUNCTION_ARGS)
{
	g_bExit = true;
}

void NaBasicModule::GetWindow(V8_FUNCTION_ARGS)
{
	Isolate *isolate = args.GetIsolate();
	int x = args[0]->Int32Value();
	int y = args[1]->Int32Value();

	NaWindow *pWindow = NaWindow::GetWindow(x, y);;
	Local<Object> result = NaWindow::WrapObject(isolate, pWindow);

	args.GetReturnValue().Set(result);
}

void NaBasicModule::FindWindows(V8_FUNCTION_ARGS)
{
	Isolate *isolate = args.GetIsolate();
	String::Value str(args[0]);
	Local<Array> results = Array::New(isolate);

	NaWindow::FindWindows(isolate, (const wchar_t*)*str, results);
	args.GetReturnValue().Set(results);
}

void NaBasicModule::FindProcesses(V8_FUNCTION_ARGS)
{
	// Not Impl
	Isolate *isolate = args.GetIsolate();
	Local<String> result = String::NewFromUtf8(isolate, "NotImpl", NewStringType::kNormal, 7).ToLocalChecked();
	args.GetReturnValue().Set(result);
}

void NaBasicModule::FindTrays(V8_FUNCTION_ARGS)
{
	// Not Impl
	Isolate *isolate = args.GetIsolate();
	Local<String> result = String::NewFromUtf8(isolate, "NotImpl", NewStringType::kNormal, 7).ToLocalChecked();
	args.GetReturnValue().Set(result);
}