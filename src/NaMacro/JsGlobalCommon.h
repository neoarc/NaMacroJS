#pragma once

#include "V8Wrap.h"
#include "ModuleBase.h"

#include "NaWindow.h"
#include <map>

class JsGlobalCommon : public ModuleBase
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

	// method helper
	static bool IncludeBase(V8_METHOD_ARGS, NaString strFullPath);

	// accessors
	DEFINE_CLASS_PROPERTY_RO(pcname);
	DEFINE_CLASS_PROPERTY_RO(username);

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
	DEFINE_CLASS_METHOD(fetch);
	DEFINE_CLASS_METHOD(exit);
	DEFINE_CLASS_METHOD(getWindow);
	DEFINE_CLASS_METHOD(getActiveWindow);
	DEFINE_CLASS_METHOD(findWindows);
	DEFINE_CLASS_METHOD(findProcesses);
	DEFINE_CLASS_METHOD(findTrays);

	// system methods
	DEFINE_CLASS_METHOD(execute);
	DEFINE_CLASS_METHOD(executeSync);
};
