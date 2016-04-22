#include "stdafx.h"

#include "Common.h"

#include "stdio.h"
#include "stdarg.h"
#include "Windows.h"

// Global Var
bool g_bExit = false;

void NaDebugOut(const char* pszFormat, ...)
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

// description: return 'system'
v8::Local<v8::Object> GetSystemObject(v8::Isolate *isolate)
{
	// HandleScope 안에서 호출

	v8::Local<v8::Object> global = isolate->GetCurrentContext()->Global();
	v8::Local<v8::String> system_name = v8::String::NewFromUtf8(isolate, "system", v8::NewStringType::kNormal).ToLocalChecked();
	v8::Local<v8::Value> system_value = global->Get(system_name);
	if (!system_value.IsEmpty() && system_value->IsUndefined())
	{
		// InitSystem
		system_value = v8::Object::New(isolate);
		global->Set(system_name, system_value);
	}

	v8::Local<v8::Object> system_obj = system_value->ToObject();
	return system_obj;
}
