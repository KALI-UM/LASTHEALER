#include "pch.h"
#include "Barricade.h"

HRESULT Barricade::Init()
{
    _Image = g_D2DEngine.LoadSingleImage("Resource/FrameImageSample/gravestone.png");
	_IsActive = TRUE;
	_Transform._Position = Vector2F((float)(rand()% 3000), (float)(rand() % 1500));
	_Transform._Scale = SizeF(0.5f, 0.5f);
	GameObject::RectColliderSetting(SizeF(_Image->OriginalHeight/2, _Image->OriginalWidth / 2), _CAMERAMANAGER->GetCamPos());
    return E_NOTIMPL;
}

void Barricade::Release()
{
}

void Barricade::Reset()
{
}

void Barricade::Update()
{
	GameObject::RectColliderSetting(SizeF(_Image->OriginalHeight / 2, _Image->OriginalWidth / 2), _CAMERAMANAGER->GetCamPos());
}

void Barricade::Render()
{
	g_D2DEngine.DrawSingleImage(_CAMERAMANAGER->GetCamPos(), _Transform, _Image);

#ifdef _DEBUG
		if (_GAMEMANAGER->GetShowDataInfo())
		{
			if (_IsCollision == false)
			{
				g_D2DEngine.DrawRect(_RectCollider, BrushColor::Black);
			}
			else
			{
				g_D2DEngine.DrawRect(_RectCollider, BrushColor::Yellow);
			}
		}
#endif
}
