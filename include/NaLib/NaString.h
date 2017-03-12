// 2012.11.24 neoarc first draft
// 2016.06.02 neoarc remake 

#pragma once

#include <string>

#include "NaLibDefines.h"

#if defined(USE_V8)
#include "v8.h"
using namespace v8;
#endif

#include <list>

class NaStrArray;
class NaString
{
private:
	union {
		wchar_t* m_pDebugBuf; // for debug
		unsigned char *m_pBuf;
	};
	int m_nBufLen;
	int m_nLen;
	char *m_pCstrBuf;

public:
	// constructor
	NaString();
	NaString(const char *lpsz);
	NaString(const wchar_t *lpsz);
	NaString(const NaString &nstr);
#if defined(USE_V8)
	NaString(Local<String> &str);
	NaString(String::Value &str);
#endif
	// destructor
	~NaString();

	// operator overloading
	const NaString& operator=(const wchar_t *lpsz);
	const NaString& operator=(const NaString &str);

	const NaString& operator+=(const wchar_t *lpsz);
	const NaString& operator+=(const NaString &str);

	bool operator==(const char *lpsz) const;
	bool operator==(const wchar_t *lpsz) const;
	bool operator==(const NaString &str) const;

	bool operator<(const NaString &str) const;

	wchar_t operator[](const int index) const;

	operator const char* () const;
	operator const wchar_t* () const;

	// functions
	void ToLower();
	void ToUpper();
	const NaString& Format(const wchar_t *fmt, ...);
	const NaString& Format(const char *fmt, ...);
	int GetLength() const;
	int Compare(const wchar_t *lpsz) const;
	int CompareNoCase(const wchar_t *lpsz) const;

	int Find(const wchar_t* const ch, const int begin = 0) const;
	NaString Left(const int count) const;
	NaString Mid(const int index, const int count = -1) const;
	NaString Right(const int count) const;
	NaStrArray Split(wchar_t *ch) const;
	int ReplaceAll(wchar_t* from, wchar_t* to);

	// utility
	const NaString& AppendFormat(const wchar_t *fmt, ...);
	const NaString& AppendFormat(const char *fmt, ...);
	const wchar_t* wstr() const;
	const char* cstr();
	wchar_t GetLast() const;
	int ToInt() const;
	float ToFloat() const;
	double ToDouble() const;

	static int ConvertWCharToChar(const wchar_t* wstr, char** str);
	static int ConvertCharToWChar(const char* str, wchar_t** wstr);

protected:
	// internal function
	const NaString& SetBuf(const wchar_t *wsz);
	const NaString& SetBuf(const char *sz);
	const wchar_t *GetBuf();
	void AllocBuf(const int len);
	void DeallocBuf(unsigned char *pBuf = nullptr);
};

class NaStrArray
{
public:
	NaStrArray() {}
	~NaStrArray() { m_Array.clear(); }

	NaString operator [](const int nIndex);

	int Add(const NaString& str);
	int Remove(const int nIndex);
	int GetCount() const;
	int Find(const NaString &str);

	NaString Join(const wchar_t * const ch);
	NaString Pop(); // remove last element

protected:
	std::list<NaString> m_Array;
};