#include "NaString.h"

#include <assert.h>
#include <stdarg.h>
#include <stdio.h>

#include <Windows.h>

NaString::NaString() : m_pBuf(nullptr), m_nBufLen(0), m_nLen(0)
{
}

NaString::NaString(const char * lpsz)
{
	m_pBuf = nullptr;
	m_nBufLen = 0;
	m_nLen = 0;

	SetBuf(lpsz);
}

NaString::NaString(const wchar_t * lpsz)
{
	m_pBuf = nullptr;
	m_nBufLen = 0;
	m_nLen = 0;

	SetBuf(lpsz);
}

NaString::NaString(const NaString & nstr)
{
	m_pBuf = nullptr;
	m_nBufLen = 0;
	m_nLen = 0;

	SetBuf((wchar_t*)nstr.m_pBuf);
}

#if defined(USE_V8)
NaString::NaString(Local<String>& str)
{
	m_pBuf = nullptr;
	m_nBufLen = 0;
	m_nLen = 0;

	String::Utf8Value value(str);
	SetBuf(*value);
}
#endif

NaString::~NaString()
{
	if (m_pBuf)
	{
		delete m_pBuf;
		m_pBuf = nullptr;
	}
}

const NaString& NaString::operator=(const wchar_t *lpsz)
{
	SetBuf((wchar_t*)lpsz);
	return *this;
}

const NaString& NaString::operator=(const NaString &str)
{
	SetBuf((wchar_t*)str.m_pBuf);
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

bool NaString::operator==(const char * lpsz)
{
	return Compare(NaString(lpsz).wstr()) == 0;
}

bool NaString::operator==(const wchar_t * lpsz)
{
	return Compare(lpsz) == 0;
}

bool NaString::operator==(NaString & str)
{
	return Compare(str.wstr()) == 0;
}

wchar_t NaString::operator[](int index)
{
	if (index >= 0 && index < m_nLen)
		return *((wchar_t*)m_pBuf + index);

	return L'\0';
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
	int minlen = m_nLen < arglen ? m_nLen : arglen;

	return wcscmp((wchar_t*)m_pBuf, lpsz);
}

int NaString::CompareNoCase(const wchar_t * lpsz)
{
	NaString str(*this);
	str.ToLower();
	
	NaString str2(lpsz);
	str2.ToLower();

	return str.Compare(str2);
}

int NaString::Find(wchar_t * ch, int begin /*= 0*/)
{
	for (int i = begin; i < m_nLen; i++)
	{
		if (*((wchar_t*)m_pBuf + i) == *ch)
			return i;
	}
	return -1;
}

NaString NaString::Left(int len)
{
	if (len >= m_nLen)
	{
		NaString strRet((wchar_t*)this->m_pBuf);
		return strRet;
	}

	NaString strRet;
	unsigned char *buf;
	int nNewLen = len;
	int nNewBufLen = sizeof(wchar_t) * (len + 1);
	buf = new unsigned char[nNewBufLen];

	memcpy(buf, this->m_pBuf, nNewBufLen);
	*((wchar_t*)buf + len) = 0;

	strRet.SetBuf((wchar_t*)buf);
	delete buf;

	return strRet;
}

NaString NaString::Mid(int index, int len /*= -1*/)
{
	if (index == 0 && len == -1)
	{
		NaString strRet((wchar_t*)this->m_pBuf);
		return strRet;
	}

	if (len == -1)
		len = m_nLen - index;
	if (index + len > m_nLen)
		len = m_nLen - index;

	NaString strRet;
	unsigned char *buf;
	int nNewLen = len;
	int nNewBufLen = sizeof(wchar_t) * (len + 1);
	buf = new unsigned char[nNewBufLen];

	memcpy(buf, (wchar_t*)(this->m_pBuf) + index, nNewBufLen);
	*((wchar_t*)buf + len) = 0;

	strRet.SetBuf((wchar_t*)buf);
	delete buf;

	return strRet;
}

NaString NaString::Right(int len)
{
	if (len >= m_nLen)
	{
		NaString strRet((wchar_t*)this->m_pBuf);
		return strRet;
	}

	int index = m_nLen - len;

	NaString strRet;
	unsigned char *buf;
	int nNewLen = len;
	int nNewBufLen = sizeof(wchar_t) * (len + 1);
	buf = new unsigned char[nNewBufLen];

	memcpy(buf, (wchar_t*)(this->m_pBuf) + index, nNewBufLen);
	*((wchar_t*)buf + len) = 0;

	strRet.SetBuf((wchar_t*)buf);
	delete buf;

	return strRet;
}

NaStrArray NaString::Split(wchar_t ch)
{
	NaStrArray ar;

	int from = 0;
	bool bBreak = false;


	while (!bBreak)
	{
		NaString token;
		int index = this->Find(&ch, from);
		if (index < 0)
		{
			bBreak = true;
			token = this->Mid(from);
		}
		else
		{
			token = this->Mid(from, index - from);
		}

		ar.Add(token);
		from = index + 1;
	}

	return ar;
}

int NaString::ReplaceAll(wchar_t * from, wchar_t * to)
{
	NaString temp(L"");
	int begin = 0;

	while (true)
	{
		int index = Find(from, begin);
		if (index < 0)
		{
			temp += Mid(begin, m_nLen - begin);
			break;
		}

		temp += Mid(begin, index - begin);
		temp += to;

		begin = index + 1;
	}

	SetBuf(temp.wstr());

	return 0;
}

const NaString & NaString::AppendFormat(const wchar_t * fmt, ...)
{
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

const char * NaString::cstr()
{
	int nChars = WideCharToMultiByte(CP_ACP, 0, (wchar_t*)m_pBuf, -1, NULL, 0, 0, 0);
	char *str = new char[nChars];
	WideCharToMultiByte(CP_ACP, 0, (wchar_t*)m_pBuf, -1, str, nChars, 0, 0);
	
	return str;
}

wchar_t NaString::GetLast()
{
	if (m_nLen <= 0)
		return 0;

	return *((wchar_t*)m_pBuf + m_nLen);
}

const NaString & NaString::SetBuf(const wchar_t* lpsz, int len)
{
	if (lpsz == nullptr)
	{
		m_nLen = 0;
		m_nBufLen = (int)(sizeof(wchar_t) * (m_nLen + 1));
		AllocBuf(m_nBufLen);
		*((wchar_t*)m_pBuf + m_nLen) = 0;
	}
	else
	{
		if (len < 0)
		{
			m_nLen = wcslen(lpsz);
			m_nBufLen = (int)(sizeof(wchar_t) * (m_nLen + 1));
		}

		AllocBuf(m_nBufLen);
		memcpy(m_pBuf, lpsz, m_nBufLen);

		*((wchar_t*)m_pBuf + m_nLen) = 0;
	}

	return *this;
}

const NaString & NaString::SetBuf(const char * sz, int len)
{
	// convert to wstr
	len = MultiByteToWideChar(CP_UTF8, 0, sz, -1, 0, 0);
	if (m_pBuf)
		delete m_pBuf;

	m_nLen = len;
	m_nBufLen = sizeof(wchar_t) * (len + 1);
	AllocBuf(m_nBufLen);

	len = MultiByteToWideChar(CP_UTF8, 0, sz, -1, (wchar_t*)m_pBuf, len);
	if (len == 0)
		assert(0);

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

//
// String Array
// 2016.06.17
//
NaStrArray::NaStrArray()
{
}

NaStrArray::~NaStrArray()
{
	m_Array.clear();
}

NaString NaStrArray::operator[](int nIndex)
{
	std::list<NaString>::iterator it = m_Array.begin();
	for (int i=0; it != m_Array.end(); ++it,++i)
	{
		if (i == nIndex)
			return (*it);
	}

	return NaString();
}

int NaStrArray::Add(NaString str)
{
	m_Array.push_back(str);
	return GetCount();
}

int NaStrArray::Remove(int nIndex)
{
	std::list<NaString>::iterator it = m_Array.begin();
	for (int i = 0; it != m_Array.end(); ++it, ++i)
	{
		if (i == nIndex)
		{
			m_Array.erase(it);
			return i;
		}
	}
	return -1;
}

int NaStrArray::GetCount()
{
	return m_Array.size();
}

NaString NaStrArray::Join(wchar_t * ch)
{
	int nCnt = GetCount();
	NaString str(L"");
	std::list<NaString>::iterator it = m_Array.begin();
	for (int i = 0; it != m_Array.end(); ++it, ++i)
	{
		str += *it;
		if (i < nCnt-1)
			str += ch;
	}

	return str;
}

NaString NaStrArray::Pop()
{
	NaString strRet(L"");
	int nCnt = GetCount();
	std::list<NaString>::iterator it = m_Array.begin();
	for (int i = 0; it != m_Array.end(); ++it, ++i)
	{
		if (i == nCnt - 1)
		{
			strRet = *it;
			m_Array.erase(it);
			break;
		}		
	}
	return strRet;
}
