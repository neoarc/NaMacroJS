#include "stdafx.h"

#include "ExtAPI.h"

#include <fstream>
#include <iostream>
#include <vector>
#include <sstream>

//#using <mscorlib.dll>
#include "sapi.h"           // SAPI includes
#include "sphelper.h"
#include "spuihelp.h"

using namespace std;
//using namespace SpeechLib;

void InitExtAPI(v8::Isolate *isolate, v8::Local<v8::ObjectTemplate>& global_template)
{
#define ADD_GLOBAL_API(_js_func, _c_func) \
	global_template->Set( \
		v8::String::NewFromUtf8(isolate, #_js_func, v8::NewStringType::kNormal).ToLocalChecked(), \
		v8::FunctionTemplate::New(isolate, _c_func) \
	);

	ADD_GLOBAL_API(convGMacroToNaMacro, ConvGMacroToNaMacro);
	ADD_GLOBAL_API(ttsSpeak, TTSSpeak);

	// TODO make extapi object
	// TODO bind apis to extapi object
}

// description: Convert GMacro data to NaMacro script
// syxtax:		convGMacroToNaMacro(filename)
void ConvGMacroToNaMacro(V8_FUNCTION_ARGS)
{
	printf("ConvGMacroToNaMacro\n");
	v8::Isolate *isolate = args.GetIsolate();

	// Load GMacro Data
	v8::String::Utf8Value arg0(args[0]);
	char *filename = *arg0;

	ifstream file;
	file.open(filename, ios::binary || ios::in || ios::ate);
	if (!file.is_open())
	{
		// v8::Local<v8::String>();
		return;
	}

#define MAKE_BUFFER(_name) char *_name = new char[1024]; memset(_name, 0, 1024);
	MAKE_BUFFER(buf);

#define SHOW_PROPERTY(_prop) \
	printf("   %s: ", #_prop); \
	for (int i=0; i<sizeof(m._prop); i++) \
		printf("%02x ", (unsigned char)m._prop[i]); \
	printf("\n");

	// macro types
	enum _MACRO_TYPES {
		key = 1,
		mousemove = 2,
		mouseclick = 3,
		mouselbuttondown = 4,
		mouselbuttonup = 5,
		delay = 6,
		mouserbuttonclick = 7,
		mouserbuttondown = 8
	} MACRO_TYPES;

	// macro data structure
	union MacroData {
		struct 
		{			
			char _a[4];
			char name[25];
			char keycode[1];
			char _b[7];
			char numname[30];
			char type[1]; // MACRO_TYPES

			char _c[7];
			char _d[2];
			char _e[2];
		};
		struct
		{
			char dump[79];
		};
	};

	vector<MacroData> MacroDataList;
	while (true)
	{
		MacroData m;
		file.read(buf, sizeof(MacroData));
		if (file.eof())
			break;

		memcpy(&m, buf, sizeof(MacroData));
		MacroDataList.push_back(m);

		//SHOW_PROPERTY(dump);
	}

	delete[] buf;

	// Create NaMacro Script
	string strNaScript = "// Converted NaMacro Script from GMacro\n";
	vector<MacroData>::iterator it = MacroDataList.begin();
	for (; it != MacroDataList.end(); ++it)
	{
		MacroData m = *it;
		printf("[%s][type: %x]\n", m.numname, (unsigned char)m.type[0]);

		string strNumName = m.numname;
		int nBegin = strNumName.find("(");
		int nEnd = strNumName.find(")");
		string strValue = strNumName.substr(nBegin + 1, nEnd - nBegin - 1);

		string str = "";
		switch (m.type[0])
		{
		case key:
			str += "// keyboard command // not support yet\n";
			break;
		case mousemove:
		{
			str += "mouse.move(";
			str += strValue;
			str += ");\n";
			break;
		}
		case mouseclick:
			str += "mouse.click();\n";
			break;
		case mouselbuttondown:
			str += "mouse.lbuttonDown();\n";
			break;
		case mouselbuttonup:
			str += "mouse.lbuttonUp();\n";
			break;
		case delay:
		{
			if (nBegin != -1 && nEnd != -1 && strValue.length() > 0)
			{
				strValue = strValue.substr(0, strValue.length() - 2); // cut '√ '
				double dDelay = atof(strValue.c_str());

				std::ostringstream strDelay;
				strDelay << (dDelay * 1000);
				
				str += "sleep(";
				str += strDelay.str();
				str += ");\n";
			}
			break;
		}
		case mouserbuttonclick:
			str += "// mouse.rbuttonClick(); // not support yet :p\n";
			break;
		case mouserbuttondown:
			str += "mouse.rbuttonDown();\n";
			break;
		default:
			str += "// unknown command // not support yet\n";
			break;
		}

		strNaScript += str;
	}

	v8::Local<v8::String> result = v8::String::NewFromUtf8(isolate, strNaScript.data(), v8::NewStringType::kNormal, strNaScript.length()).ToLocalChecked();
	args.GetReturnValue().Set(result);
}

// description: text to speech
// syntax:		ttsSpeak(text)
void TTSSpeak(V8_FUNCTION_ARGS)
{
	CoInitialize(0);

	ISpVoice * pVoice = NULL;
	HRESULT hr = CoCreateInstance(CLSID_SpVoice, NULL, CLSCTX_ALL, IID_ISpVoice, (void **)&pVoice);

	if (SUCCEEDED(hr))
	{
		v8::String::Utf8Value str(args[0]);

		int nChars = MultiByteToWideChar(CP_ACP, 0, *str, -1, NULL, 0);
		const WCHAR *pwcsName;
		pwcsName = new WCHAR[nChars];
		MultiByteToWideChar(CP_ACP, 0, *str, -1, (LPWSTR)pwcsName, nChars);

		hr = pVoice->Speak(pwcsName, SPF_IS_XML, NULL);
		pVoice->Release();
		pVoice = NULL;
	}

	::CoUninitialize();
}