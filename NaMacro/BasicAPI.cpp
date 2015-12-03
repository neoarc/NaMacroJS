#include "stdafx.h"

#include "Common.h"
#include "BasicAPI.h"

#include "Windows.h"

void InitBasicAPI(v8::Handle<v8::ObjectTemplate> &global_template)
{
#define ADD_GLOBAL_API(_js_func, _c_func) global_template->Set(v8::String::New(#_js_func), v8::FunctionTemplate::New(_c_func));
	ADD_GLOBAL_API(sleep, Sleep);
	ADD_GLOBAL_API(alert, Alert);
	ADD_GLOBAL_API(print, Print);
	ADD_GLOBAL_API(exit, Exit);
}

// description: Print message to console
// syxtax:		print(message) 
v8::Handle<v8::Value> Print(const v8::Arguments& args)
{
	bool first = true;
	for (int i = 0; i < args.Length(); i++)
	{
		v8::HandleScope handle_scope;
		if (first) {
			first = false;
		}
		else {
			printf(" ");
		}
		v8::String::Utf8Value str(args[i]);
		const char* cstr = ToCString(str);
		printf("%s", cstr);
	}
	printf("\n");
	fflush(stdout);
	return v8::Undefined();
}

// description: show MessageBox with message
// syntax:		alert(message, title, type)
v8::Handle<v8::Value> Alert(const v8::Arguments& args) 
{
	v8::String::Utf8Value msg(args[0]);
	v8::String::Utf8Value title(args[1]);
	int nType = args[2]->Int32Value();

	//const char* cstr = (const char*)*str;
	::MessageBoxA(NULL, 
		*msg, 
		args.Length() >= 2 ? *title : "Alert", 
		args.Length() >= 3 ? nType : MB_OK
		);

	return v8::Undefined();
}

v8::Handle<v8::Value> Sleep(const v8::Arguments& args)
{
	int nTime = args[0]->Int32Value();

	::Sleep(args.Length() > 0 ? nTime : 1);

	return v8::Undefined();
}

// Reads a file into a v8 string.
v8::Handle<v8::String> ReadFile(const char* name)
{
	FILE* file = fopen(name, "rb");
	if (file == NULL) 
		return v8::Handle<v8::String>();

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
	v8::Handle<v8::String> result = v8::String::New(chars, size);
	delete[] chars;
	return result;
}

v8::Handle<v8::Value> Exit(const v8::Arguments& args)
{
	g_bExit = true;
	return v8::Undefined();
}
