#pragma once

enum ActionTypes
{
	ACTION_MOUSEBEGIN,
	ACTION_MOUSEMOVE,
	ACTION_MOUSELBUTTONDOWN,
	ACTION_MOUSELBUTTONUP,
	ACTION_MOUSERBUTTONDOWN,
	ACTION_MOUSERBUTTONUP,
	ACTION_MOUSELAST,

	ACTION_KEYBEGIN,
	ACTION_KEYDOWN,
	ACTION_KEYUP,
	ACTION_KEYLAST,

	ACTION_WINDOWINFO,
};

class ActionRecord
{
public:
	ActionTypes enType;
	DWORD dwTimeStamp;
};

class MouseActionRecord : public ActionRecord
{
public:
	POINT ptPos;
	POINT ptRelativePos;
};

class KeyActionRecord : public ActionRecord
{
public:
	int nKeyCode;
};

class WindowInfoActionRecord : public ActionRecord
{
public:
	CString strText;
	CString strClass;
	RECT rcRect;
};