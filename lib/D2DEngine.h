#pragma once
#include <vector>

class GORectangle;
class Figure;
namespace Obj
{
	class Object;
	class Rectangle;
	class Ellipse;
	class Line;
	class Text;
	class Image;
	class Sprite;
}

class D2DEngine
{
public:
	D2DEngine();
	~D2DEngine();

public:

	void Initialize(HWND hWnd);

	void BeginRender();
	void EndRender();

	void Clear();

	ID2D1HwndRenderTarget* GetRenderTarget() { return pRT; }
	void SetBackgroundColor(D2D1::ColorF color);

	//각종 도형 그리기 함수
	void Draw(Obj::Object* obj);
	void SaveDrawing(Obj::Object* obj);
	void DrawRect(Obj::Rectangle* r);
	void DrawEllipse(Obj::Ellipse* e);
	void DrawLine(Obj::Line* l);
	void WriteText(Obj::Text* t);
	void DrawImage(Obj::Image* i);
	void DrawSprite(Obj::Sprite* s);

	void DrawCircle(POINT CeterPoint, int Radius, D2D1::ColorF PenColor, D2D1::ColorF FillColor, int Alpha);
	void DrawCircle(POINT CeterPoint, int Radius, D2D1::ColorF PenColor, int Alpha);
	void DrawEllipse(POINT CeterPoint, int RadiusX, int RadiusY, D2D1::ColorF PenColor, D2D1::ColorF FillColor, int Alpha);
	void DrawEllipse(POINT CeterPoint, int RadiusX, int RadiusY, D2D1::ColorF PenColor, int Alpha);

	void DrawRect(int left, int right, int top, int bottom, D2D1::ColorF PenColor, D2D1::ColorF FillColor, int Alpha);
	void DrawRect(int left, int right, int top, int bottom, D2D1::ColorF PenColor, int Alpha);
	void DrawRect(RECT target, D2D1::ColorF PenColor, int Alpha);
	void DrawRect(RECT target, D2D1::ColorF PenColor, D2D1::ColorF FillColor, int Alpha);
	void DrawRect(GORectangle* target, D2D1::ColorF PenColor, int Alpha);
	void DrawRect(GORectangle* target, D2D1::ColorF PenColor, D2D1::ColorF FillColor, int Alpha);

	void DrawLine(POINT start, POINT end, D2D1::ColorF PenColor, int PenSize, int Alpha);
	void DrawLine(POINT start, POINT end, D2D1::ColorF PenColor, int Alpha);

	void WriteText(POINT start, POINT end, const wchar_t* text, D2D1::ColorF PenColor, int Alpha);
	void WriteText(POINT start, POINT end, const wchar_t* text, const wchar_t* font, float size, D2D1::ColorF PenColor, int Alpha);

	Obj::Image* Filename2Image(const wchar_t* filename);
	void Filename2DrawImage(const wchar_t* filename);
	Obj::Sprite* Filename2Sprite(const wchar_t* filename, int maxFrameX, int maxFrameY, float speed);
	void Filename2DrawSprite(const wchar_t* filename, int maxFrameX, int maxFrameY, float speed);


	//충돌체크 함수
	bool CircleCollisionCheck(POINT thisCenter, int thisRadius, POINT targetCenter, int targetRadius);
	bool AABBCollisionCheck(RECT thisRect, RECT targetRect);

	//회전 함수

	//중점을 기준으로 회전하는 함수
	GORectangle* RotateCenter(GORectangle* Target, float anlge);




private:

	HWND HWnd;
	ID2D1Factory* pD2DFactory;
	ID2D1HwndRenderTarget* pRT;

	IDWriteFactory* pWriteFactory;
	IDWriteTextFormat* pDefaultTextFormat;

	IWICImagingFactory* pImagingFactory;
	IWICBitmapDecoder* pBitmapDecoder;
	IWICBitmapFrameDecode* pBitmapFrameDecode;
	IWICFormatConverter* pFormatConverter;


	D2D1::ColorF background;

	Figure* pFigure;
	std::vector<Obj::Object*> pObjects;
};
