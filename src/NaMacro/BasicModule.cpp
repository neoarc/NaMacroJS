#include "stdafx.h"
#include "BasicModule.h"

#include <iostream>
#include <string>
#include <functional> 

#include <Windows.h>

#include <NaLib/NaDebug.h>
#include <NaLib/NaMessageBox.h>
#include <NaLib/NaNotifyWindow.h>
#include <NaLib/NaCurl.h>
#include <NaLib/NaImage.h>

#include "NaMacroCommon.h"
#include "V8Wrap.h"

#include "NaWindow.h"
#include "NaControl.h"

#include "JsFile.h"
#include "JsWindow.h"
#include "JsImage.h"
#include "JSProcess.h"

NaWindow* NaBasicModule::s_pTimerWindow = NULL;
std::map<int, Persistent<Function, CopyablePersistentTraits<Function>>> NaBasicModule::s_mapIntervalCallback;
std::map<int, Persistent<Function, CopyablePersistentTraits<Function>>> NaBasicModule::s_mapTimeoutCallback;
int NaBasicModule::s_nTimerID = 1; // begin from 0 causes hard to store variable

void NaBasicModule::Create(Isolate * isolate, Local<ObjectTemplate>& global_template)
{
	// Global methods
	ADD_GLOBAL_METHOD(include);
	ADD_GLOBAL_METHOD(sleep);
	ADD_GLOBAL_METHOD(setInterval);
	ADD_GLOBAL_METHOD(clearInterval);
	ADD_GLOBAL_METHOD(setTimeout);
	ADD_GLOBAL_METHOD(alert);
	ADD_GLOBAL_METHOD(confirm);
	ADD_GLOBAL_METHOD(prompt);
	ADD_GLOBAL_METHOD(print);
	ADD_GLOBAL_METHOD2(trace, method_print);
	ADD_GLOBAL_METHOD(notify);
	ADD_GLOBAL_METHOD(fetch);
	ADD_GLOBAL_METHOD(exit);
	ADD_GLOBAL_METHOD(getWindow);
	ADD_GLOBAL_METHOD(getActiveWindow);
	ADD_GLOBAL_METHOD(findWindows);
	ADD_GLOBAL_METHOD(findProcesses);
	ADD_GLOBAL_METHOD(findTrays);
}

void NaBasicModule::Init(Isolate * isolate, Local<ObjectTemplate>& /*global_template*/)
{
	// add global object
	Local<Object> global = isolate->GetCurrentContext()->Global();
	{
		// Init ConsoleWindow
		Local<String> consolewindow_name = String::NewFromUtf8(isolate, "consoleWindow", NewStringType::kNormal).ToLocalChecked();
		Local<Value> consolewindow_value = global->Get(consolewindow_name);
		if (!consolewindow_value.IsEmpty() && consolewindow_value->IsUndefined())
		{
			JsWindow *pJsWindow = new JsWindow();
			pJsWindow->m_pNativeWindow = new NaWindow(0, NA_WINDOW_CONSOLE);
			Local<Object> consolewindow_object = JsWindow::WrapObject(isolate, pJsWindow);
			consolewindow_value = consolewindow_object;

			global->Set(consolewindow_name, consolewindow_value);
		}
	}

	{
		// Init class constructors to global
		// Note: Control class is not allowed to create by constructor
		ADD_GLOBAL_CONSTRUCTOR2(Window, global);
		ADD_GLOBAL_CONSTRUCTOR2(Image, global);
		ADD_GLOBAL_CONSTRUCTOR2(File, global);

		// Note: Another way to add class constructor
		/*
		Local<ObjectTemplate> obj_templ = templ->PrototypeTemplate();
		obj_templ->Set(
			window_name,
			FunctionTemplate::New(isolate, NaWindow::Constructor)->GetFunction()
		);

		global->Set(window_name, window_value);
		*/
	}

	{
		// Create Timer Window
		s_pTimerWindow = new NaWindow(NULL, NA_WINDOW_INTERNAL);
		s_pTimerWindow->Create(JsWindow::WndProc);
	}

	// Must in Init() not in Create()
	// (need HandleScope)
	HandleScope handle_scope(isolate);
	Local<Object> system_obj = V8Wrap::GetSystemObject(isolate);

#define ADD_SYSTEM_ACCESSOR_RO(_prop)	ADD_OBJ_ACCESSOR_RO(system_obj, _prop);
#define ADD_SYSTEM_METHOD(_js_func)		ADD_OBJ_METHOD(system_obj, _js_func);

	// system accessors
	ADD_SYSTEM_ACCESSOR_RO(pcname);
	ADD_SYSTEM_ACCESSOR_RO(username);

	// system methods
	ADD_SYSTEM_METHOD(execute);
	ADD_SYSTEM_METHOD(executeSync);
}

void NaBasicModule::get_pcname(V8_GETTER_ARGS)
{
	UNUSED_PARAMETER(name);

	const auto pcname = NaMacroCommon::GetSystemInfoStringByAPI(GetComputerName);
	if (!pcname.empty())
		V8Wrap::SetReturnValue(info, pcname);
}

void NaBasicModule::get_username(V8_GETTER_ARGS)
{
	UNUSED_PARAMETER(name);

	const auto username = NaMacroCommon::GetSystemInfoStringByAPI(GetUserName);
	if (!username.empty())
		V8Wrap::SetReturnValue(info, username);
}

void NaBasicModule::Release()
{
	// Destroy Console Window

	// Destroy Timer Window
	if (s_pTimerWindow != NULL)
	{
		s_pTimerWindow->Destroy();

		delete s_pTimerWindow;
		s_pTimerWindow = NULL;
	}
}

void NaBasicModule::OnTimer(Isolate *isolate, int nTimerID)
{
	std::map <int, Persistent<Function, CopyablePersistentTraits<Function>>>::iterator it;
	it = NaBasicModule::s_mapIntervalCallback.find(nTimerID);
	if (it != NaBasicModule::s_mapIntervalCallback.end())
	{
		Persistent<Function, CopyablePersistentTraits<Function>> percy = it->second;
		Local<Function> callback = Local<Function>::New(isolate, percy);
		if (!callback.IsEmpty())
		{
			Local<Value> recv = isolate->GetCurrentContext()->Global();
			callback->Call(
				isolate->GetCurrentContext(),
				recv,
				0,
				NULL
			);
		}
	}

	it = NaBasicModule::s_mapTimeoutCallback.find(nTimerID);
	if (it != NaBasicModule::s_mapTimeoutCallback.end())
	{
		Persistent<Function, CopyablePersistentTraits<Function>> percy = it->second;
		Local<Function> callback = Local<Function>::New(isolate, percy);
		if (!callback.IsEmpty())
		{
			Local<Value> recv = isolate->GetCurrentContext()->Global();
			callback->Call(
				isolate->GetCurrentContext(),
				recv,
				0,
				NULL
			);
		}

		::KillTimer(NaBasicModule::s_pTimerWindow->m_hWnd, nTimerID);

		NaBasicModule::s_mapIntervalCallback.erase(nTimerID);
	}
}

bool NaBasicModule::IncludeBase(NaString strBase, NaString strUrl)
{
	return false;
}

// description: Include external source file
// syntax:		include(filename);
void NaBasicModule::method_include(V8_FUNCTION_ARGS)
{
	Isolate *isolate = args.GetIsolate();
	Local<Context> context = isolate->GetCurrentContext();

	for (int i = 0; i < args.Length(); i++)
	{
		Local<String> script_source;
		MaybeLocal<String> script_name;

		String::Value arg_value(args[i]);
		wchar_t *wstr = (wchar_t*)(*arg_value);

		// converting relative path
		Local<StackTrace> stack_trace = StackTrace::CurrentStackTrace(isolate, 1, StackTrace::kScriptName);
		Local<StackFrame> cur_frame = stack_trace->GetFrame(0);

		NaString strBase(*String::Utf8Value(cur_frame->GetScriptName()));
// 		NaString strBase((wchar_t*)(*cur_frame->GetScriptName()));

		//
		// 2016.07.27
		// GetScriptName Unicode Issue:
		// Wrong Conversion if Special character included.
		//
		NaDebugOut(L"================================================\n");
		NaDebugOut(L"Include src: %s\n", wstr);
		NaDebugOut(L"Include base: %s\n", strBase.wstr());

		NaString strUrl(wstr);
		NaUrl url;
		url.SetBase(strBase);
		url.SetUrl(strUrl);

		// #TODO
		//IncludeBase(strBase, strUrl);

		NaString strFullUrl(url.GetFullUrl());
		NaDebugOut(L"Include full: %s\n", strFullUrl.wstr());

		script_source = V8Wrap::ReadFile(isolate, strFullUrl.cstr());
		script_name = String::NewFromUtf8(isolate, strFullUrl.cstr(), NewStringType::kNormal);
		if (script_source.IsEmpty())
		{
			NaDebugOut(L"Error reading '%s'\n", strFullUrl.wstr());

			// TODO ThrowException
			return;
		}

		Local<Script> script;
		{
			TryCatch try_catch(isolate);
			ScriptOrigin origin(script_name.ToLocalChecked());
			Script::Compile(context, script_source, &origin).ToLocal(&script);
			if (script.IsEmpty())
			{
				if (V8Wrap::g_bReportExceptions)
					V8Wrap::ReportException(isolate, &try_catch);

				NaDebugOut(L"included script is empty!\n");
				return;
			}
		}

		{
			TryCatch try_catch(isolate);
			script->Run(context);
			if (try_catch.HasCaught())
			{
				if (V8Wrap::g_bReportExceptions)
					V8Wrap::ReportException(isolate, &try_catch);
				return;
			}
		}
	}
}

// description: Print message to console
// syxtax:		print(message)
void NaBasicModule::method_print(V8_FUNCTION_ARGS)
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

// description: Notify message via notify window
// syntax:		notify(message)
void NaBasicModule::method_notify(V8_FUNCTION_ARGS)
{
	String::Value message(args[0]);
	NaString strMessage((wchar_t*)*message);
	NaString strTitle = L"NaMacro";

	if (args.Length() > 1)
	{
		String::Value title(args[1]);
		strTitle = (wchar_t*)*message;
	}

	NaNotifyWindow::AddNotifyWindow(strMessage, strTitle);
}

// description: Fetch API
// syntax:		fetch(url[, parameter]) : Promise
void NaBasicModule::method_fetch(V8_FUNCTION_ARGS)
{
	auto *isolate = args.GetIsolate();
	isolate; // not used

	String::Value url(args[0]);

	// #TODO argument validation

	// #TODO process parameter (json object)
	NaString strMethod = L"POST";
	NaString strBody = L"";

	if (args.Length() >= 2)
	{
		if (args[1]->IsObject())
		{
			auto objParam = args[1]->ToObject();

			// Method
			auto method_value = V8Wrap::GetObjectProperty(objParam, "method");
			if (!method_value->IsUndefined())
			{
				// POST, GET, PUT, DELETE
				Local<String> method_string = method_value->ToString();
				String::Utf8Value method_string_value(method_string);
				strMethod = (*method_string_value);

				NaDebugOut(L"Method: %ls\n", strMethod.wstr());
			}

			// Body
			auto body_value = V8Wrap::GetObjectProperty(objParam, "body");
			if (!body_value->IsUndefined())
			{
				Local<String> body_string = body_value->ToString();
				String::Utf8Value body_string_value(body_string);
				strBody = (*body_string_value);

				NaDebugOut(L"Body: %ls\n", strBody.wstr());
			}

			// #TODO implement
		}
	}

	NaString strUrl((const wchar_t*)*url);

	// #TODO if url is local url, open file.

	NaCurl curl;
	NaString strRet = L"";
	if (strMethod.CompareNoCase(L"POST") == 0)
		strRet = curl.Post(strUrl, strBody);

	//new Promise();

	// #TODO change return value to Promise object (from string)
	V8Wrap::SetReturnValue(args, strRet.wstr());
}

// description: show MessageBox with message
// syntax:		alert(message, title, type)
void NaBasicModule::method_alert(V8_FUNCTION_ARGS)
{
	String::Value msg(args[0]);
	String::Value title(args[1]);
	int nType = args[2]->Int32Value();

	int nRet = ::MessageBoxW(NULL,
		(const wchar_t*)*msg,
		args.Length() >= 2 ? (const wchar_t*)*title : L"Alert",
		args.Length() >= 3 ? nType : MB_OK
		);

	V8Wrap::SetReturnValue(args, nRet);
}

// description: show MessageBox with message
// syntax:		confirm(message, title)
void NaBasicModule::method_confirm(V8_FUNCTION_ARGS)
{
	String::Value msg(args[0]);
	String::Value title(args[1]);

	int nRet = ::MessageBoxW(NULL,
		(const wchar_t*)*msg,
		args.Length() >= 2 ? (const wchar_t*)*title : L"Confirm",
		MB_OKCANCEL
	);

	bool bRet = false;
	switch (nRet)
	{
	case IDOK:
		bRet = true;
		break;
	case IDCANCEL:
	default:
		break;
	}

	V8Wrap::SetReturnValue(args, bRet);
}

// description: show MessageBox with message
// syntax:		prompt(message, title[, defaultmessage])
void NaBasicModule::method_prompt(V8_FUNCTION_ARGS)
{
	String::Value msg(args[0]);
	String::Value title(args[1]);
	String::Value default_string(args[2]);

	NaMessageBox MsgBox;
	NaString strRet =
		MsgBox.DoModal(NULL,
			(wchar_t*)*msg,
			args.Length() >= 2 ? (const wchar_t*)*title : L"Prompt",
			args.Length() >= 3 ? (const wchar_t*)*default_string : L""
		);

	V8Wrap::SetReturnValue(args, strRet.cstr());
}

// description: suspends the excution script
// syntax:		sleep(time)
// param(time):	time interval in milliseconds
void NaBasicModule::method_sleep(V8_FUNCTION_ARGS)
{
	int nTime = args[0]->Int32Value();

	::Sleep(args.Length() > 0 ? nTime : 1);
}

// description:
// syntax:		setInterval(interval, callback_function) : timer_id
void NaBasicModule::method_setInterval(V8_FUNCTION_ARGS)
{
	if (args.Length() < 2)
		return;

	int nTime = args[0]->Int32Value();
	Local<Object> callback = args[1]->ToObject();
	if (callback->IsFunction())
	{
		int nTimerID = NaBasicModule::s_nTimerID++;
		::SetTimer(s_pTimerWindow->m_hWnd, nTimerID, nTime, NULL);

		Isolate *isolate = args.GetIsolate();
		Local<Function> callback_func = Local<Function>::Cast(args[1]);
		Persistent<Function, CopyablePersistentTraits<Function>> percy(isolate, callback_func);

		NaBasicModule::s_mapIntervalCallback.insert(
			std::pair<int, Persistent<Function, CopyablePersistentTraits<Function>>>(nTimerID, percy)
		);

		V8Wrap::SetReturnValue(args, nTimerID);
	}
}

// description:
// syntax:		clearInterval(timer_id)
void NaBasicModule::method_clearInterval(V8_FUNCTION_ARGS)
{
	if (args.Length() < 1)
		return;

	int nTimerID = args[0]->Int32Value();
	std::map <int, Persistent<Function, CopyablePersistentTraits<Function>>>::iterator it;
	it = NaBasicModule::s_mapIntervalCallback.find(nTimerID);
	if (it != NaBasicModule::s_mapIntervalCallback.end())
	{
		::KillTimer(NaBasicModule::s_pTimerWindow->m_hWnd, nTimerID);

		NaBasicModule::s_mapIntervalCallback.erase(nTimerID);
	}
}

// description:
// syntax:
void NaBasicModule::method_setTimeout(V8_FUNCTION_ARGS)
{
	if (args.Length() < 2)
		return;

	int nTime = args[0]->Int32Value();
	Local<Object> callback = args[1]->ToObject();
	if (callback->IsFunction())
	{
		int nTimerID = NaBasicModule::s_nTimerID++;
		::SetTimer(s_pTimerWindow->m_hWnd, nTimerID, nTime, NULL);

		Isolate *isolate = args.GetIsolate();
		Local<Function> callback_func = Local<Function>::Cast(args[1]);
		Persistent<Function, CopyablePersistentTraits<Function>> percy(isolate, callback_func);

		NaBasicModule::s_mapTimeoutCallback.insert(
			std::pair<int, Persistent<Function, CopyablePersistentTraits<Function>>>(nTimerID, percy)
		);

		V8Wrap::SetReturnValue(args, nTimerID);
	}
}

// description: exit current event loop
// syntax:		exit()
void NaBasicModule::method_exit(V8_FUNCTION_ARGS)
{
	UNUSED_PARAMETER(args);
	NaMacroCommon::g_bExit = true;
}

// description: pick a window from point
// syntax:		getWindow(x, y) : window object
void NaBasicModule::method_getWindow(V8_FUNCTION_ARGS)
{
	Isolate *isolate = args.GetIsolate();
	int x = args[0]->Int32Value();
	int y = args[1]->Int32Value();

	JsWindow *pJsWindow = new JsWindow();
	pJsWindow->m_pNativeWindow = NaWindow::GetWindow(x, y);
	Local<Object> result = JsWindow::WrapObject(isolate, pJsWindow);

	V8Wrap::SetReturnValue(args, result);
}

// description: get active window
// syntax:		getActiveWindow() : window object
void NaBasicModule::method_getActiveWindow(V8_FUNCTION_ARGS)
{
	Isolate *isolate = args.GetIsolate();

	JsWindow *pJsWindow = new JsWindow();
	pJsWindow->m_pNativeWindow = NaWindow::GetActiveWindow();
	Local<Object> result = JsWindow::WrapObject(isolate, pJsWindow);

	V8Wrap::SetReturnValue(args, result);
}

// description: find windows which contains specific text
// syntax:		findWindows(text)
void NaBasicModule::method_findWindows(V8_FUNCTION_ARGS)
{
	Isolate *isolate = args.GetIsolate();
	String::Value str(args[0]);
	Local<Array> results = Array::New(isolate);

	JsWindow::FindWindows(isolate, (const wchar_t*)*str, results);

	V8Wrap::SetReturnValue(args, results);
}

// description: find processes which contains specific name
// syntax:		findProcesses(text)
void NaBasicModule::method_findProcesses(V8_FUNCTION_ARGS)
{
	Isolate *isolate = args.GetIsolate();
	String::Value str(args[0]);
	Local<Array> results = Array::New(isolate);

	JsProcess::FindProcesses(isolate, (const wchar_t*)*str, results);

	V8Wrap::SetReturnValue(args, results);
}

// description:
// syntax:
void NaBasicModule::method_findTrays(V8_FUNCTION_ARGS)
{
	// Not Impl
	V8Wrap::SetReturnValue(args, L"NotImplemented");
}

// description: Execute external process
// syntax:		system.execute(exefile_path[, arguments[, is_show]])
void NaBasicModule::method_execute(V8_FUNCTION_ARGS)
{
	String::Value exepath(args[0]);
	NaString strExePath((wchar_t*)*exepath);
	NaString strArguments = L"";

	// https://msdn.microsoft.com/en-us/library/windows/desktop/bb762153(v=vs.85).aspx

	if (args.Length() > 1)
	{
		String::Value arguments(args[1]);
		strArguments = (wchar_t*)*arguments;
	}

	int nShowOption = SW_SHOWDEFAULT;
	if (args.Length() > 2)
	{
		bool bShow = args[2]->BooleanValue();
		if (bShow)
			nShowOption = SW_SHOW;
		else
			nShowOption = SW_HIDE;
	}

	// TODO change to ShellExecuteEx (to get return value)
	ShellExecute(
		nullptr,
		L"open",
		strExePath.wstr(),
		strArguments.wstr(),
		nullptr,
		nShowOption
		);

	// TODO implement return value
}

// description: Execute external process and return exit code
// syntax:		system.executeSync(exefile_path[, arguments[, is_show]])
void NaBasicModule::method_executeSync(V8_FUNCTION_ARGS)
{
	String::Value exepath(args[0]);
	NaString strExePath((wchar_t*)*exepath);
	NaString strArguments = L"";

	// https://msdn.microsoft.com/en-us/library/windows/desktop/bb762153(v=vs.85).aspx

	if (args.Length() > 1)
	{
		String::Value arguments(args[1]);
		strArguments = (wchar_t*)*arguments;
	}

	int nShowOption = SW_SHOWDEFAULT;
	if (args.Length() > 2)
	{
		bool bShow = args[2]->BooleanValue();
		if (bShow)
			nShowOption = SW_SHOW;
		else
			nShowOption = SW_HIDE;
	}

	SHELLEXECUTEINFOW info;
	ZeroMemory(&info, sizeof(SHELLEXECUTEINFOW));
	info.cbSize = sizeof(SHELLEXECUTEINFO);
	info.lpFile = strExePath.wstr();
	info.lpParameters = strArguments.wstr();
	info.nShow = nShowOption;
	info.fMask = SEE_MASK_NOCLOSEPROCESS;
	info.lpVerb = nullptr;
	ShellExecuteEx(&info);

	// Waiting
	WaitForSingleObject(info.hProcess, INFINITE);
	
	// Get exit code
	DWORD dwExitCode = 0;
	GetExitCodeProcess(info.hProcess, &dwExitCode);

	// implement return value
	V8Wrap::SetReturnValue(args, (int)dwExitCode);
}
