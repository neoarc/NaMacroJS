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
	DEFINE_CLASS_METHOD(Include);
	DEFINE_CLASS_METHOD(Sleep);
	DEFINE_CLASS_METHOD(SetInterval);
	DEFINE_CLASS_METHOD(ClearInterval);
	DEFINE_CLASS_METHOD(SetTimeout);
	DEFINE_CLASS_METHOD(Alert);
	DEFINE_CLASS_METHOD(Print);
	DEFINE_CLASS_METHOD(Exit);
	DEFINE_CLASS_METHOD(GetWindow);
	DEFINE_CLASS_METHOD(GetActiveWindow);
	DEFINE_CLASS_METHOD(FindWindows);
	DEFINE_CLASS_METHOD(FindProcesses);
	DEFINE_CLASS_METHOD(FindTrays);
};
