#pragma once

#include "NaObject.h"
#include "Common.h"

#include <map>

enum NaControlTypes
{
	NA_CONTROL_UNKNOWN,
	NA_CONTROL_BUTTON,
	NA_CONTROL_EDIT,
	NA_CONTROL_STATIC,
};

class NaWindow;
class NaControl : public NaObject
{
public:
	NaControl();
	~NaControl();

	HWND Create();
	void Create(V8_FUNCTION_ARGS, NaWindow *pParent);
	void Destroy();

	// member
	NaControlTypes m_enType;
	HWND m_hWnd;
	HWND m_hParentWnd;
	int m_x, m_y, m_width, m_height;

	// statics
	static std::map<HWND, Persistent<Function, CopyablePersistentTraits<Function>>> s_mapControlCallback;
	static void OnCommand(Isolate *isolate, HWND hWnd, int nCode);

	// wrap object
	virtual Local<ObjectTemplate> MakeObjectTemplate(Isolate *isolate);
	virtual Global<ObjectTemplate>& GetObjectTemplate() { return s_NaControlTemplate; };
	static Global<ObjectTemplate> s_NaControlTemplate;

	// accessors
	DEFINE_CLASS_ACCESSOR(GetX, SetX);
	DEFINE_CLASS_ACCESSOR(GetY, SetY);
	DEFINE_CLASS_ACCESSOR(GetWidth, SetWidth);
	DEFINE_CLASS_ACCESSOR(GetHeight, SetHeight);
	DEFINE_CLASS_ACCESSOR(GetText, SetText);
	DEFINE_CLASS_ACCESSOR(GetVisible, SetVisible);
	DEFINE_CLASS_ACCESSOR_RO(GetParent);
	DEFINE_CLASS_ACCESSOR(GetImage, SetImage);

	// methods
	DEFINE_CLASS_METHOD(Focus);
};

