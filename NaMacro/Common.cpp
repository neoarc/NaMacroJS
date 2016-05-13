#include "stdafx.h"

#include "Common.h"

#include "stdio.h"
#include "stdarg.h"
#include "Windows.h"

// Global Var
bool g_bReportExceptions = true;
bool g_bExit = false;

void ReportException(Isolate *isolate, TryCatch* try_catch)
{
	HandleScope handle_scope(isolate);
	String::Utf8Value exception(try_catch->Exception());
	const char* exception_string = *exception;
	Local<Message> message = try_catch->Message();
	if (message.IsEmpty())
	{
		// V8 didn't provide any extra information about this error; just
		// print the exception.
		printf("%s\n", exception_string);
	}
	else
	{
		// Print (filename):(line number): (message).
		String::Utf8Value filename(message->GetScriptResourceName());
		const char* filename_string = *filename;
		int linenum = message->GetLineNumber();
		printf("%s:%i: %s\n", filename_string, linenum, exception_string);
		NaDebugOutA("%s:%i: %s\n", filename_string, linenum, exception_string);

		// Print line of source code.
		String::Utf8Value sourceline(message->GetSourceLine());
		const char* sourceline_string = *sourceline;
		printf("%s\n", sourceline_string);
		NaDebugOutA("%s\n", sourceline_string);

		// Print wavy underline (GetUnderline is deprecated).
		int start = message->GetStartColumn();
		for (int i = 0; i < start; i++)
		{
			printf(" ");
			NaDebugOutA(" ");
		}

		int end = message->GetEndColumn();
		for (int i = start; i < end; i++)
		{
			printf("^");
			NaDebugOutA("^");
		}
		printf("\n");
		NaDebugOutA("\n");
	}
}

void NaDebugOutA(const char* pszFormat, ...)
{
#if !defined(_DEBUG)
	return;
#endif

	char str[NA_DEBUGOUT_TEMPBUFFER_SIZE];
	memset(str, 0, sizeof(char) * NA_DEBUGOUT_TEMPBUFFER_SIZE);

	va_list arglist;
	va_start(arglist, pszFormat);
	vsprintf(str, pszFormat, arglist);
	va_end(arglist);

	OutputDebugStringA(str);
}

void NaDebugOut(const wchar_t* pszFormat, ...)
{
#if !defined(_DEBUG)
	return;
#endif

	wchar_t str[NA_DEBUGOUT_TEMPBUFFER_SIZE];
	memset(str, 0, sizeof(wchar_t) * NA_DEBUGOUT_TEMPBUFFER_SIZE);

	va_list arglist;
	va_start(arglist, pszFormat);
	vswprintf(str, pszFormat, arglist);
	va_end(arglist);

	OutputDebugStringW(str);
}
// Reads a file into a v8 string.
Local<String> ReadFile(Isolate *isolate, const char* name)
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

// description: return 'system'
Local<Object> GetSystemObject(Isolate *isolate)
{
	// HandleScope 안에서 호출

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
