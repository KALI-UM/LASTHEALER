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
	//�ܼ��� �����̱⸸ �ϴ� ������Ʈ,
	//�� ����ü���� Ư���� ���𰡰� �ִٸ� �ٽ� �� ������
	if (m_IsLoaded == FALSE)
	{
		MoveTo();
	}
}

void ProjectileObject::Render()
{
	//���⼭�� �̹����� ��� ���� �����Ƿ� ��ӹ��� �ڽſ��� �׷���
}

void ProjectileObject::SetPos(D2D1_VECTOR_2F pos)
{
	_Transform._Position = Vector2F(pos.x, pos.y);
}

void ProjectileObject::Shoot(D2D1_VECTOR_2F targetPos)
{
	m_IsLoaded = FALSE;

	//Ÿ�ٰ� ���� �Ÿ� �� x, y = ����
	float _dx = targetPos.x - _Transform._Position.x;
	float _dy = targetPos.y - _Transform._Position.y;

	m_RotateAngle = acosf(_dx / sqrtf(_dx * _dx + _dy * _dy)) * 180.f / PI;

	//�� �Ÿ��� ���� ��ŭ ���⺤Ÿ
	m_TargetNVec.x = _dx / sqrtf(_dx * _dx + _dy * _dy);
	m_TargetNVec.y = _dy / sqrtf(_dx * _dx + _dy * _dy);

	if (_dx <= 0)
	{
		//�ִϸ��̼����� ������ ��ġ �̵��ʿ�
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
