#pragma once
#include <string>
namespace Obj
{
	class Object
	{
	public:
		Object(D2D1::ColorF p, D2D1::ColorF f, float a);
		virtual ~Object();
		D2D1::ColorF penColor = { 10,10,10 };
		D2D1::ColorF fillColor = { 0,0,0 };
		float alpha = 1.0f;
	};

	class Rectangle : public Object
	{
	public:
		Rectangle(D2D1::ColorF p, D2D1::ColorF f, float a, POINT xy1, POINT xy2);
		virtual ~Rectangle();
		RECT rect;
	};

	class Ellipse : public Object
	{
	public:
		Ellipse(D2D1::ColorF p, D2D1::ColorF f, float a, POINT xy, float rx, float ry);
		virtual ~Ellipse();
		POINT center = { 0,0 };
		float ridiusX = 10.f;
		float ridiusY = 10.f;
	};

	class Line : public Object
	{
	public:
		Line(D2D1::ColorF p, D2D1::ColorF f, float a, POINT xy1, POINT xy2, float s);
		virtual ~Line();
		POINT start = { 0,0 };
		POINT end = { 0,0 };
		float size = 10.f;
	};

	class Text :public Object
	{
	public:
		Text(D2D1::ColorF p, D2D1::ColorF f, float a, const wchar_t* t, POINT xy1, POINT xy2, float s);
		virtual ~Text();
		std::wstring text;
		POINT start = { 0,0 };
		POINT end = { 100,100 };
		float size = 10.f;
	};

	class Image :public Object
	{
	public:
		Image(D2D1::ColorF p, D2D1::ColorF f, float a, ID2D1Bitmap* i, POINT st, POINTF sc);
		virtual ~Image();
		ID2D1Bitmap* image = NULL;
		POINT start = { 0,0 };
		POINTF scale = { 1,1 };
	};

	class Sprite :public Object
	{
	public:
		Sprite(D2D1::ColorF p, D2D1::ColorF f, float a, ID2D1Bitmap* i, POINT st, POINTF sc, float s,float curr, int maxX, int maxY);
		virtual ~Sprite();
		ID2D1Bitmap* image = NULL;
		POINT start = { 0,0 };
		POINTF scale = { 1,1 };
		float speed = 1;
		float currentFrame = 0;
		int maxFrameX = 1;
		int maxFrameY = 1;
	};
}