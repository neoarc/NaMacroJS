#pragma once

#include <v8.h>
#include "Common.h"
#include "ModuleBase.h"

#include <vector>

#include "sapi.h"           // SAPI includes
#include "sphelper.h"
#include "spuihelp.h"

class NaExtModule : public ModuleBase
{
public:
	virtual void Create(Isolate *isolate, Local<ObjectTemplate>& global_template);
	virtual void Init(Isolate *isolate, Local<ObjectTemplate> &global_template);
	virtual void Release();

	static bool s_bInitTTS;
	static std::vector<ISpVoice*> s_vecVoices;

	// methods
	DEFINE_CLASS_METHOD(ConvGMacroToNaMacro);
	DEFINE_CLASS_METHOD(TTSSpeak);
};
