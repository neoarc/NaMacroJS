#pragma once

#include "V8Wrap.h"
#include "NaObject.h"

#include <Windows.h>
#include <map>

class NaFile : public NaObject
{
public:
	NaFile();
	virtual ~NaFile();

	// members
	long m_lUID;
	FILE* m_hFile;
	NaString m_strName;

	// static
	static NaFile* Load(const wchar_t *filename, const char *mode);
	
	// wrap object
	virtual Local<ObjectTemplate> MakeObjectTemplate(Isolate *isolate);
	virtual Global<ObjectTemplate>& GetObjectTemplate() { return s_NaFileTemplate; }
	static Global<ObjectTemplate> s_NaFileTemplate;

	// accessors
	DEFINE_CLASS_ACCESSOR(name);

	// methods
	DEFINE_CLASS_METHOD(constructor);
	DEFINE_CLASS_METHOD(read);
	DEFINE_CLASS_METHOD(write);
	DEFINE_CLASS_METHOD(close);
};