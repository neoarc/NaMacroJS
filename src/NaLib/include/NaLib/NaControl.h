#pragma once

#include <map>

#include "NaWindow.h"

enum NaControlTypes
{
	NA_CONTROL_UNKNOWN = ((int)NA_WINDOW_LAST + 1),

	NA_CONTROL_BUTTON,
	NA_CONTROL_EDIT,
	NA_CONTROL_STATIC,
};

class NaControl : public NaWindow
{
public:
	NaControl();
	virtual ~NaControl();

	HWND Create();
	void Destroy();

	// Methods
	void SetFocus();
	virtual void SetVisible(bool bVisible);

	// Member
	HWND m_hParentWnd;

	// Static
};

