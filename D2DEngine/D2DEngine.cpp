#include "pch.h"
#include <typeinfo>
#include "Figure.h"
#include "Object.h"

D2DEngine::D2DEngine()
	:pD2DFactory(nullptr), pRT(nullptr), HWnd(nullptr), pFigure(nullptr), background(NULL)
{

}

D2DEngine::~D2DEngine()
{
	SafeRelease(&pD2DFactory);
	SafeRelease(&pRT);

	SafeRelease(&pImagingFactory);
	SafeRelease(&pBitmapDecoder);
	SafeRelease(&pBitmapFrameDecode);
	SafeRelease(&pFormatConverter);
}


void D2DEngine::Initialize(HWND hWnd)
{
	HRESULT hr = E_NOTIMPL;

	hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &pD2DFactory);
	assert(hr == S_OK);
	HWnd = hWnd;
	RECT rc;
	GetClientRect(hWnd, &rc);
	hr = pD2DFactory->CreateHwndRenderTarget(D2D1::RenderTargetProperties(),
		D2D1::HwndRenderTargetProperties(hWnd, D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top)), &pRT);
	assert(hr == S_OK);

	hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(pWriteFactory), reinterpret_cast<IUnknown**>(&pWriteFactory));
	assert(hr == S_OK);


	hr = pWriteFactory->CreateTextFormat(
		L"Verdana", NULL,
		DWRITE_FONT_WEIGHT_NORMAL,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		20, L"", &pDefaultTextFormat);
	assert(hr == S_OK);
	if (SUCCEEDED(hr))
	{
		pDefaultTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
		pDefaultTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
	}

	hr = CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pImagingFactory));
	assert(hr == S_OK);

	background = D2D1::ColorF(D2D1::ColorF::Black);

	pFigure = new Figure();
	pFigure->Initailize(pRT);
}

void D2DEngine::BeginRender()
{
	RECT rc;
	GetClientRect(HWnd, &rc);

	//클라이언트 내 이미지를 항상 일정 사이즈로 유지하기 위함
	pRT->Resize(D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top));

	pRT->SetTransform(D2D1::Matrix3x2F::Identity());

	pRT->BeginDraw();
	pRT->Clear(background);

	for (Obj::Object* obj : pObjects)
		Draw(obj);
}

void D2DEngine::EndRender()
{
	pRT->EndDraw();
}

void D2DEngine::Clear()
{
	pObjects.clear();
}

void D2DEngine::SetBackgroundColor(D2D1::ColorF color)
{
	background.r = color.r;
	background.g = color.g;
	background.b = color.b;
	background.a = color.a;
}

void D2DEngine::Draw(Obj::Object* obj)
{
	Obj::Rectangle* r = dynamic_cast<Obj::Rectangle*>(obj);
	if (r != nullptr)
	{
		DrawRect(r);
		return;
	}
	Obj::Ellipse* e = dynamic_cast<Obj::Ellipse*>(obj);
	if (e != nullptr)
	{
		DrawEllipse(e);
		return;
	}
	Obj::Line* l = dynamic_cast<Obj::Line*>(obj);
	if (l != nullptr)
	{
		DrawLine(l);
		return;
	}
	Obj::Text* t = dynamic_cast<Obj::Text*>(obj);
	if (t != nullptr)
	{
		WriteText(t);
		return;
	}
	Obj::Image* i = dynamic_cast<Obj::Image*>(obj);
	if (i != nullptr)
	{
		DrawImage(i);
		return;
	}
	Obj::Sprite* s = dynamic_cast<Obj::Sprite*>(obj);
	if (s != nullptr)
	{
		DrawSprite(s);
		return;
	}
}

void D2DEngine::SaveDrawing(Obj::Object* obj)
{
	pObjects.push_back(obj);
}

void D2DEngine::DrawRect(Obj::Rectangle* r)
{
	DrawRect(r->rect, r->penColor, r->fillColor, r->alpha);
}

void D2DEngine::DrawEllipse(Obj::Ellipse* e)
{
	DrawEllipse(e->center, e->ridiusX, e->ridiusY, e->penColor, e->fillColor, e->alpha);
}

void D2DEngine::DrawLine(Obj::Line* l)
{
	DrawLine(l->start, l->end, l->penColor, l->size, l->alpha);
}

void D2DEngine::WriteText(Obj::Text* t)
{
	WriteText(t->start, t->end, t->text.c_str(), L"Verdana", t->size, t->penColor, t->alpha);
}

void D2DEngine::DrawImage(Obj::Image* i)
{
	pRT->DrawBitmap(i->image, D2D1::RectF(i->start.x, i->start.y, i->start.x+(i->image->GetSize().width * i->scale.x), i->start.y+(i->image->GetSize().height * i->scale.y)));
}

void D2DEngine::DrawSprite(Obj::Sprite* s)
{
	int curr = floor(s->currentFrame);
	pRT->DrawBitmap(s->image,
		D2D1::RectF(s->start.x,
			s->start.y,
			s->start.x + (s->image->GetSize().width / s->maxFrameX * s->scale.x),
			s->start.y + (s->image->GetSize().height / s->maxFrameY * s->scale.y)), 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR,
		D2D1::RectF(curr % s->maxFrameX * s->image->GetSize().width / s->maxFrameX,
			curr / s->maxFrameX * s->image->GetSize().height / s->maxFrameY,
			(curr % s->maxFrameX + 1) * s->image->GetSize().width / s->maxFrameX,
			(curr / s->maxFrameX + 1) * s->image->GetSize().height / s->maxFrameY));
	s->currentFrame += s->speed;
	if(s->currentFrame>= (s->maxFrameX * s->maxFrameY)) s->currentFrame-=(s->maxFrameX * s->maxFrameY);
}

void D2DEngine::DrawCircle(POINT CeterPoint, int Radius, D2D1::ColorF PenColor, D2D1::ColorF FillColor, int Alpha)
{
	ID2D1SolidColorBrush* pPenBrush = nullptr;
	ID2D1SolidColorBrush* pFillBrush = nullptr;
	pRT->CreateSolidColorBrush(D2D1::ColorF(FillColor), &pFillBrush);
	pRT->CreateSolidColorBrush(D2D1::ColorF(PenColor), &pPenBrush);

	D2D1_ELLIPSE _ellipse;
	_ellipse.point.x = CeterPoint.x;
	_ellipse.point.y = CeterPoint.y;
	_ellipse.radiusX = (float)Radius;
	_ellipse.radiusY = (float)Radius;

	pRT->DrawEllipse(_ellipse, pPenBrush, 1.0f);
	pRT->FillEllipse(_ellipse, pFillBrush);

	SafeRelease(&pFillBrush);
	SafeRelease(&pPenBrush);
}

void D2DEngine::DrawCircle(POINT CeterPoint, int Radius, D2D1::ColorF PenColor, int Alpha)
{
	ID2D1SolidColorBrush* pPenBrush = nullptr;
	pRT->CreateSolidColorBrush(D2D1::ColorF(PenColor), &pPenBrush);

	D2D1_ELLIPSE _ellipse;
	_ellipse.point.x = CeterPoint.x;
	_ellipse.point.y = CeterPoint.y;
	_ellipse.radiusX = (float)Radius;
	_ellipse.radiusY = (float)Radius;

	pRT->DrawEllipse(_ellipse, pPenBrush, 1.0f);

	SafeRelease(&pPenBrush);
}

void D2DEngine::DrawEllipse(POINT CeterPoint, int RadiusX, int RadiusY, D2D1::ColorF PenColor, D2D1::ColorF FillColor, int Alpha)
{
	ID2D1SolidColorBrush* pPenBrush = nullptr;
	ID2D1SolidColorBrush* pFillBrush = nullptr;
	pRT->CreateSolidColorBrush(D2D1::ColorF(FillColor), &pFillBrush);
	pRT->CreateSolidColorBrush(D2D1::ColorF(PenColor), &pPenBrush);

	D2D1_ELLIPSE _ellipse;
	_ellipse.point.x = CeterPoint.x;
	_ellipse.point.y = CeterPoint.y;
	_ellipse.radiusX = (float)RadiusX;
	_ellipse.radiusY = (float)RadiusY;

	pRT->DrawEllipse(_ellipse, pPenBrush, 1.0f);
	pRT->FillEllipse(_ellipse, pFillBrush);

	SafeRelease(&pFillBrush);
	SafeRelease(&pPenBrush);
}

void D2DEngine::DrawEllipse(POINT CeterPoint, int RadiusX, int RadiusY, D2D1::ColorF PenColor, int Alpha)
{
	ID2D1SolidColorBrush* pPenBrush = nullptr;
	ID2D1SolidColorBrush* pFillBrush = nullptr;
	pRT->CreateSolidColorBrush(D2D1::ColorF(PenColor), &pPenBrush);

	D2D1_ELLIPSE _ellipse;
	_ellipse.point.x = CeterPoint.x;
	_ellipse.point.y = CeterPoint.y;
	_ellipse.radiusX = (float)RadiusX;
	_ellipse.radiusY = (float)RadiusY;

	pRT->DrawEllipse(_ellipse, pPenBrush, 1.0f);

	SafeRelease(&pPenBrush);
}

void D2DEngine::DrawRect(int left, int right, int top, int bottom, D2D1::ColorF PenColor, D2D1::ColorF FillColor, int Alpha)
{
	ID2D1SolidColorBrush* pPenBrush = nullptr;
	ID2D1SolidColorBrush* pFillBrush = nullptr;
	pRT->CreateSolidColorBrush(D2D1::ColorF(FillColor), &pFillBrush);
	pRT->CreateSolidColorBrush(D2D1::ColorF(PenColor), &pPenBrush);

	D2D1_RECT_F _rect;
	_rect.left = (float)left;
	_rect.right = (float)right;
	_rect.top = (float)top;
	_rect.bottom = (float)bottom;

	pRT->DrawRectangle(_rect, pPenBrush, 1.0f);
	pRT->FillRectangle(_rect, pFillBrush);

	SafeRelease(&pPenBrush);
	SafeRelease(&pFillBrush);
}

void D2DEngine::DrawRect(int left, int right, int top, int bottom, D2D1::ColorF PenColor, int Alpha)
{
	ID2D1SolidColorBrush* pPenBrush = nullptr;
	pRT->CreateSolidColorBrush(D2D1::ColorF(PenColor), &pPenBrush);

	D2D1_RECT_F _rect;
	_rect.left = (float)left;
	_rect.right = (float)right;
	_rect.top = (float)top;
	_rect.bottom = (float)bottom;

	pRT->DrawRectangle(_rect, pPenBrush, 1.0f);

	SafeRelease(&pPenBrush);
}

void D2DEngine::DrawRect(RECT target, D2D1::ColorF PenColor, int Alpha)
{
	ID2D1SolidColorBrush* pPenBrush = nullptr;
	pRT->CreateSolidColorBrush(D2D1::ColorF(PenColor), &pPenBrush);

	D2D1_RECT_F _rect;
	_rect.left = target.left;
	_rect.right = target.right;
	_rect.top = target.top;
	_rect.bottom = target.bottom;

	pRT->DrawRectangle(_rect, pPenBrush, 1.0f);

	SafeRelease(&pPenBrush);
}

void D2DEngine::DrawRect(RECT target, D2D1::ColorF PenColor, D2D1::ColorF FillColor, int Alpha)
{
	ID2D1SolidColorBrush* pPenBrush = nullptr;
	ID2D1SolidColorBrush* pFillBrush = nullptr;
	pRT->CreateSolidColorBrush(D2D1::ColorF(FillColor), &pFillBrush);
	pRT->CreateSolidColorBrush(D2D1::ColorF(PenColor), &pPenBrush);

	D2D1_RECT_F _rect;
	_rect.left = target.left;
	_rect.right = target.right;
	_rect.top = target.top;
	_rect.bottom = target.bottom;

	pRT->DrawRectangle(_rect, pPenBrush, 1.0f);
	pRT->FillRectangle(_rect, pFillBrush);

	SafeRelease(&pPenBrush);
	SafeRelease(&pFillBrush);
}

void D2DEngine::DrawRect(GORectangle* target, D2D1::ColorF PenColor, int Alpha)
{
	ID2D1SolidColorBrush* pPenBrush = nullptr;
	pRT->CreateSolidColorBrush(D2D1::ColorF(PenColor), &pPenBrush);

	D2D1_RECT_F _rect;
	_rect.left = target->m_Rect.left;
	_rect.right = target->m_Rect.right;
	_rect.top = target->m_Rect.top;
	_rect.bottom = target->m_Rect.bottom;

	pRT->DrawRectangle(_rect, pPenBrush, 1.0f);

	SafeRelease(&pPenBrush);
}

void D2DEngine::DrawRect(GORectangle* target, D2D1::ColorF PenColor, D2D1::ColorF FillColor, int Alpha)
{
	ID2D1SolidColorBrush* pPenBrush = nullptr;
	ID2D1SolidColorBrush* pFillBrush = nullptr;
	pRT->CreateSolidColorBrush(D2D1::ColorF(FillColor), &pFillBrush);
	pRT->CreateSolidColorBrush(D2D1::ColorF(PenColor), &pPenBrush);

	D2D1_RECT_F _rect;
	_rect.left = target->m_Rect.left;
	_rect.right = target->m_Rect.right;
	_rect.top = target->m_Rect.top;
	_rect.bottom = target->m_Rect.bottom;

	pRT->DrawRectangle(_rect, pPenBrush, 1.0f);
	pRT->FillRectangle(_rect, pFillBrush);

	SafeRelease(&pPenBrush);
	SafeRelease(&pFillBrush);
}

void D2DEngine::DrawLine(POINT start, POINT end, D2D1::ColorF PenColor, int PenSize, int Alpha)
{
	ID2D1SolidColorBrush* pPenBrush = nullptr;
	pRT->CreateSolidColorBrush(D2D1::ColorF(PenColor), &pPenBrush);

	pRT->DrawLine(
		D2D1::Point2F(start.x, start.y),
		D2D1::Point2F(end.x, end.y),
		pPenBrush,
		(float)PenSize);

	SafeRelease(&pPenBrush);
}

void D2DEngine::DrawLine(POINT start, POINT end, D2D1::ColorF PenColor, int Alpha)
{
	ID2D1SolidColorBrush* pPenBrush = nullptr;
	pRT->CreateSolidColorBrush(D2D1::ColorF(PenColor), &pPenBrush);

	pRT->DrawLine(
		D2D1::Point2F(start.x, start.y),
		D2D1::Point2F(end.x, end.y),
		pPenBrush,
		1.0f);

	SafeRelease(&pPenBrush);
}

void D2DEngine::WriteText(POINT start, POINT end, const wchar_t* text, D2D1::ColorF PenColor, int Alpha)
{
	ID2D1SolidColorBrush* pPenBrush = nullptr;
	pRT->CreateSolidColorBrush(D2D1::ColorF(PenColor), &pPenBrush);

	std::wstring str(text);
	pRT->DrawText(text, str.length(), pDefaultTextFormat, D2D1::RectF(start.x, start.y, end.x, end.y), pPenBrush);
	SafeRelease(&pPenBrush);
}

void D2DEngine::WriteText(POINT start, POINT end, const wchar_t* text, const wchar_t* font, float size, D2D1::ColorF PenColor, int Alpha)
{
	ID2D1SolidColorBrush* pPenBrush = nullptr;
	pRT->CreateSolidColorBrush(D2D1::ColorF(PenColor), &pPenBrush);

	std::wstring str(text);
	IDWriteTextFormat* pTempTextFormat;
	HRESULT hr = pWriteFactory->CreateTextFormat(
		font, NULL,
		DWRITE_FONT_WEIGHT_NORMAL,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		size, L"", &pTempTextFormat);

	if (SUCCEEDED(hr))
	{
		pTempTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
		pTempTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
	}

	pRT->DrawText(text, str.length(), pTempTextFormat, D2D1::RectF(start.x, start.y, end.x, end.y), pPenBrush);
	SafeRelease(&pPenBrush);
	SafeRelease(&pTempTextFormat);
}

Obj::Image* D2DEngine::Filename2Image(const wchar_t* filename)
{
	ID2D1Bitmap* bitmap = NULL;
	HRESULT hr = pImagingFactory->CreateDecoderFromFilename(filename, NULL, GENERIC_READ, WICDecodeMetadataCacheOnLoad, &pBitmapDecoder);

	if (SUCCEEDED(hr))
		hr = pBitmapDecoder->GetFrame(0, &pBitmapFrameDecode);

	if (SUCCEEDED(hr))
		hr = pImagingFactory->CreateFormatConverter(&pFormatConverter);

	if (SUCCEEDED(hr))
		hr = pFormatConverter->Initialize(pBitmapFrameDecode, GUID_WICPixelFormat32bppPBGRA, WICBitmapDitherTypeNone, NULL, 0.f, WICBitmapPaletteTypeMedianCut);

	if (SUCCEEDED(hr))
		hr = pRT->CreateBitmapFromWicBitmap(pFormatConverter, NULL, &bitmap);
	Obj::Image* image = new Obj::Image(RGB(0, 0, 0), RGB(0, 0, 0), 1.f, bitmap, { 0,0 }, { 1,1 });
	return image;
}

void D2DEngine::Filename2DrawImage(const wchar_t* filename)
{
	SaveDrawing(Filename2Image(filename));
}

Obj::Sprite* D2DEngine::Filename2Sprite(const wchar_t* filename, int maxFrameX, int maxFrameY, float speed)
{
	ID2D1Bitmap* bitmap = NULL;
	HRESULT hr = pImagingFactory->CreateDecoderFromFilename(filename, NULL, GENERIC_READ, WICDecodeMetadataCacheOnLoad, &pBitmapDecoder);
	if (maxFrameX == 0)maxFrameX = 1;
	if (maxFrameY == 0)maxFrameY = 1;

	if (SUCCEEDED(hr))
		hr = pBitmapDecoder->GetFrame(0, &pBitmapFrameDecode);

	if (SUCCEEDED(hr))
		hr = pImagingFactory->CreateFormatConverter(&pFormatConverter);

	if (SUCCEEDED(hr))
		hr = pFormatConverter->Initialize(pBitmapFrameDecode, GUID_WICPixelFormat32bppPBGRA, WICBitmapDitherTypeNone, NULL, 0.f, WICBitmapPaletteTypeMedianCut);

	if (SUCCEEDED(hr))
		hr = pRT->CreateBitmapFromWicBitmap(pFormatConverter, NULL, &bitmap);
	Obj::Sprite* sprite = new Obj::Sprite(RGB(0, 0, 0), RGB(0, 0, 0), 1.f, bitmap, { 0,0 }, { 1,1 }, 1, 0, maxFrameX, maxFrameY);
	return sprite;
}

void D2DEngine::Filename2DrawSprite(const wchar_t* filename, int maxFrameX, int maxFrameY, float speed)
{
	SaveDrawing(Filename2Sprite(filename, maxFrameX, maxFrameY, speed));
}

bool D2DEngine::CircleCollisionCheck(POINT thisCenter, int thisRadius, POINT targetCenter, int targetRadius)
{
	float _distance = (thisCenter.x - targetCenter.x) * (thisCenter.x - targetCenter.x) +
		(thisCenter.y - targetCenter.y) * (thisCenter.y - targetCenter.y);
	float _R1R2 = (thisRadius + targetRadius) * (thisRadius + targetRadius);
	if (_R1R2 > _distance)
	{
		return true;
	}

	return false;
}

bool D2DEngine::AABBCollisionCheck(RECT thisRect, RECT targetRect)
{
	bool Xcoll = false;
	bool Ycoll = false;

	//X축 충돌
	if (thisRect.left <= targetRect.right && thisRect.right >= targetRect.left) Xcoll = true;

	//Y축 충돌
	if (thisRect.bottom >= targetRect.top) Ycoll = true;

	if (Xcoll == true && Ycoll == true)
	{
		return true;
	}

	return false;
}

GORectangle* D2DEngine::RotateCenter(GORectangle* Target, float angle)
{
	/*Matrix31 _targetMatrix = _targetMatrix.ConvertTo31(Target->m_Center);

	float _radius = sqrtf(powf(Target->m_Center.x - center.x, 2) + powf(Target->m_Center.y - center.y, 2));

	Matrix33 _rotateMatrix(_radius * cosf(anlge), 0, 0, _radius * sinf(anlge), 0, 0, 0, 0, 1);
	Matrix31 _result = _rotateMatrix * _targetMatrix;*/

	D2D1::Matrix3x2F _transformMatrix = D2D1::Matrix3x2F::Translation((Target->m_Rect.left) * (-1), (Target->m_Rect.top) * (-1));
	D2D1::Matrix3x2F _transformMatrix2 = D2D1::Matrix3x2F::Translation((Target->m_Rect.left), (Target->m_Rect.top));
	D2D1::Matrix3x2F _rotationMatrix = D2D1::Matrix3x2F::Rotation(angle, D2D1::Point2F(Target->m_Center.x, Target->m_Center.y));


	//pRT->SetTransform(_transformMatrix);
	pRT->SetTransform(_rotationMatrix);

	DrawRect(Target, D2D1::ColorF(RGB(200, 200, 0)), 1.0f);
	//pRT->SetTransform(_transformMatrix2);

	//다시 좌표계를 초기화한다.
	pRT->SetTransform(D2D1::Matrix3x2F::Identity());
	DrawRect(Target, D2D1::ColorF(RGB(200, 200, 0)), 1.0f);

	return Target;
}
