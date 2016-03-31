#include "stdafx.h"

#include "Common.h"
#include "BasicModule.h"

#include "Windows.h"
#include "NaWindow.h"

#include <iostream>

void NaBasicModule::Create(v8::Isolate * isolate, v8::Local<v8::ObjectTemplate>& global_template)
{
#define ADD_GLOBAL_API(_js_func, _c_func) \
	global_template->Set( \
		v8::String::NewFromUtf8(isolate, #_js_func, v8::NewStringType::kNormal).ToLocalChecked(), \
		v8::FunctionTemplate::New(isolate, _c_func) \
	);

	ADD_GLOBAL_API(sleep, Sleep);
	ADD_GLOBAL_API(alert, Alert);
	ADD_GLOBAL_API(print, Print);
	ADD_GLOBAL_API(trace, Print);
	ADD_GLOBAL_API(exit, Exit);
	ADD_GLOBAL_API(findWindows, FindWindows);
	ADD_GLOBAL_API(findProcesses, FindProcesses);
	ADD_GLOBAL_API(findTrays, FindTrays);
}

void NaBasicModule::Init(v8::Isolate * isolate, v8::Local<v8::ObjectTemplate>& global_template)
{
	// add global object
	v8::Local<v8::Object> global = isolate->GetCurrentContext()->Global();
	{
		v8::Local<v8::String> consolewindow_name = v8::String::NewFromUtf8(isolate, "consolewindow", v8::NewStringType::kNormal).ToLocalChecked();
		v8::Local<v8::Value> consolewindow_value = global->Get(consolewindow_name);
		if (!consolewindow_value.IsEmpty() && consolewindow_value->IsUndefined())
		{
			// InitConsoleWindow

			// create empty handle window
			v8::Local<v8::Object> consolewindow_object = NaWindow::ConvertHWNDToV8WindowObject(isolate, NULL);
			consolewindow_value = consolewindow_object;
			
			consolewindow_object->Set(
				v8::String::NewFromUtf8(isolate, "_type", v8::NewStringType::kNormal).ToLocalChecked(),
				v8::Number::New(isolate, (double)(long)NaWindow::NA_WINDOW_CONSOLE)
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
void Print(V8_FUNCTION_ARGS)
{
	bool first = true;
	for (int i = 0; i < args.Length(); i++)
	{
		v8::HandleScope handle_scope(args.GetIsolate());
		if (first) {
			first = false;
		}
		else {
			printf(" ");
		}

		v8::String::Value str(args[i]);
		wprintf(_T("%s"), (const wchar_t*)*str);
	}
	printf("\n");
	fflush(stdout);
}

// description: show MessageBox with message
// syntax:		alert(message, title, type)
void Alert(V8_FUNCTION_ARGS)
{
	v8::String::Value msg(args[0]);
	v8::String::Value title(args[1]);
	int nType = args[2]->Int32Value();

	::MessageBoxW(NULL,
		(const wchar_t*)*msg,
		args.Length() >= 2 ? (const wchar_t*)*title : L"Alert",
		args.Length() >= 3 ? nType : MB_OK
		);
}

void Sleep(V8_FUNCTION_ARGS)
{
	int nTime = args[0]->Int32Value();

	::Sleep(args.Length() > 0 ? nTime : 1);
}

// Reads a file into a v8 string.
v8::Local<v8::String> ReadFile(v8::Isolate *isolate, const char* name)
{
	FILE* file = fopen(name, "rb");
	if (file == NULL) 
		return v8::Local<v8::String>();

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
	v8::Local<v8::String> result = v8::String::NewFromUtf8(isolate, chars, v8::NewStringType::kNormal/*, size*/).ToLocalChecked();
	delete[] chars;
	return result;
}

void Exit(V8_FUNCTION_ARGS)
{
	g_bExit = true;
}

void FindWindows(V8_FUNCTION_ARGS)
{
	v8::Isolate *isolate = args.GetIsolate();

	v8::String::Value str(args[0]);
	v8::Local<v8::Array> results = v8::Array::New(isolate);

	NaWindow::FindWindows(isolate, (const wchar_t*)*str, results);
	args.GetReturnValue().Set(results);
}

void FindProcesses(V8_FUNCTION_ARGS)
{
	// Not Impl
	v8::Isolate *isolate = args.GetIsolate();
	v8::Local<v8::String> result = v8::String::NewFromUtf8(isolate, "NotImpl", v8::NewStringType::kNormal, 7).ToLocalChecked();
	args.GetReturnValue().Set(result);
}

void FindTrays(V8_FUNCTION_ARGS)
{
	// Not Impl
	v8::Isolate *isolate = args.GetIsolate();
	v8::Local<v8::String> result = v8::String::NewFromUtf8(isolate, "NotImpl", v8::NewStringType::kNormal, 7).ToLocalChecked();
	args.GetReturnValue().Set(result);
}