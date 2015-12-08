// NaMacro.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

//#include <v8.h>
#include "include/libplatform/libplatform.h"
#include "include/v8.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "Common.h"

#include "BasicAPI.h"
#include "MouseAPI.h"
#include "ExtAPI.h"

#include "NaMacro.h"

// Prototype
void ReportException(v8::Isolate *isolate, v8::TryCatch* handler);
bool report_exceptions = true;

using namespace v8;

class ArrayBufferAllocator : public v8::ArrayBuffer::Allocator {
public:
	virtual void* Allocate(size_t length) {
		void* data = AllocateUninitialized(length);
		return data == NULL ? data : memset(data, 0, length);
	}
	virtual void* AllocateUninitialized(size_t length) {
		return malloc(length);
	}
	virtual void Free(void* data, size_t) {
		free(data);
	}
};

// Main
int main(int argc, char* argv[])
{
	// Initialize V8.
	V8::InitializeICU();
	V8::InitializeExternalStartupData(argv[0]);
	Platform* platform = platform::CreateDefaultPlatform();
	V8::InitializePlatform(platform);
	V8::Initialize();
	v8::V8::SetFlagsFromCommandLine(&argc, argv, true);

	// Create a new Isolate and make it the current one.
	Isolate::CreateParams create_params;
	ArrayBufferAllocator allocator;
	create_params.array_buffer_allocator = &allocator;
	Isolate* isolate = Isolate::New(create_params);

	// Creates a new execution environment containing the built-in functions.
	{
		v8::Isolate::Scope isolate_scope(isolate);
		v8::HandleScope handle_scope(isolate);
	
		// Create a template for the global object.
		v8::Local<v8::ObjectTemplate> global_template = v8::ObjectTemplate::New(isolate);

		// Load Default js
		v8::Local<v8::String> script_source;
		v8::MaybeLocal<v8::String> script_name;

		const char *str = "NaMacro.js";
		script_source = ReadFile(isolate, str);
		script_name = v8::String::NewFromUtf8(isolate, str, v8::NewStringType::kNormal);
		if (script_source.IsEmpty()) {
			printf("Error reading '%s'\n", str);
			return 1;
		}

		// TODO modulize
		{
			// init Modules that doesn't need global context
			InitBasicAPI(isolate, global_template);
			InitExtAPI(isolate, global_template);
		}

		// Enter the newly created execution environment.
		v8::Local<v8::Context> context = v8::Context::New(isolate, NULL, global_template);
		v8::Context::Scope context_scope(context);

		// TODO modulize
		{
			// init Modules that needs global context
			InitMouseAPI(isolate, global_template);
		}

		v8::Local<v8::Script> script;
		{
			// Compile script in try/catch context.
			v8::TryCatch try_catch(isolate);

			v8::ScriptOrigin origin(script_source);
			v8::Script::Compile(context, script_source, &origin).ToLocal(&script);
			if (script.IsEmpty())
			{
				// Print errors that happened during compilation.
				//if (report_exceptions)
				//	ReportException(isolate, &try_catch);
				return 1;
			}
		}

		{
			v8::TryCatch try_catch(isolate);

			script->Run(context);
			if (try_catch.HasCaught())
			{
				if (report_exceptions)
					ReportException(isolate, &try_catch);
				return 1;
			}
		}

		// Find main()
		v8::Local<v8::Object> global = isolate->GetCurrentContext()->Global();
		v8::Local<v8::String> main_name = v8::String::NewFromUtf8(isolate, "main", v8::NewStringType::kNormal).ToLocalChecked();
		v8::Local<v8::Value> main_value = global->Get(main_name);
		if (main_value.IsEmpty() || main_value->IsUndefined())
		{
			// error
			printf("Cannot find main function\n");
			return 0;
		}

		if (!main_value->IsFunction())
		{
			// error
			printf("main is not function!\n");
			return 0;
		}

		v8::Local<v8::Function> main_fn = v8::Local<v8::Function>::Cast(global->Get(main_name));

		// main arguments
		const int js_argc = 1;
		v8::Local<v8::Value> js_argv[js_argc] = { v8::String::NewFromUtf8(isolate, "hello :)", String::kNormalString) };
		main_fn->Call(isolate->GetCurrentContext()->Global(), js_argc, js_argv);

		// infinite loop 
		while (!g_bExit) { }
	}

	// Dispose the isolate and tear down V8.
	isolate->Dispose();
	v8::V8::Dispose();
	v8::V8::ShutdownPlatform();
	delete platform;

	return 0;
}

void ReportException(v8::Isolate *isolate, v8::TryCatch* try_catch) 
{
	v8::HandleScope handle_scope(isolate);
	v8::String::Utf8Value exception(try_catch->Exception());
	const char* exception_string = ToCString(exception);
	v8::Local<v8::Message> message = try_catch->Message();
	if (message.IsEmpty()) 
	{
		// V8 didn't provide any extra information about this error; just
		// print the exception.
		printf("%s\n", exception_string);
	}
	else 
	{
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
		for (int i = 0; i < start; i++) 
		{
			printf(" ");
		}

		int end = message->GetEndColumn();
		for (int i = start; i < end; i++) 
		{
			printf("^");
		}
		printf("\n");
	}
}