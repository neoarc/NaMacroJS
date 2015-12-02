// SimpleJS.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <v8.h>

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "Common.h"
#include "BasicAPI.h"
#include "NaMacro.h"

// Prototype
void ReportException(v8::TryCatch* handler);
bool report_exceptions = true;

// Main
int _tmain(int argc, _TCHAR* argv[])
{
	v8::HandleScope handle_scope;

	v8::Handle<v8::String> script_source(NULL);
	v8::Handle<v8::Value> script_name(NULL);

	const char *str = "NaMacro.js";
	script_source = ReadFile(str);
	script_name = v8::String::New(str);
	if (script_source.IsEmpty()) {
		printf("Error reading '%s'\n", str);
		return 1;
	}
	
	v8::Handle<v8::ObjectTemplate> global_template = v8::ObjectTemplate::New();
	global_template->Set(v8::String::New("print"), v8::FunctionTemplate::New(Print));
	global_template->Set(v8::String::New("exit"), v8::FunctionTemplate::New(Exit));

	v8::Handle<v8::Context> context = v8::Context::New(NULL, global_template);

	// Enter the newly created execution environment.
	v8::Context::Scope context_scope(context);
	v8::Handle<v8::Script> script;
	{
		// Compile script in try/catch context.
		v8::TryCatch try_catch;
		script = v8::Script::Compile(script_source, script_name);
		if (script.IsEmpty()) {
			// Print errors that happened during compilation.
			//if (report_exceptions)
			//	ReportException(&try_catch);
			return 1;
		}
	}

	{
		v8::TryCatch try_catch;

		script->Run();
		if (try_catch.HasCaught()) {
			if (report_exceptions)     
				ReportException(&try_catch);
			return 1;
		}
	}

	// Find main()
	v8::Handle<v8::Object> global = v8::Context::GetCurrent()->Global();
	v8::Local<v8::String> main_name = v8::String::New("main");
	
	// TODO main 함수가 없는지 체크하는 방법 잘못됨 
// 	if (!global->Get(main_name) || global->Get(main_name)->IsNull())
// 	{
// 		printf("Cannot find main function\n");
// 		return 0;
// 	}

	v8::Local<v8::Function> main_fn = v8::Local<v8::Function>::Cast(global->Get(main_name));

	if (main_fn.IsEmpty())
	{
		// error
		printf("Cannot find main function\n");
		return 0;
	}

	const int argc2 = 1;
	v8::Handle<v8::Value> argv2[argc2] = { v8::String::New("test") };
	main_fn->Call(v8::Context::GetCurrent()->Global(), argc2, argv2);

	while (!g_bExit)
	{

	}

	return 0;
}




void ReportException(v8::TryCatch* try_catch) {
	v8::HandleScope handle_scope;
	v8::String::Utf8Value exception(try_catch->Exception());
	const char* exception_string = ToCString(exception);
	v8::Handle<v8::Message> message = try_catch->Message();
	if (message.IsEmpty()) {
		// V8 didn't provide any extra information about this error; just
		// print the exception.
		printf("%s\n", exception_string);
	} else {
		// Print (filename):(line number): (message).
		v8::String::Utf8Value filename(message->GetScriptResourceName());
		const char* filename_string = ToCString(filename);
		int linenum = message->GetLineNumber();
		printf("%s:%i: %s\n", filename_string, linenum, exception_string);
		// Print line of source code.
		v8::String::Utf8Value sourceline(message->GetSourceLine());
		const char* sourceline_string = ToCString(sourceline);
		printf("%s\n", sourceline_string);
		// Print wavy underline (GetUnderline is deprecated).
		int start = message->GetStartColumn();
		for (int i = 0; i < start; i++) {
			printf(" ");
		}
		int end = message->GetEndColumn();
		for (int i = start; i < end; i++) {
			printf("^");
		}
		printf("\n");
	}
}