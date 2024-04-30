#include "pch.h"
#include "Enemy.h"
#include "ProjectileObject.h"

ProjectileObject::ProjectileObject()
	:GameObject::GameObject(),
	m_IsLoaded(TRUE), m_TargetNVec(), m_Velo(10), m_Dir(P_DIR::LEFT), m_RotateAngle(0)
{
	m_TargetNVec.x = 0;
	m_TargetNVec.y = 0;
}

ProjectileObject::~ProjectileObject()
{

}

void ProjectileObject::MoveTo()
{
	_Transform._Position.x += m_TargetNVec.x * m_Velo;
	_Transform._Position.y += m_TargetNVec.y * m_Velo;
}

HRESULT ProjectileObject::Init()
{
	_Transform._Position = Vector2F(0.f, 0.f);
	_Transform._Scale = SizeF(0.3f, 0.3f);

	m_Dir = P_DIR::LEFT;
	m_RotateAngle = 0.f;

	return S_OK;
}

void ProjectileObject::Release()
{

}

void ProjectileObject::Reset()
{
	m_IsLoaded = TRUE;
	_Transform._Position = Vector2F(0.f, 0.f);
	m_TargetNVec = Vector2F(0.f, 0.f);
	m_Dir = P_DIR::LEFT;
	m_RotateAngle = 0.f;

}

void ProjectileObject::Update()
{
	//단순히 움직이기만 하는 업데이트,
	//각 투사체에서 특별한 무언가가 있다면 다시 재 정의해
	if (m_IsLoaded == FALSE)
	{
		MoveTo();
	}
}

void ProjectileObject::Render()
{
	//여기서는 이미지를 들고 있지 않으므로 상속받은 자신에서 그려라
}

void ProjectileObject::SetPos(D2D1_VECTOR_2F pos)
{
	_Transform._Position = Vector2F(pos.x, pos.y);
}

void ProjectileObject::Shoot(D2D1_VECTOR_2F targetPos)
{
	m_IsLoaded = FALSE;

	//타겟과 나의 거리 차 x, y = 벡터
	float _dx = targetPos.x - _Transform._Position.x;
	float _dy = targetPos.y - _Transform._Position.y;

	m_RotateAngle = acosf(_dx / sqrtf(_dx * _dx + _dy * _dy)) * 180.f / PI;

	//를 거리로 나눈 만큼 방향벡타
	m_TargetNVec.x = _dx / sqrtf(_dx * _dx + _dy * _dy);
	m_TargetNVec.y = _dy / sqrtf(_dx * _dx + _dy * _dy);

	if (_dx <= 0)
	{
		//애니메이션으로 변동시 위치 이동필요
		m_Dir = P_DIR::LEFT;
		//_Image->CurrentFrameY = 0;
		//_Image->MaxFrameX = 5;

		_dy <= 0 ?
			m_RotateAngle = 180.f - m_RotateAngle : m_RotateAngle = m_RotateAngle - 180.f;
	}
	else
	{
		m_Dir = P_DIR::RIGHT;
		//_Image->CurrentFrameY = 1;
		//_Image->MaxFrameX = 5;

		_dy <= 0 ?
			m_RotateAngle = 360.f - m_RotateAngle : m_RotateAngle = m_RotateAngle;
	}

	_Transform.ResetRotationAngle();
	_Transform.SetRotationAngle(m_RotateAngle);
#ifdef _DEBUG
	cout << _Transform.GetRotationAngle() << endl;
#endif
}
