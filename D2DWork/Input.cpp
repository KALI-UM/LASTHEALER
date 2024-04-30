#include "Input.h"

Input::Input()
{
	bMouseState[LEFTBUTTON] = false;
	bMouseState[RIGHTBUTTON] = false;
	bMouseState[PREVLEFTBUTTON] = false;
	bMouseState[PREVRIGHTBUTTON] = false;
}

Input::~Input()
{
}

void Input::Update()
{
	bMouseState[PREVLEFTBUTTON] = bMouseState[LEFTBUTTON];
	bMouseState[PREVRIGHTBUTTON] = bMouseState[RIGHTBUTTON];
}

void Input::SetMouseState(WPARAM wparam, LPARAM lparam)
{
	bMouseState[LEFTBUTTON] = (MK_LBUTTON == (wparam & MK_LBUTTON));
	bMouseState[RIGHTBUTTON] = (MK_RBUTTON == (wparam & MK_RBUTTON));
	SetMousePoint(lparam);
}

void Input::SetMousePoint(LPARAM lparam)
{
	prevMousePoint = MousePoint;
	MousePoint.x = MAKEPOINTS(lparam).x;
	MousePoint.y = MAKEPOINTS(lparam).y;
}
