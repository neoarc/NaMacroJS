#pragma once

#include <Windows.h>
#include <map>
#include "NaString.h"

class NaFile
{
public:
	NaFile();
	virtual ~NaFile();

	// members
	long m_lUID;
	FILE* m_hFile;
	NaString m_strName;

	bool IsExist();
	char* Read();
	size_t Write(NaString& str);
	void Close();

	// static
	static NaFile* Load(const wchar_t *filename, const char *mode);
	static bool IsExist(const wchar_t *filename);
};