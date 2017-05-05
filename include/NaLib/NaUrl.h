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

protected:
	NaString m_base;
	NaString m_url;
};

