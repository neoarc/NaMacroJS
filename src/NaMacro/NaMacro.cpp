// NaMacro.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "NaMacroCommon.h"
#include "V8Wrap.h"
#include <NaLib/NaUrl.h>

using namespace v8;

int __stdcall WinMain(HINSTANCE /*hInstance*/, HINSTANCE /*hPrevInstance*/, char*, int /*nShowCmd*/)
{
	Platform* platform = V8Wrap::Initialize();

	v8::V8::SetFlagsFromCommandLine(&__argc, __argv, true);

	{
		Isolate* isolate = V8Wrap::CreateNewIsolate();
		Isolate::Scope _is(isolate);
		HandleScope _hs(isolate);
	
		// this is little weird.
		// #TODO remove default script
		// #TODO change to usage
		std::string defaultScriptPath = std::string(NaUrl::GetMyDocumentDirectory().cstr());
		defaultScriptPath += "\\NaMacroJS\\NaMacro.njs";

		// #TODO argument parser
		std::string scriptPath;
		Local<String> scriptSource;
		for (int i=1; i<__argc; i++)
		{
			if (__argv[i] && __argv[i][0] == L'-')
				continue;

			scriptPath = (__argc > i) ? __argv[i] : defaultScriptPath;
			scriptSource = V8Wrap::ReadScript(isolate, scriptPath);
			if (scriptSource.IsEmpty())
				return 1;
		}

		// Create a template for the global object.
		Local<ObjectTemplate> global_template = ObjectTemplate::New(isolate);
		V8Wrap::CreateDefaultModules(isolate, global_template);

		// Enter the newly created execution environment.
		Local<Context> context = Context::New(isolate, NULL, global_template);
		Context::Scope context_scope(context);

		V8Wrap::InitModules(isolate, global_template);

		Local<Script> script = V8Wrap::Compile(isolate, context, scriptPath, scriptSource);
		if (script.IsEmpty())
			return 1;
		if (!V8Wrap::RunScript(isolate, context, script))
			return 1;
		if (!V8Wrap::RunMainFunc(isolate))
			return 1;

		NaMacroCommon::MessageLoopTillExit(isolate);

		isolate->IdleNotification(1000);
		V8Wrap::ReleaseModules();
	}

	V8Wrap::TearDown(platform);

	return 0;
}

