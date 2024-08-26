#include "stdafx.h"
#include "JsGlobalExternal.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>

using namespace std;

bool JaGlobalExternal::s_bInitTTS = false;
vector<ISpVoice*> JaGlobalExternal::s_vecVoices;

void JaGlobalExternal::Create(Isolate * isolate, Local<ObjectTemplate>& global_template)
{
	// methods
	ADD_GLOBAL_METHOD(convGMacroToNaMacro);
	ADD_GLOBAL_METHOD(ttsSpeak);
}

void JaGlobalExternal::Init(Isolate * /*isolate*/, Local<ObjectTemplate>& /*global_template*/)
{
	// TODO make extapi object
	// TODO bind apis to extapi object
}

void JaGlobalExternal::Release()
{
	if (JaGlobalExternal::s_bInitTTS == true) 
	{
		for_each(s_vecVoices.begin(), s_vecVoices.end(),
			[&](ISpVoice* v) { v->Release(); } );

		JaGlobalExternal::s_vecVoices.clear();
		::CoUninitialize();
	}
}

// description: Convert GMacro data to NaMacro script
// syntax:		convGMacroToNaMacro(filename)
void JaGlobalExternal::method_convGMacroToNaMacro(V8_METHOD_ARGS)
{
	printf("ConvGMacroToNaMacro\n");

	// Load GMacro Data
	String::Utf8Value arg0(args[0]);
	char *filename = *arg0;

	ifstream file;
	file.open(filename, ios::binary || ios::in || ios::ate);
	if (!file.is_open())
	{
		// Local<String>();
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
	};

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

	for_each(MacroDataList.begin(), MacroDataList.end(),
		[&](MacroData m)
		{
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
					strValue = strValue.substr(0, strValue.length() - 2); // cut '��'
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
	);

	V8_METHOD_RET(strNaScript.data());
}

// description: text to speech
// syntax:		ttsSpeak(text, [async=false])
void JaGlobalExternal::method_ttsSpeak(V8_METHOD_ARGS)
{
	if (JaGlobalExternal::s_bInitTTS == false)
	{
		CoInitialize(0);
		JaGlobalExternal::s_bInitTTS = true;
	}

	ISpVoice *pVoice = NULL;
	HRESULT hr = CoCreateInstance(CLSID_SpVoice, NULL, CLSCTX_ALL, IID_ISpVoice, (void **)&pVoice);

	if (SUCCEEDED(hr))
	{
		String::Utf8Value str(args[0]);
		bool bAsync = false;
		if (args.Length() >= 2)
		{
			bAsync = args[1]->BooleanValue();
		}

		int nChars = MultiByteToWideChar(CP_ACP, 0, *str, -1, NULL, 0);
		const WCHAR *pwcsName;
		pwcsName = new WCHAR[nChars];
		MultiByteToWideChar(CP_ACP, 0, *str, -1, (LPWSTR)pwcsName, nChars);

		DWORD dwFlag = SPF_IS_XML;
		if (bAsync)
			dwFlag |= SPF_ASYNC;

		hr = pVoice->Speak(pwcsName, dwFlag, NULL);

		JaGlobalExternal::s_vecVoices.push_back(pVoice);
	}
}
