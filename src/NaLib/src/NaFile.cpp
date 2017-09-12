#include "stdafx.h"
#include "NaFile.h"

#include "NaDebug.h"

NaFile::NaFile()
{
	m_hFile = NULL;
}

NaFile::~NaFile()
{
	if (m_hFile)
	{
		fclose(m_hFile);
	}
}

bool NaFile::IsExist()
{
	return NaFile::IsExist(m_strName.wstr());
}

char * NaFile::Read()
{
	fseek(m_hFile, 0, SEEK_END);
	int nSize = ftell(m_hFile);
	rewind(m_hFile);

	char *buf = new char[nSize + 1];
	fread(buf, nSize, 1, m_hFile);
	buf[nSize] = '\0';

	return buf;
}

size_t NaFile::Write(NaString & str)
{
	auto c = str.cstr();
	auto len = strlen(c);

	return fwrite(c, len, 1, m_hFile);
}

void NaFile::Close()
{
	if (m_hFile != nullptr)
	{
		fclose(m_hFile);
		m_hFile = nullptr;
	}
}

// description: load file for read/write
NaFile * NaFile::Load(const wchar_t * filename, const char * mode)
{
	NaFile *pFile = new NaFile;

	pFile->m_strName = filename;
	fopen_s(&(pFile->m_hFile), pFile->m_strName.cstr(), mode);

	return pFile;
}

bool NaFile::IsExist(const wchar_t * filename)
{
	DWORD dwAttrs = ::GetFileAttributes(filename);
	return (dwAttrs != INVALID_FILE_ATTRIBUTES);
}
