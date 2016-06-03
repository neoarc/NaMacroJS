#include "NaString.h"

#include <assert.h>
#include <stdarg.h>
#include <stdio.h>

#include <Windows.h>

NaString::NaString() : m_pBuf(nullptr), m_nBufLen(0), m_nLen(0)
{
}

NaString::NaString(const wchar_t * lpsz)
{
	m_pBuf = nullptr;
	m_nBufLen = 0;
	m_nLen = 0;

	SetBuf(lpsz);
}

NaString::NaString(NaString & nstr)
{
	m_pBuf = nullptr;
	m_nBufLen = 0;
	m_nLen = 0;

	SetBuf(nstr.GetBuf());
}

const NaString& NaString::operator=(const wchar_t *lpsz)
{
	SetBuf((wchar_t*)lpsz);
	return *this;
}

const NaString& NaString::operator=(NaString &str)
{
	SetBuf((wchar_t*)str.GetBuf());
	return *this;
}

const NaString& NaString::operator+=(const wchar_t *lpsz)
{
	Format(L"%s%s", m_pBuf, lpsz);
	return *this;
}

const NaString& NaString::operator+=(NaString &str)
{
	Format(L"%s%s", m_pBuf, str.wstr());
	return *this;
}

NaString::~NaString()
{
	if (m_pBuf)
	{
		delete m_pBuf;
		m_pBuf = nullptr;
	}
}

NaString::operator const char* () const
{
	if (m_pBuf == nullptr)
		return "";

	int len = WideCharToMultiByte(CP_UTF8, 0, (wchar_t*)m_pBuf, -1, 0, 0, 0, 0);
	char *sz = (char*)malloc(len);
	if (sz == 0)
		return "";
	
	len = WideCharToMultiByte(CP_UTF8, 0, (wchar_t*)m_pBuf, -1, sz, len, 0, 0);
	if (len == 0)
	{
		free(sz);
		sz = 0;
	}

	return sz;
}

NaString::operator const wchar_t* () const
{
	if (m_pBuf == nullptr)
		return L"";
	return (const wchar_t*)m_pBuf;
}

void NaString::ToLower()
{
	_wcslwr_s((wchar_t*)m_pBuf, m_nLen+1);
}

void NaString::ToUpper()
{
	_wcsupr_s((wchar_t*)m_pBuf, m_nLen+1);
}

const NaString & NaString::Format(const wchar_t * fmt, ...)
{
	wchar_t buf[NASTRING_FORMAT_BUFFER_SIZE];
	memset(&buf, 0, sizeof(wchar_t) * NASTRING_FORMAT_BUFFER_SIZE);

	va_list arglist;
	va_start(arglist, fmt);
	vswprintf_s(buf, NASTRING_FORMAT_BUFFER_SIZE, fmt, arglist);
	va_end(arglist);
	
	SetBuf(buf);
	return *this;
}

const NaString & NaString::Format(const char * fmt, ...)
{
	char buf[NASTRING_FORMAT_BUFFER_SIZE];
	memset(&buf, 0, sizeof(char) * NASTRING_FORMAT_BUFFER_SIZE);

	va_list arglist;
	va_start(arglist, fmt);
	vsprintf_s(buf, NASTRING_FORMAT_BUFFER_SIZE, fmt, arglist);
	va_end(arglist);

	// convert to wstr
	int len = MultiByteToWideChar(CP_UTF8, 0, buf, -1, 0, 0);
	if (m_pBuf)
		delete m_pBuf;

	m_nLen = len + 1;
	m_nBufLen = sizeof(wchar_t) * (len + 1);
	AllocBuf(m_nBufLen);

	len = MultiByteToWideChar(CP_UTF8, 0, buf, -1, (wchar_t*)m_pBuf, len);
	if (len == 0)
		assert(0);

	return *this;
}

int NaString::GetLength()
{
	return m_nLen;
}

int NaString::Compare(const wchar_t * lpsz)
{
	int arglen = wcslen(lpsz);
	int minlen = m_nBufLen < arglen ? m_nBufLen : arglen;

	return memcmp(m_pBuf, lpsz, minlen);
}

int NaString::CompareNoCase(const wchar_t * lpsz)
{
	NaString str(*this);
	str.ToLower();
	
	NaString str2(lpsz);
	str2.ToLower();

	return str.Compare(str2);
}

const NaString & NaString::AppendFormat(const wchar_t * fmt, ...)
{
	// TODO: 여기에 반환 구문을 삽입합니다.
	wchar_t buf[NASTRING_FORMAT_BUFFER_SIZE];
	memset(&buf, 0, sizeof(wchar_t) * NASTRING_FORMAT_BUFFER_SIZE);

	va_list arglist;
	va_start(arglist, fmt);
	vswprintf_s(buf, NASTRING_FORMAT_BUFFER_SIZE, fmt, arglist);
	va_end(arglist);

	Format(L"%s%s", m_pBuf, buf);
	return *this;
}

const NaString & NaString::AppendFormat(const char * fmt, ...)
{
	// TODO: 여기에 반환 구문을 삽입합니다.
	char buf[NASTRING_FORMAT_BUFFER_SIZE];
	memset(&buf, 0, sizeof(char) * NASTRING_FORMAT_BUFFER_SIZE);

	va_list arglist;
	va_start(arglist, fmt);
	vsprintf_s(buf, NASTRING_FORMAT_BUFFER_SIZE, fmt, arglist);
	va_end(arglist);

	// convert to wstr
	/*
	int len = MultiByteToWideChar(CP_UTF8, 0, buf, -1, 0, 0);
	int buflen = sizeof(wchar_t) * (len + 1);
	wchar_t *wbuf = new wchar_t[buflen];

	len = MultiByteToWideChar(CP_UTF8, 0, buf, -1, (wchar_t*)wbuf, len);
	if (len == 0)
		assert(0);
	*/
	Format("%s%s", (const char*)*this, buf);
	return *this;
}

const wchar_t * NaString::wstr()
{
	if (m_pBuf == nullptr)
		return L"";
	return (const wchar_t*)m_pBuf;
}

const NaString & NaString::SetBuf(const wchar_t* lpsz, int len)
{
	if (len < 0)
	{
		m_nLen = wcslen(lpsz);
		m_nBufLen = (int)(sizeof(wchar_t) * (m_nLen + 1));
	}

	AllocBuf(m_nBufLen);
	memcpy(m_pBuf, lpsz, m_nBufLen);

	*((wchar_t*)m_pBuf + m_nLen) = 0;

	return *this;
}

const wchar_t* NaString::GetBuf()
{
	return reinterpret_cast<wchar_t*>(m_pBuf);
}

void NaString::AllocBuf(int len)
{
	if (len < 0)
		assert(0);

	m_pBuf = new unsigned char[len];
}
