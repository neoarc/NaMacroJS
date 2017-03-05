//
// Url(or Directory Path) parser
// 2016.06.15
//

#include "NaUrl.h"

// #include "../NaMacro/Common.h"

#include <list>

NaUrl::NaUrl()
{
}

NaUrl::~NaUrl()
{
}

void NaUrl::SetBase(NaString base)
{
	m_strBase = NaString(base);
}

void NaUrl::SetUrl(NaString url)
{
	m_strUrl = NaString(url);
}

NaString NaUrl::GetFullUrl()
{
	bool bRelative = false;
	int nIdx = m_strUrl.Find(L":");
	if (nIdx > 0)
	{
		// This is full path, ignore base path
		NaString strUrl = m_strUrl;
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

		NaString strUrl = m_strUrl;
		NaString strBase = m_strBase;
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
