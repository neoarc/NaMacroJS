#include "stdafx.h"
#include "NaKnownFolder.h"

#include <windows.h>
#include <initguid.h>
#include <KnownFolders.h>
#include <ShlObj.h>

NaString NaKnownFolder::MyDocument()
{
	PWSTR path = NULL;
	HRESULT hr = ::SHGetKnownFolderPath(FOLDERID_Documents, 0, NULL, &path);
	if (FAILED(hr))
		return L"";

	NaString folder = path;
	::CoTaskMemFree(path);
	return folder;
}
