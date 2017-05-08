#include "stdafx.h"
#include "NaUrl.h"

#include <list>

NaUrl::NaUrl()
{
}

NaUrl::~NaUrl()
{
}

void NaUrl::SetBase(NaString base)
{
	m_base = NaString(base);
}

void NaUrl::SetUrl(NaString url)
{
	m_url = NaString(url);
}

NaString NaUrl::GetFullUrl()
{
	bool bRelative = false;
	int nIdx = m_url.Find(L":");
	if (nIdx > 0)
	{
		// This is full path, ignore base path
		NaString strUrl = m_url;
		NaStrArray arUrl = strUrl.Split(L"/");
		NaStrArray arFull;

		for (int i = 0; i < arUrl.GetCount(); i++)
		{
			if (arUrl[i] == "/" && i == 0)
			{
				// TODO check; Is this possible case?
				continue;
			}
			if (arUrl[i] == ".")
				continue;
			if (arUrl[i] == "..")
			{
				arFull.Pop();
				continue;
			}

			arFull.Add(arUrl[i]);
		}

		return arFull.Join(L"/").wstr();
	}
	else
	{
		bRelative = true;

		NaString strUrl = m_url;
		NaString strBase = m_base;
		strUrl.ReplaceAll(L"\\", L"/");
		strBase.ReplaceAll(L"\\", L"/");

		NaStrArray arUrl = strUrl.Split(L"/");
		NaStrArray arBase = strBase.Split(L"/");
		if (strBase.GetLast() != L'/')
		{
			// Maybe filename
			arBase.Pop();
		}

		for (int i = 0; i < arUrl.GetCount(); i++)
		{
			if (arUrl[i] == "/" && i == 0)
			{
				// TODO this is root
				continue;
			}
			if (arUrl[i] == ".")
				continue;
			if (arUrl[i] == "..")
			{
				arBase.Pop();
				continue;
			}

			arBase.Add(arUrl[i]);
		}

		return arBase.Join(L"/").wstr();
	}
}
