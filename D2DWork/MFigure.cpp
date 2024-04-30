#include "framework.h"
#include "MFigure.h"

MFigure::MFigure()
{
}

MFigure::~MFigure()
{
}

void MFigure::Initialize()
{
}

void MFigure::Draw()
{
	if (pInput->bMouseState[PREVLEFTBUTTON] == false && pInput->bMouseState[LEFTBUTTON])
	{
		prevx = pInput->MousePoint.x;
		prevy = pInput->MousePoint.y;
	}
	else if (pInput->bMouseState[LEFTBUTTON])
	{
		if (bRect)
			pD2DEngine->DrawRect(prevx, pInput->MousePoint.x, prevy, pInput->MousePoint.y, Colors[COLOR_PEN], 1.0f);
		else
			pD2DEngine->DrawEllipse({ (prevx + pInput->MousePoint.x) / 2,(prevy+ pInput->MousePoint.y) / 2 }, (prevx - pInput->MousePoint.x)/2, (prevy - pInput->MousePoint.y)/2, Colors[COLOR_PEN], 1.0f);
	}
	else if (pInput->bMouseState[PREVLEFTBUTTON] && pInput->bMouseState[LEFTBUTTON] == false)
	{
		if (bRect)
		{
			Obj::Rectangle* r = new Obj::Rectangle(Colors[COLOR_PEN], Colors[COLOR_FILL], 1.0f, { prevx, prevy }, { pInput->MousePoint.x, pInput->MousePoint.y });
			pD2DEngine->SaveDrawing(r);
		}
		else
		{
			Obj::Ellipse* e = new Obj::Ellipse(Colors[COLOR_PEN], Colors[COLOR_FILL], 1.0f, { (prevx + pInput->MousePoint.x) / 2,(prevy + pInput->MousePoint.y) / 2 }, (prevx - pInput->MousePoint.x) / 2, (prevy - pInput->MousePoint.y) / 2);
			pD2DEngine->SaveDrawing(e);
		}
	}
}
