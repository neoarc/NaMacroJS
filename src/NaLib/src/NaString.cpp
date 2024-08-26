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
	SetBuf((wchar_t*)nstr.m_pBuf);
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
	if (m_szBuf != nullptr)
	{
		delete[] m_szBuf;
		m_szBuf = nullptr;
	}
}

const NaString& NaString::operator=(const wchar_t* lpsz)
{
	SetBuf((wchar_t*)lpsz);
	return *this;
}

const NaString& NaString::operator=(const NaString& str)
{
	SetBuf((wchar_t*)str.m_pBuf);
	return *this;
}

const NaString& NaString::operator+=(const wchar_t* lpsz)
{
	if (m_pBuf == nullptr || m_nLen == 0)
		SetBuf(lpsz);
	else
		Format(L"%s%s", (wchar_t*)m_pBuf, lpsz);
	return *this;
}

const NaString& NaString::operator+=(const NaString& str)
{
	if (m_pBuf == nullptr || m_nLen == 0)
		SetBuf(str.wstr());
	else
		Format(L"%s%s", (wchar_t*)m_pBuf, str.wstr());
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
	return wcscmp((wchar_t*)m_pBuf, (wchar_t*)str.m_pBuf) < 0;
}

wchar_t NaString::operator[](const int index) const
{
	if (index >= 0 && index < m_nLen)
		return *((wchar_t*)m_pBuf + index);

	return L'\0';
}

NaString::operator const char* () const
{
	char *sz = nullptr;
	ConvertWCharToChar((wchar_t*)m_pBuf, &sz);

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
#ifdef WIN32
	_wcslwr_s((wchar_t*)m_pBuf, m_nLen + 1);
#else
#endif
}

void NaString::ToUpper()
{
#ifdef WIN32
	_wcsupr_s((wchar_t*)m_pBuf, m_nLen + 1);
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
	int nLen = ConvertCharToWChar(buf, (wchar_t**)&m_pBuf);
	m_nLen = nLen - 1;
	m_nBufLen = sizeof(wchar_t) * (nLen);

	return *this;
}

int NaString::GetLength() const
{
	return m_nLen;
}

int NaString::Compare(const wchar_t* lpsz) const
{
	/*
	int arglen = wcslen(lpsz);
	int minlen = m_nLen < arglen ? m_nLen : arglen;
	*/

	if (m_pBuf == nullptr)
		return wcscmp((wchar_t*)L"", lpsz);

	return wcscmp((wchar_t*)m_pBuf, lpsz);
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
	if (m_nLen < itemlen)
		return -1;

	for (int i = begin; i < m_nLen - itemlen + 1; i++)
	{
		bool bMatch = true;
		for (int j = 0; j < itemlen; j++)
		{
			if (*((wchar_t*)m_pBuf + i + j) != *(ch + j))
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

	if (count >= m_nLen)
	{
		NaString strRet((wchar_t*)this->m_pBuf);
		return strRet;
	}

	NaString strRet;
	unsigned char *buf;
	int nNewLen = count;
	int nNewBufLen = sizeof(wchar_t) * (nNewLen + 1);
	buf = new unsigned char[nNewBufLen];

	memcpy(buf, this->m_pBuf, nNewBufLen);
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
		NaString ret((wchar_t*)this->m_pBuf);
		return ret;
	}

	int length = count;
	if (length == -1)
		length = m_nLen - idx;
	if (idx + length > m_nLen)
		length = m_nLen - idx;

	int nNewBufLen = sizeof(wchar_t) * (length + 1);
	unsigned char *buf = new unsigned char[nNewBufLen];

	memcpy(buf, (wchar_t*)(this->m_pBuf) + idx, nNewBufLen);
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

	if (count >= m_nLen)
	{
		NaString strRet((wchar_t*)this->m_pBuf);
		return strRet;
	}

	int index = m_nLen - count;

	int nNewBufLen = sizeof(wchar_t) * (count + 1);
	unsigned char *buf = new unsigned char[nNewBufLen];

	memcpy(buf, (wchar_t*)(this->m_pBuf) + index, nNewBufLen);
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
			temp += Mid(begin, m_nLen - begin);
			break;
		}

		temp += Mid(begin, index - begin);
		temp += to;

		begin = index + (int)wcslen(from);
	}

	SetBuf(temp.wstr());

	return 0;
}

void NaString::Trim()
{
	// find left
	int left;
	for (left = 0; left < m_nLen; left++)
	{
		if (*((wchar_t*)m_pBuf + left) == L' ')
			continue;
		break;
	}

	// find right
	int right;
	for (right = m_nLen - 1; right > left; right--)
	{
		if (*((wchar_t*)m_pBuf + right) == L' ')
			continue;
		break;
	}

	if (left == right)
		SetBuf(L"");
	else
		SetBuf(Mid(left, right - left + 1).wstr());
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

	Format(L"%s%s", (wchar_t*)m_pBuf, buf);
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
	if (m_pBuf == nullptr)
		return L"";
	return (const wchar_t*)m_pBuf;
}

const char* NaString::cstr()
{
	if (m_szBuf != nullptr)
	{
		delete[] m_szBuf;
		m_szBuf = nullptr;
	}

	ConvertWCharToChar((wchar_t*)m_pBuf, &m_szBuf);

	return m_szBuf;
}

wchar_t NaString::GetLast() const
{
	if (m_nLen <= 0)
		return 0;

	return *((wchar_t*)m_pBuf + m_nLen);
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

	//NaDebugOut(L"* ConvWCharToChar\n* (1) %s\n", wstr);
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
	m_pBuf = nullptr;
	m_nBufLen = 0;
	m_nLen = 0;
	m_szBuf = nullptr;
}

const NaString& NaString::SetBuf(const wchar_t* lpsz)
{
	unsigned char* pCurBuf = m_pBuf;

	if (lpsz == nullptr)
	{
		m_nLen = 0;
		m_nBufLen = (int)(sizeof(wchar_t) * (m_nLen + 1));
		AllocBuf(m_nBufLen);
		*((wchar_t*)m_pBuf + m_nLen) = 0;
	}
	else
	{
		m_nLen = (int)wcslen(lpsz);
		m_nBufLen = (int)(sizeof(wchar_t) * (m_nLen + 1));

		AllocBuf(m_nBufLen);
		memcpy(m_pBuf, lpsz, m_nBufLen);

		*((wchar_t*)m_pBuf + m_nLen) = 0;
	}

	delete pCurBuf;

	return *this;
}

const NaString& NaString::SetBuf(const char* sz)
{
	DeallocBuf();

	// convert to wstr
	const int nLen = ConvertCharToWChar(sz, (wchar_t**)&m_pBuf);
	m_nLen = nLen;
	m_nBufLen = sizeof(wchar_t) * (nLen + 1);

	return *this;
}

const wchar_t* NaString::GetBuf()
{
	return reinterpret_cast<wchar_t*>(m_pBuf);
}

void NaString::AllocBuf(const int len)
{
	if (len < 0)
		assert(0);

	m_pBuf = new unsigned char[len];
}

void NaString::DeallocBuf(unsigned char* buf)
{
	if (buf == nullptr)
		buf = m_pBuf;

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
