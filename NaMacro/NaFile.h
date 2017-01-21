#pragma once

#include "Common.h"
#include "NaObject.h"
#include "NaString.h"

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
	DEFINE_CLASS_ACCESSOR(GetName, SetName);

	// methods
	DEFINE_CLASS_METHOD(Constructor);
	DEFINE_CLASS_METHOD(Read);
	DEFINE_CLASS_METHOD(Write);
	DEFINE_CLASS_METHOD(Close);
};