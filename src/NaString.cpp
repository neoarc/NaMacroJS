#include "stdafx.h"
#include "NaString.h"

#include <cassert>
#include <iostream>

NaString::NaString()
{
	Init();
}

NaString::NaString(const char* lpsz)
{
	Init();
	SetBuf(lpsz);
}

NaString::NaString(const wchar_t* lpsz)
{
	Init();
	SetBuf(lpsz);
}

NaString::NaString(const NaString& nstr)
{
	Init();
	SetBuf((wchar_t*)nstr.m_buf);
}

#if defined(USE_V8)
NaString::NaString(Local<String>& str)
{
	Init();
	String::Utf8Value value(str);
	SetBuf(*value);
}

NaString::NaString(String::Value& str)
{
	Init();
	SetBuf((const wchar_t*)*str);
}
#endif

NaString::~NaString()
{
	DeallocBuf();
	if (m_charBuf != nullptr)
	{
		delete[] m_charBuf;
		m_charBuf = nullptr;
	}
}

const NaString& NaString::operator=(const wchar_t* lpsz)
{
	SetBuf((wchar_t*)lpsz);
	return *this;
}

const NaString& NaString::operator=(const NaString& str)
{
	SetBuf((wchar_t*)str.m_buf);
	return *this;
}

const NaString& NaString::operator+=(const wchar_t* lpsz)
{
	if (m_buf == nullptr || m_len == 0)
		SetBuf(lpsz);
	else
		Format(L"%ls%ls", (wchar_t*)m_buf, lpsz);
	return *this;
}

const NaString& NaString::operator+=(const NaString& str)
{
	if (m_buf == nullptr || m_len == 0)
		SetBuf(str.wstr());
	else
		Format(L"%ls%ls", (wchar_t*)m_buf, str.wstr());
	return *this;
}

bool NaString::operator==(const char* lpsz) const
{
	return Compare(NaString(lpsz).wstr()) == 0;
}

bool NaString::operator==(const wchar_t* lpsz) const
{
	return Compare(lpsz) == 0;
}

bool NaString::operator==(const NaString& str) const
{
	return Compare(str.wstr()) == 0;
}

bool NaString::operator<(const NaString& str) const
{
	return wcscmp((wchar_t*)m_buf, (wchar_t*)str.m_buf) < 0;
}

wchar_t NaString::operator[](const int index) const
{
	if (index >= 0 && index < m_len)
		return *((wchar_t*)m_buf + index);

	return L'\0';
}

NaString::operator const char* () const
{
	char *sz = nullptr;
	ConvertWCharToChar((wchar_t*)m_buf, &sz);

	return sz;
}

NaString::operator const wchar_t* () const
{
	if (m_buf == nullptr)
		return L"";
	return (const wchar_t*)m_buf;
}

void NaString::ToLower()
{
#ifdef WIN32
	_wcslwr_s((wchar_t*)m_buf, m_len + 1);
#else
#endif
}

void NaString::ToUpper()
{
#ifdef WIN32
	_wcsupr_s((wchar_t*)m_buf, m_len + 1);
#else
#endif
}

const NaString & NaString::Format(const wchar_t* fmt, ...)
{
	const int nBufSize = 5 * NASTRING_FORMAT_BUFFER_SIZE;
	wchar_t *buf = new wchar_t[nBufSize];
	memset(buf, 0, sizeof(wchar_t) * nBufSize);

	va_list arglist;
	va_start(arglist, fmt);
#ifdef WIN32
	vswprintf_s(buf, nBufSize, fmt, arglist);
#else
	vswprintf(buf, nBufSize, fmt, arglist);
#endif
	va_end(arglist);

	SetBuf(buf);
	delete[] buf;

	return *this;
}

const NaString & NaString::Format(const char* fmt, ...)
{
	char buf[NASTRING_FORMAT_BUFFER_SIZE];
	memset(&buf, 0, sizeof(char) * NASTRING_FORMAT_BUFFER_SIZE);

	va_list arglist;
	va_start(arglist, fmt);
#ifdef WIN32
	vsprintf_s(buf, NASTRING_FORMAT_BUFFER_SIZE, fmt, arglist);
#else
	vsprintf(buf, fmt, arglist);
#endif
	va_end(arglist);

	DeallocBuf();

	// convert to wstr
	int nLen = ConvertCharToWChar(buf, (wchar_t**)&m_buf);
	m_len = nLen - 1;
	m_bufLen = sizeof(wchar_t) * (nLen);

	return *this;
}

int NaString::GetLength() const
{
	return m_len;
}

int NaString::Compare(const wchar_t* lpsz) const
{
	/*
	int arglen = wcslen(lpsz);
	int minlen = m_nLen < arglen ? m_nLen : arglen;
	*/

	if (m_buf == nullptr)
		return wcscmp((wchar_t*)L"", lpsz);

	return wcscmp((wchar_t*)m_buf, lpsz);
}

int NaString::CompareNoCase(const wchar_t* lpsz) const
{
	NaString str(*this);
	str.ToLower();

	NaString str2(lpsz);
	str2.ToLower();

	return str.Compare(str2);
}

int NaString::Find(const wchar_t* const ch, const int begin /*= 0*/) const
{
	int itemlen = (int)wcslen(ch);
	if (m_len < itemlen)
		return -1;

	for (int i = begin; i < m_len - itemlen + 1; i++)
	{
		bool bMatch = true;
		for (int j = 0; j < itemlen; j++)
		{
			if (*((wchar_t*)m_buf + i + j) != *(ch + j))
			{
				bMatch = false;
				break;
			}
		}

		if (bMatch == true)
			return i;
	}
	return -1;
}

NaString NaString::Left(const int count) const
{
	if (count <= 0)
		return L"";

	if (count >= m_len)
	{
		NaString strRet((wchar_t*)this->m_buf);
		return strRet;
	}

	NaString strRet;
	unsigned char *buf;
	int nNewLen = count;
	int nNewBufLen = sizeof(wchar_t) * (nNewLen + 1);
	buf = new unsigned char[nNewBufLen];

	memcpy(buf, this->m_buf, nNewBufLen);
	*((wchar_t*)buf + nNewLen) = 0;

	strRet.SetBuf((wchar_t*)buf);
	delete[] buf;

	return strRet;
}

NaString NaString::Mid(const int index, const int count /*= -1*/) const
{
	int idx = index;
	if (idx < 0)
		idx = 0;

	if (idx == 0 && count == -1)
	{
		NaString ret((wchar_t*)this->m_buf);
		return ret;
	}

	int length = count;
	if (length == -1)
		length = m_len - idx;
	if (idx + length > m_len)
		length = m_len - idx;

	int nNewBufLen = sizeof(wchar_t) * (length + 1);
	unsigned char *buf = new unsigned char[nNewBufLen];

	memcpy(buf, (wchar_t*)(this->m_buf) + idx, nNewBufLen);
	*((wchar_t*)buf + length) = 0;

	NaString ret;
	ret.SetBuf((wchar_t*)buf);
	delete[] buf;

	return ret;
}

NaString NaString::Right(const int count) const
{
	if (count <= 0)
		return L"";

	if (count >= m_len)
	{
		NaString strRet((wchar_t*)this->m_buf);
		return strRet;
	}

	int index = m_len - count;

	int nNewBufLen = sizeof(wchar_t) * (count + 1);
	unsigned char *buf = new unsigned char[nNewBufLen];

	memcpy(buf, (wchar_t*)(this->m_buf) + index, nNewBufLen);
	*((wchar_t*)buf + count) = 0;

	NaString ret;
	ret.SetBuf((wchar_t*)buf);
	delete[] buf;

	return ret;
}

NaStrArray NaString::Split(wchar_t* ch) const
{
	NaStrArray ar;

	int from = 0;
	bool bBreak = false;

	while (!bBreak)
	{
		NaString token;
		int index = this->Find(ch, from);
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

int NaString::ReplaceAll(wchar_t* from, wchar_t* to)
{
	NaString temp(L"");
	int begin = 0;

	while (true)
	{
		int index = Find(from, begin);
		if (index < 0)
		{
			temp += Mid(begin, m_len - begin);
			break;
		}

		temp += Mid(begin, index - begin);
		temp += to;

		begin = index + (int)wcslen(from);
	}

	SetBuf(temp.wstr());

	return 0;
}

const NaString & NaString::AppendFormat(const wchar_t* fmt, ...)
{
	wchar_t buf[NASTRING_FORMAT_BUFFER_SIZE];
	memset(&buf, 0, sizeof(wchar_t) * NASTRING_FORMAT_BUFFER_SIZE);

	va_list arglist;
	va_start(arglist, fmt);
#ifdef WIN32
	vswprintf_s(buf, NASTRING_FORMAT_BUFFER_SIZE, fmt, arglist);
#else
	vswprintf(buf, NASTRING_FORMAT_BUFFER_SIZE, fmt, arglist);
#endif
	va_end(arglist);

	Format(L"%ls%ls", (wchar_t*)m_buf, buf);
	return *this;
}

const NaString & NaString::AppendFormat(const char* fmt, ...)
{
	char buf[NASTRING_FORMAT_BUFFER_SIZE];
	memset(&buf, 0, sizeof(char) * NASTRING_FORMAT_BUFFER_SIZE);

	va_list arglist;
	va_start(arglist, fmt);
#ifdef WIN32
	vsprintf_s(buf, NASTRING_FORMAT_BUFFER_SIZE, fmt, arglist);
#else
	vsprintf(buf, fmt, arglist);
#endif
	va_end(arglist);

	Format("%s%s", cstr(), buf);
	return *this;
}

const wchar_t* NaString::wstr() const
{
	if (m_buf == nullptr)
		return L"";
	return (const wchar_t*)m_buf;
}

const char* NaString::cstr()
{
	if (m_charBuf != nullptr)
	{
		delete[] m_charBuf;
		m_charBuf = nullptr;
	}

	ConvertWCharToChar((wchar_t*)m_buf, &m_charBuf);

	return m_charBuf;
}

wchar_t NaString::GetLast() const
{
	if (m_len <= 0)
		return 0;

	return *((wchar_t*)m_buf + m_len);
}

int NaString::ToInt() const
{
	return std::stoi(wstr());
}

float NaString::ToFloat() const
{
	return std::stof(wstr());
}

double NaString::ToDouble() const
{
	return std::stod(wstr());
}

int NaString::ConvertWCharToChar(const wchar_t* wstr, char** str)
{
#ifdef WIN32
	int nChars = WideCharToMultiByte(CP_ACP, 0, (wchar_t*)wstr, -1, NULL, 0, 0, 0);
	if (nChars == 0)
	{
		*str = new char[1];
		*(*str) = '\0';
		return 0;
	}

	*str = new char[nChars];
	WideCharToMultiByte(CP_ACP, 0, (wchar_t*)wstr, -1, *str, nChars, 0, 0);
#else
	int nChars = wcstombs(nullptr, wstr, 0);
	*str = new char[nChars + 1];

	size_t szTest = wcstombs(*str, wstr, nChars);
	*((char*)(*str) + nChars) = '\0';

	//NaDebugOut(L"* ConvWCharToChar\n* (1) %ls\n", wstr);
	//printf("* (2) %s\n", *str);
#endif

	return nChars;
}

int NaString::ConvertCharToWChar(const char* str, wchar_t** wstr)
{
#ifdef WIN32
	// MBTWC = contains end byte(zero)
	int nWChars = MultiByteToWideChar(CP_UTF8, 0, str, -1, 0, 0);
	if (nWChars == 0)
	{
		*wstr = new wchar_t[1];
		*(*wstr) = L'\0';
		return 0;
	}

	*wstr = new wchar_t[nWChars];
	MultiByteToWideChar(CP_UTF8, 0, str, -1, (wchar_t*)*wstr, nWChars);
#else
	// strlen = just text length
	int nWChars = strlen(str) + 1;

	*wstr = new wchar_t[nWChars];
	mbstowcs((wchar_t*)*wstr, str, nWChars);

	*((*wstr) + nWChars - 1) = L'\0';
#endif

	return nWChars;
}

void NaString::Init()
{
	m_buf = nullptr;
	m_bufLen = 0;
	m_len = 0;
	m_charBuf = nullptr;
}

const NaString& NaString::SetBuf(const wchar_t* lpsz)
{
	unsigned char* pCurBuf = m_buf;

	if (lpsz == nullptr)
	{
		m_len = 0;
		m_bufLen = (int)(sizeof(wchar_t) * (m_len + 1));
		AllocBuf(m_bufLen);
		*((wchar_t*)m_buf + m_len) = 0;
	}
	else
	{
		m_len = (int)wcslen(lpsz);
		m_bufLen = (int)(sizeof(wchar_t) * (m_len + 1));

		AllocBuf(m_bufLen);
		memcpy(m_buf, lpsz, m_bufLen);

		*((wchar_t*)m_buf + m_len) = 0;
	}

	delete pCurBuf;

	return *this;
}

const NaString& NaString::SetBuf(const char* sz)
{
	DeallocBuf();

	// convert to wstr
	const int nLen = ConvertCharToWChar(sz, (wchar_t**)&m_buf);
	m_len = nLen;
	m_bufLen = sizeof(wchar_t) * (nLen + 1);

	return *this;
}

const wchar_t* NaString::GetBuf()
{
	return reinterpret_cast<wchar_t*>(m_buf);
}

void NaString::AllocBuf(const int len)
{
	if (len < 0)
		assert(0);

	m_buf = new unsigned char[len];
}

void NaString::DeallocBuf(unsigned char* buf)
{
	if (buf == nullptr)
		buf = m_buf;

	if (buf)
	{
		// for debug
		//OutputDebugStringW(L"DeleteBuf: ");
		//OutputDebugStringW((wchar_t*)m_pBuf);
		//OutputDebugStringW(L"\n");

		delete[] buf;
		buf = nullptr;
	}
}

//
// String Array
// 2016.06.17
//

NaString NaStrArray::operator[](const int index)
{
	std::list<NaString>::iterator it = m_strs.begin();
	for (int i = 0; it != m_strs.end(); ++it, ++i)
	{
		if (i == index)
			return (*it);
	}

	return NaString();
}

int NaStrArray::Add(const NaString& str)
{
	m_strs.push_back(str);
	return GetCount();
}

int NaStrArray::Remove(const int index)
{
	std::list<NaString>::iterator it = m_strs.begin();
	for (int i = 0; it != m_strs.end(); ++it, ++i)
	{
		if (i == index)
		{
			m_strs.erase(it);
			return i;
		}
	}
	return -1;
}

int NaStrArray::GetCount() const
{
	return (int)m_strs.size();
}

int NaStrArray::Find(const NaString& str)
{
	std::list<NaString>::iterator it = m_strs.begin();
	for (int i = 0; it != m_strs.end(); ++it, ++i)
	{
		if (str == *it)
			return i;
	}
	return -1;
}

NaString NaStrArray::Join(const wchar_t* const ch)
{
	const int cnt = GetCount();
	NaString str(L"");
	std::list<NaString>::iterator it = m_strs.begin();
	for (int i = 0; it != m_strs.end(); ++it, ++i)
	{
		str += *it;
		if (i < cnt - 1)
			str += ch;
	}

	return str;
}

NaString NaStrArray::Pop()
{
	if (m_strs.empty())
		return L"";

	NaString last = m_strs.back();
	m_strs.pop_back();

	return last;
}
