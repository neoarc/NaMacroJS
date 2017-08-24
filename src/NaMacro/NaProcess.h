#pragma once

#include "V8Wrap.h"
#include "JsObjectBase.h"

#include <Windows.h>
#include <map>

class NaProcess
{
public:
	NaProcess();
	NaProcess(HANDLE hHandle);
	virtual ~NaProcess();

	// members
	NaString m_strName;
	HANDLE m_hProcess;

	void Terminate();

	// internal struct
	struct FindProcessInfo {
		wchar_t* name;
		std::list<HANDLE> foundlist;
	};

	// static
	static void FindProcesses(const wchar_t *name, FindProcessInfo &info);
	static void KillProcess(HANDLE hProcess);
	static NaProcess* GetProcess(HANDLE hProcess);
};