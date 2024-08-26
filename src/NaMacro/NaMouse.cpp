#include "stdafx.h"
#include "NaMouse.h"

#define MOUSECLICK_SLEEP	10

POINT NaMouse::GetPosition()
{
	POINT pt;
	::GetCursorPos(&pt);

	return pt;
}

void NaMouse::SetPosition(int x, int y)
{
	::SetCursorPos(x, y);
}

void NaMouse::Move(int x, int y)
{
	SetPosition(x, y);
}

void NaMouse::Click(std::optional<int> x, std::optional<int> y)
{
	if (x.has_value() && y.has_value())
	{
		Move(x.value(), y.value());
	}

	INPUT input;
	ZeroMemory(&input, sizeof(INPUT));
	input.type = INPUT_MOUSE;
	input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
	::SendInput(1, &input, sizeof(INPUT));
	::Sleep(MOUSECLICK_SLEEP);

	input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
	::SendInput(1, &input, sizeof(INPUT));
}

void NaMouse::LbuttonDown(std::optional<int> x, std::optional<int> y)
{
	if (x.has_value() && y.has_value())
	{
		Move(x.value(), y.value());
	}

	INPUT input;
	ZeroMemory(&input, sizeof(INPUT));
	input.type = INPUT_MOUSE;
	input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
	::SendInput(1, &input, sizeof(INPUT));
}

void NaMouse::LbuttonUp(std::optional<int> x, std::optional<int> y)
{
	if (x.has_value() && y.has_value())
	{
		Move(x.value(), y.value());
	}

	INPUT input;
	ZeroMemory(&input, sizeof(INPUT));
	input.type = INPUT_MOUSE;
	input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
	::SendInput(1, &input, sizeof(INPUT));
}

void NaMouse::RbuttonDown(std::optional<int> x, std::optional<int> y)
{
	if (x.has_value() && y.has_value())
	{
		Move(x.value(), y.value());
	}

	INPUT input;
	ZeroMemory(&input, sizeof(INPUT));
	input.type = INPUT_MOUSE;
	input.mi.dwFlags = MOUSEEVENTF_RIGHTDOWN;
	::SendInput(1, &input, sizeof(INPUT));
}

void NaMouse::RbuttonUp(std::optional<int> x, std::optional<int> y)
{
	if (x.has_value() && y.has_value())
	{
		Move(x.value(), y.value());
	}

	INPUT input;
	ZeroMemory(&input, sizeof(INPUT));
	input.type = INPUT_MOUSE;
	input.mi.dwFlags = MOUSEEVENTF_RIGHTUP;
	::SendInput(1, &input, sizeof(INPUT));
}

void NaMouse::WheelDown(std::optional<int> x, std::optional<int> y)
{
	if (x.has_value() && y.has_value())
	{
		Move(x.value(), y.value());
	}

	INPUT input;
	ZeroMemory(&input, sizeof(INPUT));
	input.type = INPUT_MOUSE;
	input.mi.dwFlags = MOUSEEVENTF_WHEEL;
	// A positive value indicates that the wheel was rotated forward, away from the user;
	// a negative value indicates that the wheel was rotated backward, toward the user.
	// One wheel click is defined as WHEEL_DELTA, which is 120.
	input.mi.mouseData = (DWORD)-WHEEL_DELTA;
	::SendInput(1, &input, sizeof(INPUT));
}

void NaMouse::WheelUp(std::optional<int> x, std::optional<int> y)
{
	if (x.has_value() && y.has_value())
	{
		Move(x.value(), y.value());
	}

	INPUT input;
	ZeroMemory(&input, sizeof(INPUT));
	input.type = INPUT_MOUSE;
	input.mi.dwFlags = MOUSEEVENTF_WHEEL;
	// A positive value indicates that the wheel was rotated forward, away from the user;
	// a negative value indicates that the wheel was rotated backward, toward the user.
	// One wheel click is defined as WHEEL_DELTA, which is 120.
	input.mi.mouseData = (DWORD)WHEEL_DELTA;
	::SendInput(1, &input, sizeof(INPUT));
}

void NaMouse::PostMove(int x, int y)
{
	POINT pt = { x, y };
	HWND hWnd = WindowFromPoint(pt);
	::ScreenToClient(hWnd, &pt);

	PostMessage(hWnd, WM_MOUSEMOVE, MK_LBUTTON, MAKELPARAM(pt.x, pt.y));
}

void NaMouse::PostClick(int x, int y)
{
	PostLbuttonDown(x, y);
	::Sleep(MOUSECLICK_SLEEP);

	PostLbuttonUp(x, y);
}

void NaMouse::PostLbuttonDown(int x, int y)
{
	POINT pt = { x, y };
	HWND hWnd = WindowFromPoint(pt);
	::ScreenToClient(hWnd, &pt);

	PostMessage(hWnd, WM_LBUTTONDOWN, MK_LBUTTON, MAKELPARAM(pt.x, pt.y));
}

void NaMouse::PostLbuttonUp(int x, int y)
{
	POINT pt = { x, y };
	HWND hWnd = WindowFromPoint(pt);
	::ScreenToClient(hWnd, &pt);

	PostMessage(hWnd, WM_LBUTTONUP, MK_LBUTTON, MAKELPARAM(pt.x, pt.y));
}

void NaMouse::PostRbuttonDown(int x, int y)
{
	POINT pt = { x, y };
	HWND hWnd = WindowFromPoint(pt);
	::ScreenToClient(hWnd, &pt);

	PostMessage(hWnd, WM_RBUTTONDOWN, MK_RBUTTON, MAKELPARAM(pt.x, pt.y));
}

void NaMouse::PostRbuttonUp(int x, int y)
{
	POINT pt = { x, y };
	HWND hWnd = WindowFromPoint(pt);
	::ScreenToClient(hWnd, &pt);

	PostMessage(hWnd, WM_RBUTTONUP, MK_RBUTTON, MAKELPARAM(pt.x, pt.y));
}
