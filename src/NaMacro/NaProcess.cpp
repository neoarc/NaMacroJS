#include "stdafx.h"
#include "NaProcess.h"

#include <NaLib/NaDebug.h>

#include <TlHelp32.h>

NaProcess::NaProcess()
{
	m_hProcess = nullptr;
}

NaProcess::~NaProcess()
{
}

void NaProcess::Terminate()
{
	if (m_hProcess)
		KillProcess(m_hProcess);
}

void NaProcess::FindProcesses(const wchar_t * name, FindProcessInfo & info)
{
	HANDLE hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPALL, NULL);
	
	PROCESSENTRY32 entry;
	entry.dwSize = sizeof(entry);

	BOOL hRes = Process32First(hSnapShot, &entry);
	while (hRes)
	{
		NaString strFound = entry.szExeFile;
		if (strFound.Find(name) >= 0)
		{
			// #CHECK rights = terminate ...
			HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, 0, (DWORD)entry.th32ProcessID);
			info.name = const_cast<wchar_t*>(name);
			info.foundlist.push_back(hProcess);
		}

		hRes = Process32Next(hSnapShot, &entry);
	}

	CloseHandle(hSnapShot);
}

void NaProcess::KillProcess(HANDLE hProcess)
{
	if (hProcess != nullptr) // && entry.th32ProcessID != GetCurrentProcessId())
	{
		TerminateProcess(hProcess, 9);
		CloseHandle(hProcess);
	}
}
