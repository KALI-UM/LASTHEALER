#pragma once
class D2D1::ColorF;
class Figure
{
public:
	Figure();
	~Figure();

	void Initailize(ID2D1HwndRenderTarget* pRanderTarget);

private:
	ID2D1HwndRenderTarget* pRT;
	ID2D1SolidColorBrush* pPenBrush;
	ID2D1SolidColorBrush* pFillBrush;

public:
	void SetBrushColor(D2D1::ColorF color);
	void SetFillColor(D2D1::ColorF color);
	D2D1::ColorF GetBrushColor();
	D2D1::ColorF GetFillColor();
	void DrawRectangle(int left, int right, int top, int bottom, int alpha);
	void DrawRectangle(POINT start, POINT end, int alpha);
	void DrawCircle(POINT ceter, int radius, int alpha);


	//void DrawCircle(POINT start, POINT end, int alpha);
};

