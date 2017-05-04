// NaMacro.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include <v8.h>
#include <libplatform.h>

#include "v8cmn.h"

using namespace v8;

int __stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, char*, int nShowCmd)
{
	Platform* platform = v8cmn::Initialize();
	//SetFlagsFromCommandLine(&argc, argv, true);
	{
		Isolate* isolate = v8cmn::CreateNewIsolate();
		Isolate::Scope _is(isolate);
		HandleScope _hs(isolate);
	
		std::string scriptPath = (__argc > 1) ? __argv[1] : "NaMacro.js";
		Local<String> scriptSource = v8cmn::ReadScript(isolate, scriptPath);
		if (scriptSource.IsEmpty()) return 1;

		// Create a template for the global object.
		Local<ObjectTemplate> global_template = ObjectTemplate::New(isolate);
		v8cmn::CreateDefaultModules(isolate, global_template);

		// Enter the newly created execution environment.
		Local<Context> context = Context::New(isolate, NULL, global_template);
		Context::Scope context_scope(context);

		v8cmn::InitModules(isolate, global_template);

		Local<Script> script = v8cmn::Compile(isolate, context, scriptPath, scriptSource);
		if (script.IsEmpty()) return 1;

		bool ok = true;
		ok = v8cmn::RunScript(isolate, context, script);	if (!ok) return 1;
		ok = v8cmn::RunMainFunc(isolate);					if (!ok) return 1;

		v8cmn::MessageLoopTillExit(isolate);

		isolate->IdleNotification(1000);
		v8cmn::ReleaseModules();
	}

	v8cmn::TearDown(platform);
	return 0;
}

