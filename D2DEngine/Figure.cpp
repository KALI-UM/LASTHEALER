#include "pch.h"
#include "Figure.h"

Figure::Figure()
{
}

Figure::~Figure()
{
	SafeRelease(&pPenBrush);
	SafeRelease(&pFillBrush);
}

void Figure::Initailize(ID2D1HwndRenderTarget* pRanderTarget)
{
	pRT = pRanderTarget;
	pRT->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), &pFillBrush);
	pRT->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black), &pPenBrush);
}

void Figure::SetBrushColor(D2D1::ColorF color)
{
	pPenBrush->SetColor(D2D1::ColorF(color));
}

void Figure::SetFillColor(D2D1::ColorF color)
{
	pFillBrush->SetColor(D2D1::ColorF(color));
}

D2D1::ColorF Figure::GetBrushColor()
{
	return { pPenBrush->GetColor().r, pPenBrush->GetColor().g, pPenBrush->GetColor().b };
}

D2D1::ColorF Figure::GetFillColor()
{
	return { pFillBrush->GetColor().r, pFillBrush->GetColor().g, pFillBrush->GetColor().b };
}



void Figure::DrawRectangle(int left, int right, int top, int bottom, int alpha)
{

	D2D1_RECT_F rect;
	rect.left = (float)left;
	rect.right = (float)right;
	rect.top = (float)top;
	rect.bottom = (float)bottom;

	pFillBrush->SetOpacity(alpha);
	pRT->DrawRectangle(rect, pPenBrush, 1.0f);
	pRT->FillRectangle(rect, pFillBrush);
}

void Figure::DrawRectangle(POINT start, POINT end, int alpha)
{
	D2D1_RECT_F rect;
	rect.left = (float)start.x;
	rect.right = (float)end.x;
	rect.top = (float)start.y;
	rect.bottom = (float)end.y;
	if (rect.left > rect.right)
	{
		rect.left = (float)end.x;
		rect.right = (float)start.x;
	}
	if (rect.top < rect.bottom)
	{
		rect.top = (float)end.y;
		rect.bottom = (float)start.y;
	}

	pFillBrush->SetOpacity(alpha);
	pRT->DrawRectangle(rect, pPenBrush, 1.0f);
	pRT->FillRectangle(rect, pFillBrush);
}


void Figure::DrawCircle(POINT ceter, int radius, int alpha)
{

	D2D1_ELLIPSE ellipse;
	ellipse.point.x = ceter.x;
	ellipse.point.y = ceter.y;
	ellipse.radiusX = (float)radius;
	ellipse.radiusY = (float)radius;

	pRT->DrawEllipse(ellipse, pPenBrush, 1.0f);
	pRT->FillEllipse(ellipse, pFillBrush);
}

//void Figure::DrawCircle(POINT start, POINT end, int alpha)
//{
//
//	D2D1_ELLIPSE ellipse;
//	ellipse.point.x = (start.x + end.x) / 2;
//	ellipse.point.y = (start.y + end.y) / 2;
//	ellipse.radiusX = end.x - ellipse.point.x;
//	ellipse.radiusY = start.y - ellipse.point.y;
//
//	pRT->DrawEllipse(ellipse, pPenBrush, 1.0f);
//	pRT->FillEllipse(ellipse, pFillBrush);
//}