#include "framework.h"
#include "MPen.h"

MPen::MPen()
	:size(10)
{
}

MPen::~MPen()
{
}

void MPen::Initialize()
{

}

void MPen::Draw()
{
	pD2DEngine->DrawCircle({ pInput->MousePoint.x, pInput->MousePoint.y }, size , Colors[COLOR_PEN], 1.0f);

	if ((pInput->bMouseState[PREVLEFTBUTTON] == false && pInput->bMouseState[LEFTBUTTON]) || (pInput->bMouseState[PREVLEFTBUTTON] && pInput->bMouseState[LEFTBUTTON] == false))
	{
		Obj::Ellipse* e = new Obj::Ellipse(Colors[COLOR_PEN], Colors[COLOR_PEN], 1.0f, { pInput->MousePoint.x, pInput->MousePoint.y }, size / 2, size / 2);
		pD2DEngine->SaveDrawing(e);
	}
	
	if (pInput->bMouseState[LEFTBUTTON])
	{
		Obj::Line* l = new Obj::Line(Colors[COLOR_PEN], Colors[COLOR_PEN], 1.0f, { prevx, prevy }, { pInput->MousePoint.x, pInput->MousePoint.y }, size);
		pD2DEngine->SaveDrawing(l);
		prevx = pInput->MousePoint.x; prevy = pInput->MousePoint.y;
	}
	else
	{
		prevx = pInput->MousePoint.x; prevy = pInput->MousePoint.y;
	}

}
