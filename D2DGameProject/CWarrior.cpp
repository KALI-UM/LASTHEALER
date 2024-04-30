#include "pch.h"

#include "Companion.h"
#include "Enemy.h"
#include "PSwordAura.h"
#include "CWarrior.h"

CWarrior::CWarrior()
	:Companion::Companion("전사"), m_SwodAura{}
{
	for (int i = 0; i < SAURACNT; i++)
	{
		m_SwodAura[i] = nullptr;
	}
}

CWarrior::~CWarrior()
{

}



void CWarrior::CheckSwordAura(vector<Enemy*> enemies)
{
	for (int i = 0; i < SAURACNT; i++)
	{
		if (m_SwodAura[i]->GetIsLoaded() == FALSE)
		{
			m_SwodAura[i]->CheckCollision(enemies);
		}
	}
}

void CWarrior::Move()
{
	Companion::Move();
}

void CWarrior::FindMonster(vector<Enemy*> enemies)
{
	CheckSwordAura(enemies);
	Companion::FindMonster(enemies);
}

void CWarrior::Attack()
{
	if (m_CurrTarget == nullptr)
	{
		m_State = C_STATE::FIND;
		_Image->CurrentFrameX = 0;
		return;
	}
	float _charsingTime = WARR_ATTCOOL;
	static float _waitForArr = 0;

	_waitForArr += _TIMEMANAGER->GetOneFrameElapsedTime();

	if (_waitForArr >= _charsingTime)
	{
		//프레임 넘버 몇번몇번일때 충돌체크를 한다
		if (_Image->CurrentFrameX == 5)
		{
			m_CurrTarget->Attacked(m_AP);
			_waitForArr = 0.0f;
		}
	}

	if (_Image->CurrentFrameX == _Image->MaxFrameX - 1)
	{
		_Transform._Position.x - m_CurrTarget->_Transform._Position.x >= 0 ?
			m_Dir = C_DIR::LEFT : m_Dir = C_DIR::RIGHT;
	}

	if (((powf((_Transform._Position.x - m_CurrTarget->_Transform._Position.x), 2)
		+ powf((_Transform._Position.y - m_CurrTarget->_Transform._Position.y), 2)) > m_Range
		&& _Image->CurrentFrameX == _Image->MaxFrameX - 1) || m_CurrTarget->GetIsDead() == TRUE)
	{
		//_Image->CurrentFrameX = 0;
		m_State = C_STATE::FIND;
	}
}

bool CWarrior::Passive()
{
	//체력이 N% 이하로 내려가면 공격력 x만큼 증가
	if (m_HP <= m_HPMax * WARR_PASSIVEHP / 100)
	{
		m_AP = m_OriginAP + WARR_PASSIVEAP;
		m_Passive = TRUE;
		return TRUE;
	}

	else
	{
		m_AP = m_OriginAP;
		m_Passive = FALSE;
		return FALSE;
	}
}


void CWarrior::SkillAttack()
{
	if (_Image->CurrentFrameX == 6 && m_IsSkillActive == TRUE)
	{
		for (int i = 0; i < SAURACNT; i++)
		{
			if (m_SwodAura[i]->GetIsLoaded() == TRUE)
			{
				m_SwodAura[i]->SetPos(_Transform._Position);
				m_SwodAura[i]->Shoot(m_CurrTarget->_Transform._Position);
				break;
			}
		}

		m_IsSkillActive = FALSE;
		m_SkillCool = 0.f;
	}
	//애니메이션 마지막장이 되면 스테이트를 바꽈바꽈 모든걸 다박꽈
	//여튼 다시 원래 FSM으로 돌려준다.
	if (_Image->CurrentFrameX == _Image->MaxFrameX - 1)
	{
		_Image->CurrentFrameX = 0;
		m_State = C_STATE::IDLE;
	}
}

HRESULT CWarrior::Init()
{
	m_Kind = PersonKind::Warrior;
	_IsActive = TRUE;
	_Image = g_D2DEngine.LoadFrameImage("Resource/FrameImageSample/Warior_.png", 16, 7);
	_Image->CurrentFrameY = 0;
	_Transform._Position = Vector2F((float)(GetSystemMetrics(SM_CXSCREEN) / 2 + 256.f), (float)(GetSystemMetrics(SM_CYSCREEN) / 2));
	_Transform._Scale = SizeF(0.5f, 0.5f);
	GameObject::RectColliderSetting(SizeF(RECTSIZE, RECTSIZE), _CAMERAMANAGER->GetCamPos());


	m_IsDead = FALSE;
	m_Passive = FALSE;
	m_Dir = C_DIR::LEFT;
	m_State = C_STATE::IDLE;
	m_CurrTarget = nullptr;

	m_HPMax = WARR_HP;
	m_HP = m_HPMax;
	m_DP = 10;
	m_OriginAP = WARR_AP;
	m_AP = m_OriginAP;
	m_Velo = WARR_VELO;
	m_Range = (int)pow(256 * WARR_ATTRANGE, 2);

	for (int i = 0; i < SAURACNT; i++)
	{
		m_SwodAura[i] = new PSwordAura();
		m_SwodAura[i]->Init();
	}

	return S_OK;
}

void CWarrior::Release()
{
	for (int i = 0; i < SAURACNT; i++)
	{
		SAFE_RELEASE(m_SwodAura[i]);
	}
}

void CWarrior::Reset()
{
	_Image = g_D2DEngine.LoadFrameImage("Resource/FrameImageSample/Warrior_TotalAni.png", 16, 7);
	_Image->CurrentFrameY = 0;
	_Image->CurrentFrameX = 0;
	_Transform._Position = Vector2F((float)(GetSystemMetrics(SM_CXSCREEN) / 2 + 256.f), (float)(GetSystemMetrics(SM_CYSCREEN) / 2));
	GameObject::RectColliderSetting(SizeF(RECTSIZE, RECTSIZE), _CAMERAMANAGER->GetCamPos());

	m_AP = m_OriginAP;
	m_Velo = WARR_VELO;
	m_Passive = FALSE;
	m_Dir = C_DIR::LEFT;
	m_State = C_STATE::IDLE;
	m_CurrTarget = nullptr;
	m_IsSkillActive = FALSE;
	m_SkillCool = 0.f;

	for (int i = 0; i < SAURACNT; i++)
	{
		m_SwodAura[i]->Reset();
	}
}

void CWarrior::Update()
{
	//m_SkillCool += _TIMEMANAGER->GetOneFrameElapsedTime();
	//if (m_SkillCool >= WARR_SKILLCOOL)
	//{
	//	m_IsSkillActive = TRUE;
	//}

	//if (_KEYMANAGER->KeyDown(VK_SPACE))
	//{
	//}
	//매 프레임마다 죽었는지 확인한다
	for (int i = 0; i < SAURACNT; i++)
	{
		if (m_SwodAura[i]->GetIsLoaded() == FALSE)
		{
			m_SwodAura[i]->Update();
		}
	}
	if (m_HP <= 0)
	{
		m_State = C_STATE::DEAD;
		m_IsDead = TRUE;
	}

	if (m_IsDead == TRUE)
	{
		return;
	}

	if (m_IsSkillActive == TRUE)
	{
		m_State = C_STATE::SKILLATTACK;
	}


	if (m_IsSet == TRUE)
	{
		m_State = C_STATE::MOVETO;
	}

	m_Passive = Passive();

	switch (m_State)
	{
	case C_STATE::DEAD:
	{

	}
	break;
	case C_STATE::SKILLATTACK:
	{
		SkillAttack();
		//y축 기준 몇번째 이미지인지, 총 몇 프레임짜리 애니메이션인지
		m_Dir == C_DIR::LEFT ?
			_Image->CurrentFrameY = 5 : _Image->CurrentFrameY = 6;
		//	오른쪽일때					//왼쪽일때
		_Image->MaxFrameX = 16;
	}
	break;
	case C_STATE::MOVE:
	{
		Move();
		m_Dir == C_DIR::LEFT ?
			_Image->CurrentFrameY = 1 : _Image->CurrentFrameY = 2;
		_Image->MaxFrameX = 7;
	}
	break;
	case C_STATE::ATTACK:
	{
		Attack();
		m_Dir == C_DIR::LEFT ?
			_Image->CurrentFrameY = 3 : _Image->CurrentFrameY = 4;
		_Image->MaxFrameX = 16;
	}
	break;
	case C_STATE::IDLE:
	{
		Idle();
		_Image->CurrentFrameY = 0;
		_Image->MaxFrameX = 2;
	}
	break;
	case C_STATE::MOVETO:
	{
		MoveTo(_GAMEMANAGER->_PlayerPosition);
		_Image->MaxFrameX = 7;
	}
	break;
	}

	GameObject::RectColliderSetting(SizeF(RECTSIZE, RECTSIZE), _CAMERAMANAGER->GetCamPos());

}

void CWarrior::Render()
{
	for (int i = 0; i < SAURACNT; i++)
	{
		if (m_SwodAura[i]->GetIsLoaded() == FALSE)
		{
			m_SwodAura[i]->Render();
		}
	}
	if (m_IsDead == FALSE && _IsActive == TRUE)
		g_D2DEngine.DrawFrameImage(_CAMERAMANAGER->GetCamPos(), _TIMEMANAGER->GetOneFrameElapsedTime(), _Transform, _Image);

#ifdef _DEBUG
	if (_GAMEMANAGER->GetShowDataInfo())
	{
		g_D2DEngine.DrawTextOut("HP : %d", m_HP, _Image->ShowRect.right, _Image->ShowRect.top - 20.f, BrushColor::Yellow);
		g_D2DEngine.DrawTextOut("Warrior", _Image->ShowRect.right, _Image->ShowRect.top, BrushColor::Yellow);
		switch (m_State)
		{
		case C_STATE::IDLE:
			g_D2DEngine.DrawTextOut("State : IDLE", _Image->ShowRect.right, _Image->ShowRect.top + 20.0f, BrushColor::Yellow);
			break;
		case C_STATE::MOVE:
			g_D2DEngine.DrawTextOut("State : MOVE", _Image->ShowRect.right, _Image->ShowRect.top + 20.0f, BrushColor::Yellow);
			break;
		case C_STATE::ATTACK:
			g_D2DEngine.DrawTextOut("State : ATTACK", _Image->ShowRect.right, _Image->ShowRect.top + 20.0f, BrushColor::Yellow);
			break;
		case C_STATE::SKILLATTACK:
			g_D2DEngine.DrawTextOut("State : SKILLATTACK", _Image->ShowRect.right, _Image->ShowRect.top + 20.0f, BrushColor::Yellow);
			break;
		case C_STATE::FIND:
			g_D2DEngine.DrawTextOut("State : FIND", _Image->ShowRect.right, _Image->ShowRect.top + 20.0f, BrushColor::Yellow);
			break;
		case C_STATE::DEAD:
			g_D2DEngine.DrawTextOut("State : DEAD", _Image->ShowRect.right, _Image->ShowRect.top + 20.0f, BrushColor::Yellow);
			break;
		}

		if (m_IsSkillActive == TRUE)
		{
			g_D2DEngine.DrawTextOut("SkillActive : True", _Image->ShowRect.right, _Image->ShowRect.top + 40.0f, BrushColor::Yellow);
		}
		else
		{
			g_D2DEngine.DrawTextOut("SkillActive : False", _Image->ShowRect.right, _Image->ShowRect.top + 40.0f, BrushColor::Yellow);
		}
		g_D2DEngine.DrawTextOut("SkillCool : %2f", m_SkillCool, _Image->ShowRect.right, _Image->ShowRect.top + 60.0f, BrushColor::Yellow);
		if (m_Passive == TRUE)
		{
			g_D2DEngine.DrawTextOut("Passive : True", _Image->ShowRect.right, _Image->ShowRect.top + 80.0f, BrushColor::Yellow);
		}
		else
		{
			g_D2DEngine.DrawTextOut("Passive : False", _Image->ShowRect.right, _Image->ShowRect.top + 80.0f, BrushColor::Yellow);
			g_D2DEngine.DrawTextOut("CurrFrmaeX : %d", _Image->CurrentFrameX, _Image->ShowRect.right, _Image->ShowRect.top + 100.0f, BrushColor::Yellow);
		}
	}
#endif
}
