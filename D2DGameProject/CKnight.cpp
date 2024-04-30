#include "pch.h"
#include "Companion.h"
#include "Enemy.h"
#include "CKnight.h"

CKnight::CKnight()
	:Companion::Companion("기사"),
	m_SkillTargets(), m_AreaEffect(nullptr)
{
}

CKnight::~CKnight()
{

}

void CKnight::Move()
{
	Companion::Move();
}

void CKnight::Attacked(int deal)
{
	m_Passive = Passive();
	if (m_Passive == TRUE)
	{
		return;
	}
	else
		Companion::Attacked(deal);

}

void CKnight::FindMonster(vector<Enemy*> enemies)
{
	//m_SkillTarget은 스킬어택상태일때 외에는 항상 비워둔다
	if (m_State == C_STATE::SKILLATTACK && m_SkillTargets.empty() == TRUE && m_IsSkillActive == TRUE)
	{
		for (unsigned int i = 0; i < enemies.size(); i++)
		{
			if (enemies[i]->GetIsDead() == TRUE)
				continue;

			if (g_D2DEngine.IsCollisionIntersect(enemies[i]->_RectCollider, _CircleCollider) == TRUE)
			{
				m_SkillTargets.push_back(enemies[i]);
#ifdef _DEBUG
				cout << "기사 SkillAttack enemy[" << enemies[i]->GetEnemyNum() << "] 지정" << endl;
#endif
			}
		}
	}
	else
	{
		Companion::FindMonster(enemies);
	}
}

void CKnight::Attack()
{
	if (m_CurrTarget == nullptr)
	{
		m_State = C_STATE::FIND;
		_Image->CurrentFrameX = 0;
		return;
	}
	float _charsingTime = KNIG_ATTCOOL;
	static float _waitForArr = 0;

	_waitForArr += _TIMEMANAGER->GetOneFrameElapsedTime();

	if (_waitForArr >= _charsingTime)
	{

		//프레임 넘버 몇번몇번일때 충돌체크를 한다두루리리둥둥
		if (_Image->CurrentFrameX == 7)
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
		m_State = C_STATE::FIND;
		_Image->CurrentFrameX = 0;
	}
}

bool CKnight::Passive()
{
	if (ProbCalculator(KNIG_PASSIVEPR) == TRUE)
	{
		return TRUE;
	}
	return FALSE;
}

void CKnight::SkillAttack()
{
	if (_Image->CurrentFrameX == 7 && m_IsSkillActive == TRUE && m_SkillTargets.empty() == FALSE)
	{
		if (m_SkillTargets.size() > 0)
		{
			for (unsigned int i = 0; i < m_SkillTargets.size(); i++)
			{
				m_SkillTargets[i]->Attacked(m_AP * KNIG_SKILLATT);
#ifdef _DEBUG
				cout << "기사 SkillAttack enemy[" << m_SkillTargets[i]->GetEnemyNum() << "] 공격" << endl;
#endif
				_EFFECTMANAGER->PlayShot(EffectKind::EFFKNIG_SKILL,
					Vector2F(m_SkillTargets[i]->_Transform._Position.x, m_SkillTargets[i]->_Transform._Position.y - 60.f), 0, SizeF(0.5f, 0.5f), 0.1f);
			}
			m_SkillTargets.clear();

			m_IsSkillActive = FALSE;
			m_SkillCool = 0.f;
		}
	}
	//애니메이션 마지막장이 되면 스테이트를 바꽈바꽈 모든걸 다박꽈
	//여튼 다시 원래 FSM으로 돌려준다.
	if (_Image->CurrentFrameX == _Image->MaxFrameX - 1)
	{
		_Image->CurrentFrameX = 0;
		m_State = C_STATE::IDLE;
	}
}

HRESULT CKnight::Init()
{
	m_Kind = PersonKind::Knight;
	_IsActive = TRUE;
	GameObject::RectColliderSetting(SizeF(RECTSIZE, RECTSIZE), _CAMERAMANAGER->GetCamPos());
	CircleInfomationSetting(_CAMERAMANAGER->GetCamPos());
	_Transform._Radius = 256 * KNIG_SKILLRANGE;
	_Image = g_D2DEngine.LoadFrameImage("Resource/FrameImageSample/Knight_TotalAni.png", 12, 7);
	_EFFECTMANAGER->AddEffect(EffectKind::EFFKNIG_SKILL, "Resource/FrameImageSample/Knight_EffectAni_Sword.png", 1.0f, 8, 1, true);
	_Image->CurrentFrameX = 0;
	_Image->CurrentFrameY = 0;
	_Transform._Position = Vector2F((float)(GetSystemMetrics(SM_CXSCREEN) / 2 - 256.f * 2), (float)(GetSystemMetrics(SM_CYSCREEN) / 2));
	_Transform._Scale = SizeF(0.5f, 0.5f);

	m_IsDead = FALSE;
	m_Passive = FALSE;
	m_Dir = C_DIR::LEFT;
	m_State = C_STATE::IDLE;
	m_CurrTarget = nullptr;

	m_HPMax = KNIG_HP;
	m_HP = m_HPMax;
	m_DP = 10;
	m_OriginAP = KNIG_AP;
	m_AP = m_OriginAP;
	m_Velo = KNIG_VELO;
	m_Range = (int)pow(256 * KNIG_ATTRANGE, 2);
	m_IsSkillActive = FALSE;
	m_SkillCool = 0.f;
	m_SkillTargets.clear();
	m_AreaEffect = new KNIG_AreaEffect();
	m_AreaEffect->Init();

	return S_OK;
}

void CKnight::Release()
{
	SAFE_RELEASE(m_AreaEffect);
}

void CKnight::Reset()
{
	_Image->CurrentFrameY = 0;
	_Image->CurrentFrameX = 0;
	_Transform._Position = Vector2F((float)(GetSystemMetrics(SM_CXSCREEN) / 2 - 256.f * 2), (float)(GetSystemMetrics(SM_CYSCREEN) / 2));
	GameObject::RectColliderSetting(SizeF(RECTSIZE, RECTSIZE), _CAMERAMANAGER->GetCamPos());
	CircleInfomationSetting(_CAMERAMANAGER->GetCamPos());
	m_AP = m_OriginAP;
	m_Velo = KNIG_VELO;
	m_Passive = FALSE;
	m_Dir = C_DIR::LEFT;
	m_State = C_STATE::IDLE;
	m_CurrTarget = nullptr;
	m_IsSkillActive = FALSE;
	m_SkillCool = 0.f;
	m_SkillTargets.clear();
	m_AreaEffect->Reset();
}

void CKnight::Update()
{
	//m_SkillCool += _TIMEMANAGER->GetOneFrameElapsedTime();
	//if (m_SkillCool >= KNIG_SKILLCOOL)
	//{
	//	m_IsSkillActive = TRUE;
	//}

	////추후에  UI나오면 변경
	//if (_KEYMANAGER->KeyDown(VK_SPACE) == TRUE)
	//{
	//}
	//매 프레임마다 죽었는지 확인한다
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
		m_AreaEffect->_IsActive = TRUE;
	}
	if (m_IsSet == TRUE)
	{
		m_State = C_STATE::MOVETO;
	}


	switch (m_State)
	{
	case C_STATE::DEAD:
	{
		Reset();
	}
	break;
	case C_STATE::SKILLATTACK:
	{
		SkillAttack();

		if (_Image->CurrentFrameX == _Image->MaxFrameX - 1)
		{
			m_SkillCool = 0.f;
			m_State = C_STATE::IDLE;
		}

		//y축 기준 몇번째 이미지인지, 총 몇 프레임짜리 애니메이션인지
		m_Dir == C_DIR::LEFT ?
			_Image->CurrentFrameY = 5 : _Image->CurrentFrameY = 6;
		//	오른쪽일때					//왼쪽일때
		_Image->MaxFrameX = 12;
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
		_Image->MaxFrameX = 12;
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

	m_AreaEffect->SetPos(_Transform._Position);
	m_AreaEffect->Update();

	GameObject::RectColliderSetting(SizeF(RECTSIZE, RECTSIZE), _CAMERAMANAGER->GetCamPos());
	CircleInfomationSetting(_CAMERAMANAGER->GetCamPos());
}


void CKnight::Render()
{
	m_AreaEffect->Render();

	if (m_IsDead == FALSE && _IsActive == TRUE)
		g_D2DEngine.DrawFrameImage(_CAMERAMANAGER->GetCamPos(), _TIMEMANAGER->GetOneFrameElapsedTime(), _Transform, _Image);

#ifdef _DEBUG
	if (_GAMEMANAGER->GetShowDataInfo())
	{
		g_D2DEngine.DrawTextOut("HP : %d", m_HP, _Image->ShowRect.right, _Image->ShowRect.top - 20.f, BrushColor::Yellow);
		g_D2DEngine.DrawTextOut("Knight", _Image->ShowRect.right, _Image->ShowRect.top, BrushColor::Yellow);
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
		}
		g_D2DEngine.DrawTextOut("Current FN : %d", _Image->CurrentFrameX, _Image->ShowRect.right, _Image->ShowRect.top + 100.0f, BrushColor::Yellow);

	}
#endif

}
