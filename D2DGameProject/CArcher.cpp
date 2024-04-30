#include "pch.h"
#include "Companion.h"
#include "Enemy.h"
#include "PArrow.h"
#include "CArcher.h"

CArcher::CArcher()
	:Companion::Companion("궁수"), m_ArrPoint(), m_Arrows{}
{
	m_ArrPoint.x = 0;
	m_ArrPoint.y = 0;

	for (int i = 0; i < ARROWCNT; i++)
	{
		m_Arrows[i] = nullptr;
	}
}

CArcher::~CArcher()
{

}

void CArcher::Move()
{
	Companion::Move();
	m_ArrPoint.x = _Transform._Position.x;
	m_ArrPoint.y = _Transform._Position.y;
}

void CArcher::FindMonster(vector<Enemy*> enemies)
{
	CheckArrows(enemies);
	Companion::FindMonster(enemies);
}

void CArcher::CheckArrows(vector<Enemy*> enemies)
{
	for (int i = 0; i < ARROWCNT; i++)
	{
		m_Arrows[i]->CheckCollision(enemies);
	}
}

void CArcher::Attack()
{
	if (m_CurrTarget == nullptr)
	{
		m_State = C_STATE::FIND;
		_Image->CurrentFrameX = 0;
		return;
	}

	float _charsingTime = ARCH_ATTCOOL;
	static float _waitForArr = 0;

	_waitForArr += _TIMEMANAGER->GetOneFrameElapsedTime();

	if (_waitForArr >= _charsingTime && _Image->CurrentFrameX == 14)
	{
		m_Passive = Passive();

		for (int i = 0; i < ARROWCNT; i++)
		{
			if (m_Arrows[i]->GetIsLoaded() == TRUE)
			{
				m_Arrows[i]->SetPos(m_ArrPoint);
				m_Arrows[i]->Shoot(m_CurrTarget->_Transform._Position);

				break;
			}
		}
		PassiveOff();

		_waitForArr = 0.0f;
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

bool CArcher::Passive()
{
	m_OriginAP = m_AP;
	if (ProbCalculator(ARCH_PASSIVEPR) == TRUE)
	{
		float _tempAP = m_AP * ARCH_PASSIVEAP;
		m_AP = (int)_tempAP;
		m_Passive = TRUE;
		return TRUE;
	}
	return FALSE;
}

void CArcher::PassiveOff()
{
	m_AP = m_OriginAP;
	m_Passive = FALSE;
}

void CArcher::SkillAttack()
{
	if (_Image->CurrentFrameX == 12 && m_IsSkillActive == TRUE)
	{
		for (int i = 0; i < ARROWCNT; i++)
		{
			if (m_Arrows[i]->GetIsLoaded() == TRUE)
			{
				m_Arrows[i]->SetPos(m_ArrPoint);
				m_Arrows[i]->SetDirVec(m_CurrTarget->_Transform._Position);
				m_Arrows[i]->Shoot_S();
#ifdef _DEBUG
				cout << "SKILLARROW ANGLE[" << i << "] : " << m_Arrows[i]->GetRAngle() << endl;
#endif
				break;
			}
		}
		for (int i = 0; i < ARROWCNT; i++)
		{
			if (m_Arrows[i]->GetIsLoaded() == TRUE)
			{
				m_Arrows[i]->SetPos(m_ArrPoint);
				m_Arrows[i]->SetDirVec(m_CurrTarget->_Transform._Position);
				m_Arrows[i]->SetDegreeVec(30.f);
				m_Arrows[i]->Shoot_S();
#ifdef _DEBUG
				cout << "SKILLARROW ANGLE[" << i << "] : " << m_Arrows[i]->GetRAngle() << endl;
#endif
				break;
			}
		}
		for (int i = 0; i < ARROWCNT; i++)
		{
			if (m_Arrows[i]->GetIsLoaded() == TRUE)
			{
				m_Arrows[i]->SetPos(m_ArrPoint);
				m_Arrows[i]->SetDirVec(m_CurrTarget->_Transform._Position);
				m_Arrows[i]->SetDegreeVec(360.f - 30.f);
				m_Arrows[i]->Shoot_S();
#ifdef _DEBUG
				cout << "SKILLARROW ANGLE[" << i << "] : " << m_Arrows[i]->GetRAngle() << endl;
#endif
				break;
			}
		}
		for (int i = 0; i < ARROWCNT; i++)
		{
			if (m_Arrows[i]->GetIsLoaded() == TRUE)
			{
				m_Arrows[i]->SetPos(m_ArrPoint);
				m_Arrows[i]->SetDirVec(m_CurrTarget->_Transform._Position);
				m_Arrows[i]->SetDegreeVec(15.f);
				m_Arrows[i]->Shoot_S();
#ifdef _DEBUG
				cout << "SKILLARROW ANGLE[" << i << "] : " << m_Arrows[i]->GetRAngle() << endl;
#endif
				break;
			}
		}
		for (int i = 0; i < ARROWCNT; i++)
		{
			if (m_Arrows[i]->GetIsLoaded() == TRUE)
			{
				m_Arrows[i]->SetPos(m_ArrPoint);
				m_Arrows[i]->SetDirVec(m_CurrTarget->_Transform._Position);
				m_Arrows[i]->SetDegreeVec(360.f - 15.f);
				m_Arrows[i]->Shoot_S();
#ifdef _DEBUG
				cout << "SKILLARROW ANGLE[" << i << "] : " << m_Arrows[i]->GetRAngle() << endl;
#endif
				break;
			}
		}
		m_IsSkillActive = FALSE;
		m_SkillCool = 0.f;
	}
}

HRESULT CArcher::Init()
{
	m_Kind = PersonKind::Archer;
	_IsActive = TRUE;
	// 프레임 이미지 로드
	_Image = g_D2DEngine.LoadFrameImage("Resource/FrameImageSample/Archer_TotalAni.png", 22, 7);
	//_Image = g_D2DEngine.LoadSingleImage("Resource/SingleImageSample/x.png");
	// 현재보여줄 y축 프레임이미지 초기화
	_Image->CurrentFrameY = 0;
	// 위치좌표 초기화
	_Transform._Position = Vector2F((float)(GetSystemMetrics(SM_CXSCREEN) / 2), (float)(GetSystemMetrics(SM_CYSCREEN) / 2));
	_Transform._Scale = SizeF(0.5f, 0.5f);
	GameObject::RectColliderSetting(SizeF(RECTSIZE, RECTSIZE), _CAMERAMANAGER->GetCamPos());
	_Image->AnimationSpeed = 0.1f;
	// 이미지 크기 배 수 초기화 (디폴트: 1.0f)
	//_Transform._Scale = SizeF(2.0f, 2.0f);
	// 애니메이션 재생 속도 초기화 (디폴트: 0.1f, 낮을수록 재생속도가 빨리진다)
	//_Image->AnimationSpeed = 0.05f;
	m_ArrPoint.x = _Transform._Position.x;
	m_ArrPoint.y = _Transform._Position.y - 5.0f;
	m_IsDead = FALSE;
	m_Dir = C_DIR::LEFT;
	m_State = C_STATE::IDLE;
	m_CurrTarget = nullptr;
	m_HPMax = ARCH_HP;
	m_HP = m_HPMax;
	m_DP = 10;
	m_AP = ARCH_AP;
	m_OriginAP = m_AP;
	m_Velo = ARCH_VELO;
	//m_Range = (int)pow(256 * 1, 2);
	m_Range = (int)pow(256 * ARCH_ATTRANGE, 2);
	m_IsSkillActive = FALSE;
	m_SkillCool = 0.f;


	for (int i = 0; i < ARROWCNT; i++)
	{
		m_Arrows[i] = new PArrow(m_AP);
		m_Arrows[i]->Init();
	}

	return S_OK;
}

void CArcher::Release()
{

}

void CArcher::Reset()
{
	_Image->CurrentFrameY = 0;
	_Image->CurrentFrameX = 0;
	_Image = g_D2DEngine.LoadFrameImage("Resource/FrameImageSample/Archer_TotalAni.png", 22, 7);
	_Transform._Position = Vector2F((float)(GetSystemMetrics(SM_CXSCREEN) / 2), (float)(GetSystemMetrics(SM_CYSCREEN) / 2));
	GameObject::RectColliderSetting(SizeF(RECTSIZE, RECTSIZE), _CAMERAMANAGER->GetCamPos());
	_Image->AnimationSpeed = 0.1f;

	m_AP = m_OriginAP;
	m_Velo = ARCH_VELO;
	m_Passive = FALSE;
	m_Dir = C_DIR::LEFT;
	m_State = C_STATE::IDLE;
	m_CurrTarget = nullptr;
	m_IsSkillActive = FALSE;
	m_SkillCool = 0.f;
	m_ArrPoint = Vector2F(_Transform._Position.x, _Transform._Position.y + 5.0f);
	for (int i = 0; i < ARROWCNT; i++)
	{
		m_Arrows[i]->Reset();
		m_Arrows[i]->SetAP(m_AP);
	}
}

void CArcher::Update()
{
	for (int i = 0; i < ARROWCNT; i++)
	{
		m_Arrows[i]->Update();
	}

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
	//m_SkillCool += _TIMEMANAGER->GetOneFrameElapsedTime();
	//if (m_SkillCool >= ARCH_SKILLCOOL)
	//{
	//	m_IsSkillActive = TRUE;
	//}

	////추후에  UI나오면 변경
	//if (_KEYMANAGER->KeyDown(VK_SPACE) == TRUE)
	//{
	//}
	if (m_IsSkillActive == TRUE)
	{
		m_State = C_STATE::SKILLATTACK;
	}

	if (m_IsSet == TRUE)
	{
		m_State = C_STATE::MOVETO;
	}

	//매 업데이트마다 화살이 어디서 나가는 지 바꿔준다.
	//이미지에서 어느정도 떨어졌는지 지정해주기
	m_Dir == C_DIR::LEFT ?
		m_ArrPoint = Vector2F(_Transform._Position.x - 10.f, _Transform._Position.y + 5.0f) :
		m_ArrPoint = Vector2F(_Transform._Position.x + 10.f, _Transform._Position.y + 5.0f);


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
			m_State = C_STATE::IDLE;
		}

		//y축 기준 몇번째 이미지인지, 총 몇 프레임짜리 애니메이션인지
		m_Dir == C_DIR::LEFT ?
			_Image->CurrentFrameY = 5 : _Image->CurrentFrameY = 6;
		//	오른쪽일때					//왼쪽일때
		_Image->MaxFrameX = 22;
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
		_Image->MaxFrameX = 22;
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

void CArcher::Render()
{
	for (int i = 0; i < ARROWCNT; i++)
	{
		if (m_Arrows[i]->GetIsLoaded() == FALSE)
		{
			m_Arrows[i]->Render();
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

				// 오브젝트 정보 텍스트 그리기
				g_D2DEngine.DrawTextOut("Arrow", m_Arrows[i]->_Image->ShowRect.right, m_Arrows[i]->_Image->ShowRect.top, BrushColor::Yellow);
				g_D2DEngine.DrawTextOut("_Transform.GetRotationAngle(): %.2f", m_Arrows[i]->_Transform.GetRotationAngle(), m_Arrows[i]->_Image->ShowRect.right, m_Arrows[i]->_Image->ShowRect.top + 20.0f, BrushColor::White);
			}
#endif
		}
	}

	if (m_IsDead == FALSE && _IsActive == TRUE)
	{
		g_D2DEngine.DrawFrameImage(_CAMERAMANAGER->GetCamPos(), _TIMEMANAGER->GetOneFrameElapsedTime(), _Transform, _Image);
	}
#ifdef _DEBUG
	if (_GAMEMANAGER->GetShowDataInfo())
	{
		g_D2DEngine.DrawTextOut("HP : %d", m_HP, _Image->ShowRect.right, _Image->ShowRect.top - 20.f, BrushColor::Yellow);
		g_D2DEngine.DrawTextOut("Archer", _Image->ShowRect.right, _Image->ShowRect.top, BrushColor::Yellow);
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
	}

#endif

}


