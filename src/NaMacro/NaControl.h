#pragma once

#include "JsObjectBase.h"
#include "V8Wrap.h"

#include <map>

#include "NaWindow.h"

enum NaControlTypes
{
	NA_CONTROL_UNKNOWN,
	NA_CONTROL_BUTTON,
	NA_CONTROL_EDIT,
	NA_CONTROL_STATIC,
};

class NaControl : public NaWindow
{
public:
	NaControl();
	~NaControl();

	HWND Create();
	void Create(V8_FUNCTION_ARGS, NaWindow *pParent);
	void Destroy();

	// Methods
	void SetFocus();

	// Member
	NaControlTypes m_enType;
	HWND m_hParentWnd;

	// Static
};

