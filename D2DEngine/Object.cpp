#include "pch.h"
#include "Object.h"

namespace Obj
{
	Object::Object(D2D1::ColorF p, D2D1::ColorF f, float a)
		:penColor(p), fillColor(f), alpha(a)
	{

	}

	Object::~Object()
	{
	}

	Rectangle::Rectangle(D2D1::ColorF p, D2D1::ColorF f, float a, POINT xy1, POINT xy2)
		: Object(p, f, a)
	{
		rect.left = xy1.x;
		rect.top = xy1.y;
		rect.right = xy2.x;
		rect.bottom = xy2.y;
	}

	Rectangle::~Rectangle()
	{
	}

	Ellipse::Ellipse(D2D1::ColorF p, D2D1::ColorF f, float a, POINT xy, float rx, float ry)
		: Object(p, f, a), center(xy), ridiusX(rx), ridiusY(ry)
	{

	}

	Ellipse::~Ellipse()
	{
	}

	Line::Line(D2D1::ColorF p, D2D1::ColorF f, float a, POINT xy1, POINT xy2, float s)
		: Object(p, f, a), start(xy1), end(xy2), size(s)
	{

	}
	Line::~Line()
	{
	}

	Text::Text(D2D1::ColorF p, D2D1::ColorF f, float a, const wchar_t* t, POINT xy1, POINT xy2, float s)
		: Object(p, f, a), text(t), start(xy1), end(xy2), size(s)
	{
	}
	Text::~Text()
	{
	}

	Image::Image(D2D1::ColorF p, D2D1::ColorF f, float a, ID2D1Bitmap* i, POINT st, POINTF sc)
		: Object(p, f, a), image(i), start(st), scale(sc)
	{
	}
	Image::~Image()
	{
	}

	Sprite::Sprite(D2D1::ColorF p, D2D1::ColorF f, float a, ID2D1Bitmap* i, POINT st, POINTF sc, float s, float curr, int maxX, int maxY)
		:Object(p, f, a), image(i), start(st), scale(sc), speed(s), currentFrame(curr), maxFrameX(maxX), maxFrameY(maxY)
	{
	}
	Sprite::~Sprite()
	{
	}
}