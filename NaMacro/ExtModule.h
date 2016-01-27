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
	virtual void Init(v8::Isolate *isolate, v8::Local<v8::ObjectTemplate> &global_template);
	virtual void Release();

	static bool s_bInitTTS;
	static std::vector<ISpVoice*> s_vecVoices;
};

void ConvGMacroToNaMacro(V8_FUNCTION_ARGS);
void TTSSpeak(V8_FUNCTION_ARGS);