// 2012.11.24 neoarc first draft
// 2016.06.02 neoarc remake 

#pragma once

#include <string.h>
#include <Windows.h>

#define NASTRING_FORMAT_BUFFER_SIZE	65535

class NaString
{
private:
	unsigned char *m_pBuf;
	int m_nBufLen;
	int m_nLen;

public:
	// constructor
	NaString();
	NaString(const wchar_t *lpsz);

	// copy constructor
	NaString(NaString &nstr);
	
	// assigning
	const NaString& operator=(const wchar_t *lpsz);
	const NaString& operator=(NaString &str);

	const NaString& operator+=(const wchar_t *lpsz);
	const NaString& operator+=(NaString &str);

	// destructor
	~NaString();

	// operator overloading
	operator const char* () const;
	operator const wchar_t* () const;

	// functions
	void ToLower();
	void ToUpper();
	const NaString& Format(const wchar_t *fmt, ...);
	const NaString& Format(const char *fmt, ...);
	int GetLength();
	int Compare(const wchar_t *lpsz);
	int CompareNoCase(const wchar_t *lpsz);

	// utility
	const NaString& AppendFormat(const wchar_t *fmt, ...);
	const NaString& AppendFormat(const char *fmt, ...);
	const wchar_t* wstr();
	
protected:
	// internal function
	const NaString& SetBuf(const wchar_t *wsz, int len = -1);
	const wchar_t *GetBuf();
	void AllocBuf(int len);
};
