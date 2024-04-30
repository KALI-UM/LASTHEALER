#include "pch.h"
#include "Enemy.h"
#include "PSwordAura.h"

PSwordAura::PSwordAura()
	:m_FixFrame(0), m_SkillTargets(), m_AP(0), m_IsGone(FALSE)
{

}
PSwordAura::PSwordAura(int AP)
	: m_FixFrame(0), m_SkillTargets(), m_AP(AP), m_IsGone(FALSE)
{

}

PSwordAura::~PSwordAura()
{

}

bool PSwordAura::CheckCollision(vector<Enemy*> enemies)
{
	for (int i = 0; i < enemies.size(); i++)
	{
		if (enemies[i]->GetIsDead() == TRUE)
			continue;

		if (g_D2DEngine.IsCollisionIntersect(enemies[i]->_RectCollider, _CircleCollider) == TRUE)
		{
			//첫번째 인자면 확인과정없이 바로 때려
			if (m_SkillTargets.empty() == TRUE)
			{
				enemies[i]->Attacked(m_AP);
			}
			else
			{

				bool _FirstColl = TRUE;
				for (int k = 0; k < m_SkillTargets.size(); k++)
				{
					if (m_SkillTargets[k]->GetEnemyNum() == enemies[i]->GetEnemyNum())
					{
						_FirstColl = FALSE;
						break;
					}
				}
				//몬스터와 상호작용해서 여튼 딜넣으셈!
				if (_FirstColl == TRUE)
					enemies[i]->Attacked(m_AP);
			}
			m_SkillTargets.push_back(enemies[i]);

#ifdef _DEBUG
			cout << "전사 스킬 맞은 enemy[" << i << "]" << endl;
#endif


			return TRUE;
		}
	}
	return FALSE;
}

void PSwordAura::Shoot(D2D1_VECTOR_2F targetPos)
{
	ProjectileObject::Shoot(targetPos);
	m_FixFrame = 3;

	if (m_TargetNVec.x <= 0)
	{
		_Image->CurrentFrameY = 0;
		_Image->MaxFrameX = 4;
	}
	else
	{
		_Image->CurrentFrameY = 1;
		_Image->MaxFrameX = 4;
	}
}

HRESULT PSwordAura::Init()
{
	_Image = g_D2DEngine.LoadFrameImage("Resource/FrameImageSample/Warrior_SkillEffect_Total.png", 8, 4);
	m_FixFrame = 3;
	_Image->CurrentFrameY = 0;
	_Image->CurrentFrameX = 0;
	_Image->AnimationSpeed = 0.1f;
	_Transform._Position = Vector2F(0.f, 0.f);
	CircleInfomationSetting(_CAMERAMANAGER->GetCamPos());
	_Transform._Radius = 5.f;
	_Transform._Scale = SizeF(0.3f, 0.3f);
	m_IsLoaded = TRUE;
	m_IsGone = FALSE;
	m_Velo = WARR_SKILLVELO;
	m_SkillTargets.clear();

	return S_OK;
}

void PSwordAura::Release()
{

}

void PSwordAura::Reset()
{
	_Image->CurrentFrameY = 0;
	_Image->CurrentFrameX = 0;
	m_FixFrame = 3;
	_Image->AnimationSpeed = 0.1f;
	_Transform._Position = Vector2F(0.f, 0.f);
	CircleInfomationSetting(_CAMERAMANAGER->GetCamPos());
	m_IsLoaded = TRUE;
	m_IsGone = FALSE;

}

void PSwordAura::Update()
{
	if (m_IsLoaded == FALSE)
	{
		MoveTo();
		CircleInfomationSetting(_CAMERAMANAGER->GetCamPos());

		//사라지는 범위 확정되면 다시 수정
		//벽에 닿으면

		if (((_Transform._Position.x >= _CAMERAMANAGER->_MoveMax.x - 200.f || _Transform._Position.x < _CAMERAMANAGER->_MoveMin.x + 200.f)
			|| (_Transform._Position.y > _CAMERAMANAGER->_MoveMax.y - 200.f || _Transform._Position.y < _CAMERAMANAGER->_MoveMin.y + 200.f)))
		{
			if (m_IsGone == FALSE)
			{
				_Image->CurrentFrameX = 0;
				_Image->AnimationSpeed = 0.05f;


				m_Dir == P_DIR::LEFT ?
					_Image->CurrentFrameY = 2 : _Image->CurrentFrameY = 3;

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

void PSwordAura::Render()
{
	if (m_IsLoaded == FALSE)
	{
		g_D2DEngine.DrawFrameImageOnce(_CAMERAMANAGER->GetCamPos(), _TIMEMANAGER->GetOneFrameElapsedTime(), _Transform, _Image, TRUE, m_FixFrame);
	}
}


//나는 김흐정~
//
//김! 김이 팔랑거리듯
//흐! 흐느적 거린다항~ 꺄호호호홓
//정! 정말 흐느적흐느적 김흐느정