#pragma once

#include "NaString.h"

class NaUrl
{
public:
	NaUrl();
	~NaUrl();

	void SetBase(NaString base);
	void SetUrl(NaString url);
	NaString GetFullUrl();

public:
	static NaString GetMyDocumentDirectory();

protected:
	NaString m_strBase;
	NaString m_strUrl;
};

