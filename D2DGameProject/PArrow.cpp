#include "pch.h"
#include "Enemy.h"
#include "PArrow.h"

PArrow::PArrow()
	:m_IsSkill(FALSE), m_TargetVec(Vector2F(0, 0)), m_FixFrame(0), m_AP(0), m_SkillTargets(), m_IsGone(FALSE)
{

}

PArrow::PArrow(int AP)
	: m_IsSkill(FALSE), m_TargetVec(Vector2F(0, 0)), m_FixFrame(0), m_AP(AP), m_SkillTargets(), m_IsGone(FALSE)
{

}

PArrow::~PArrow()
{

}

bool PArrow::CheckCollision(vector<Enemy*> enemies)
{
	if (m_IsLoaded == TRUE || enemies.empty() == TRUE)
	{
		return FALSE;
	}

	for (unsigned int i = 0; i < enemies.size(); i++)
	{
		if (enemies[i]->GetIsDead() == TRUE || enemies[i]->_IsActive == FALSE)
			continue;

		//화살하나가 모든 몬스터와 부딫혔는지 검사한다.
		if (g_D2DEngine.IsCollisionAABB(_RectCollider, enemies[i]->_RectCollider) == TRUE)
		{

			if (m_IsSkill == FALSE)
			{
				_IsCollision = TRUE;
				enemies[i]->Attacked(m_AP);

				if (_Image->CurrentFrameX == _Image->MaxFrameX - 1)
				{
					Reset();
				}
			}
			else
			{
				//스킬어택일때
				//첫번째 인자면 확인과정없이 바로 때려
				if (m_SkillTargets.empty() == TRUE)
				{
					enemies[i]->Attacked(m_AP);
				}
				else
				{
					//중복공격을 막기위한 장치
					bool _FirstColl = TRUE;
					for (int k = 0; k < m_SkillTargets.size(); k++)
					{
						if (m_SkillTargets[k]->GetEnemyNum() == enemies[i]->GetEnemyNum())
						{
							_FirstColl = FALSE;
							break;
						}

					}
					if (_FirstColl == TRUE)
						enemies[i]->Attacked(m_AP * ARCH_SKILLATT);
				}
				m_SkillTargets.push_back(enemies[i]);

#ifdef _DEBUG
				cout << "궁수 스킬 맞은 enemy[" << enemies[i]->GetEnemyNum() << "]" << endl;
#endif
			}
			return TRUE;
		}
	}
	return FALSE;
}


void PArrow::SetDegreeVec(float rotateDegree)
{
	m_TargetVec.x = cosf(rotateDegree * PI / 180.f) * m_TargetVec.x + sinf(rotateDegree * PI / 180.f) * m_TargetVec.y;
	m_TargetVec.y = (-sinf(rotateDegree * PI / 180.f)) * m_TargetVec.x + cosf(rotateDegree * PI / 180.f) * m_TargetVec.y;

	m_TargetNVec.x = m_TargetVec.x / sqrtf(m_TargetVec.x * m_TargetVec.x + m_TargetVec.y * m_TargetVec.y);
	m_TargetNVec.y = m_TargetVec.y / sqrtf(m_TargetVec.x * m_TargetVec.x + m_TargetVec.y * m_TargetVec.y);
}

void PArrow::SetDirVec(D2D1_VECTOR_2F targetPos)
{
	m_TargetVec.x = targetPos.x - _Transform._Position.x;
	m_TargetVec.y = targetPos.y - _Transform._Position.y;

	//를 거리로 나눈 만큼 방향벡타
	m_TargetNVec.x = m_TargetVec.x / sqrtf(m_TargetVec.x * m_TargetVec.x + m_TargetVec.y * m_TargetVec.y);
	m_TargetNVec.y = m_TargetVec.y / sqrtf(m_TargetVec.x * m_TargetVec.x + m_TargetVec.y * m_TargetVec.y);
}

HRESULT PArrow::Init()
{
	_Image = g_D2DEngine.LoadFrameImage("Resource/FrameImageSample/Arrow_TotalAni.png", 8, 8);
	_Image->CurrentFrameX = 0;
	_Image->CurrentFrameY = 0;
	_Image->AnimationSpeed = 0.5f;

	_Transform._Position = Vector2F(0.f, 0.f);
	m_FixFrame = 2;
	RectColliderSetting(SizeF(20.f, 20.f), _CAMERAMANAGER->GetCamPos());
	_Transform._Scale = SizeF(0.3f, 0.3f);
	m_IsSkill = FALSE;
	m_Velo = ARCH_ARRVELO;
	m_SkillTargets.clear();
	m_IsGone = FALSE;
	return S_OK;
}

void PArrow::Release()
{

}

void PArrow::Reset()
{
	m_IsLoaded = TRUE;
	_Transform._Position = Vector2F(0.f, 0.f);
	_IsCollision = FALSE;
	m_TargetVec = Vector2F(0.f, 0.f);
	m_TargetNVec = Vector2F(0.f, 0.f);
	m_Dir = P_DIR::LEFT;
	m_RotateAngle = 0.f;
	m_IsSkill = FALSE;
	m_IsGone = FALSE;

	RectColliderSetting(SizeF(20.f, 20.f), _CAMERAMANAGER->GetCamPos());

	_Image->CurrentFrameY = 0;
	_Image->CurrentFrameX = 0;

	m_SkillTargets.clear();

}

void PArrow::Update()
{
	if (m_IsLoaded == FALSE)
	{
		MoveTo();
		RectColliderSetting(SizeF(20.f, 20.f), _CAMERAMANAGER->GetCamPos());

		//일반공격일때
		if (((_Transform._Position.x >= _CAMERAMANAGER->_MoveMax.x - 100.f || _Transform._Position.x < _CAMERAMANAGER->_MoveMin.x + 100.f)
			|| (_Transform._Position.y > _CAMERAMANAGER->_MoveMax.y - 100.f || _Transform._Position.y < _CAMERAMANAGER->_MoveMin.y + 100.f))
			&& m_IsSkill == FALSE)
		{
			if (m_IsGone == FALSE)
			{
				_Image->CurrentFrameX = 0;
				m_Dir == P_DIR::LEFT ?
					_Image->CurrentFrameY = 2 : _Image->CurrentFrameY = 3;

				m_FixFrame = 2;
				_Image->MaxFrameX = 3;
			}

			m_IsGone = TRUE;

			if ((_Transform._Position.x >= _CAMERAMANAGER->_MoveMax.x || _Transform._Position.x < _CAMERAMANAGER->_MoveMin.x)
				|| (_Transform._Position.y > _CAMERAMANAGER->_MoveMax.y || _Transform._Position.y < _CAMERAMANAGER->_MoveMin.y))
			{
				if (_Image->CurrentFrameX == _Image->MaxFrameX - 1)
				{
					Reset();
				}
			}
		}

		//스킬일때
		if (((_Transform._Position.x >= _CAMERAMANAGER->_MoveMax.x - 200.f || _Transform._Position.x < _CAMERAMANAGER->_MoveMin.x + 200.f)
			|| (_Transform._Position.y > _CAMERAMANAGER->_MoveMax.y - 200.f || _Transform._Position.y < _CAMERAMANAGER->_MoveMin.y + 200.f))
			&& m_IsSkill == TRUE)
		{
			if (m_IsGone == FALSE)
			{
				_Image->CurrentFrameX = 0;
				_Image->AnimationSpeed = 0.1f;

				m_Dir == P_DIR::LEFT ?
					_Image->CurrentFrameY = 6 : _Image->CurrentFrameY = 7;

				m_FixFrame = 7;
				_Image->MaxFrameX = 8;
			}

			m_IsGone = TRUE;

			if ((_Transform._Position.x >= _CAMERAMANAGER->_MoveMax.x || _Transform._Position.x < _CAMERAMANAGER->_MoveMin.x)
				|| (_Transform._Position.y > _CAMERAMANAGER->_MoveMax.y || _Transform._Position.y < _CAMERAMANAGER->_MoveMin.y))
			{
				if (_Image->CurrentFrameX == _Image->MaxFrameX - 1)
				{
					Reset();
				}
			}
		}
	}

}

void PArrow::Render()
{
	if (m_IsLoaded == FALSE)
	{
		g_D2DEngine.DrawFrameImageOnce(_CAMERAMANAGER->GetCamPos(), _TIMEMANAGER->GetOneFrameElapsedTime(), _Transform, _Image, TRUE, m_FixFrame);
	}
}

void PArrow::Shoot(D2D1_VECTOR_2F targetPos)
{
	m_IsSkill = FALSE;
	ProjectileObject::Shoot(targetPos);
	_Image->AnimationSpeed = 0.05f;
	m_FixFrame = 2;
	if (m_TargetNVec.x <= 0)
	{
		_Image->CurrentFrameY = 0;
		_Image->MaxFrameX = 3;
	}
	else
	{
		_Image->CurrentFrameY = 1;
		_Image->MaxFrameX = 3;
	}
}

void PArrow::MoveTo()
{
	ProjectileObject::MoveTo();
}

void PArrow::Shoot_S()
{
	m_IsSkill = TRUE;

	m_IsLoaded = FALSE;
	m_RotateAngle = acosf(m_TargetVec.x / sqrtf(m_TargetVec.x * m_TargetVec.x + m_TargetVec.y * m_TargetVec.y)) * 180.f / PI;

	/*m_TargetVec = targetVec;
	m_TargetNVec.x = m_TargetVec.x / sqrtf(m_TargetVec.x * m_TargetVec.x + m_TargetVec.y * m_TargetVec.y);
	m_TargetNVec.y = m_TargetVec.y / sqrtf(m_TargetVec.x * m_TargetVec.x + m_TargetVec.y * m_TargetVec.y);*/

	if (m_TargetVec.x <= 0)
	{
		//애니메이션으로 변동시 위치 이동필요
		m_Dir = P_DIR::LEFT;
		_Image->CurrentFrameY = 4;
		m_FixFrame = 3;
		_Image->MaxFrameX = 4;

		m_TargetVec.y <= 0 ?
			m_RotateAngle = 180.f - m_RotateAngle : m_RotateAngle = m_RotateAngle - 180.f;
	}
	else
	{
		m_Dir = P_DIR::RIGHT;
		_Image->CurrentFrameY = 5;
		m_FixFrame = 3;
		_Image->MaxFrameX = 4;

		m_TargetVec.y <= 0 ?
			m_RotateAngle = 360.f - m_RotateAngle : m_RotateAngle = m_RotateAngle;
	}

	_Transform.ResetRotationAngle();
	_Transform.SetRotationAngle(m_RotateAngle);

	cout << _Transform.GetRotationAngle() << endl;
}
