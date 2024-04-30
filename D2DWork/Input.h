#pragma once
#include <windows.h>

#define LEFTBUTTON 0
#define RIGHTBUTTON 1
#define PREVLEFTBUTTON 2
#define PREVRIGHTBUTTON 3


class Input
{
	struct Point
	{
		long x; long y;
	};

public: 
	Input();
	~Input();

	void Update();

	void SetMouseState(WPARAM wparam, LPARAM lparam);
	void SetMousePoint(LPARAM lparam);
	
	Point MousePoint;
	Point prevMousePoint;
	bool bMouseState[4];

};

