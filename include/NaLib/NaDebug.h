#pragma once

#define NaDebugOut		NaDebug::Out
#define NaDebugOutA		NaDebug::OutA

namespace NaDebug
{
	void OutA(const char* pszFormat, ...);
	void Out(const wchar_t* pszFormat, ...);
};
