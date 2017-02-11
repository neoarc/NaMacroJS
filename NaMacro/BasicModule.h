#pragma once

#include <v8.h>
#include "Common.h"
#include "ModuleBase.h"

#include "NaWindow.h"
#include <map>

class NaBasicModule : public ModuleBase
{
public:
	virtual void Create(Isolate *isolate, Local<ObjectTemplate>& global_template);
	virtual void Init(Isolate *isolate, Local<ObjectTemplate> &global_template);
	virtual void Release();

	// statics
	static void OnTimer(Isolate *isolate, int nTimerID);
	static NaWindow* s_pTimerWindow;
	static std::map<int, Persistent<Function, CopyablePersistentTraits<Function>>> s_mapIntervalCallback;
	static std::map<int, Persistent<Function, CopyablePersistentTraits<Function>>> s_mapTimeoutCallback;
	static int s_nTimerID;

	// methods
	DEFINE_CLASS_METHOD(include);
	DEFINE_CLASS_METHOD(sleep);
	DEFINE_CLASS_METHOD(setInterval);
	DEFINE_CLASS_METHOD(clearInterval);
	DEFINE_CLASS_METHOD(setTimeout);
	DEFINE_CLASS_METHOD(alert);
	DEFINE_CLASS_METHOD(confirm);
	DEFINE_CLASS_METHOD(prompt);
	DEFINE_CLASS_METHOD(print);
	DEFINE_CLASS_METHOD(notify);
	DEFINE_CLASS_METHOD(exit);
	DEFINE_CLASS_METHOD(getWindow);
	DEFINE_CLASS_METHOD(getActiveWindow);
	DEFINE_CLASS_METHOD(findWindows);
	DEFINE_CLASS_METHOD(findProcesses);
	DEFINE_CLASS_METHOD(findTrays);
};
