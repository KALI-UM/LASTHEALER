#include "pch.h"
#include "Enemy.h"
#include "CAssassin.h"

CAssassin::CAssassin()
	:Companion::Companion("�ϻ���"), m_FixFrame(0), m_PrevState(C_STATE::IDLE)
{

}

CAssassin::~CAssassin()
{

}

void CAssassin::Idle()
{
	Companion::Idle();
	_Image->AnimationSpeed = 0.1f;

}

void CAssassin::Move()
{
	if (m_CurrTarget == nullptr || m_CurrTarget->GetIsDead() == TRUE)
	{
		m_State = C_STATE::FIND;
		return;
	}

	if (_Image->CurrentFrameX <= 8)
	{
		//���ڸ���
	}
	else
	{
		if (_Image->CurrentFrameX == 0 || _Image->CurrentFrameX == _Image->MaxFrameX)
		{
			//���� ������
			_Image->CurrentFrameX = 9;
		}

		D2D1_VECTOR_2F _DirVec;

		//Ÿ�ٰ� ���� �Ÿ� �� x, y
		float _dx = m_CurrTarget->_Transform._Position.x - _Transform._Position.x;
		float _dy = m_CurrTarget->_Transform._Position.y - _Transform._Position.y;

		//�� �Ÿ��� ���� ��ŭ
		_DirVec.x = _dx / sqrtf(_dx * _dx + _dy * _dy);
		_DirVec.y = _dy / sqrtf(_dx * _dx + _dy * _dy);

		_Transform._Position.x += _DirVec.x * m_Velo;
		_Transform._Position.y += _DirVec.y * m_Velo;

		//���⺤�Ͱ� 
		_DirVec.x <= 0 ?
			m_Dir = C_DIR::LEFT : m_Dir = C_DIR::RIGHT;


		if ((_dx * _dx + _dy * _dy) <= m_Range && _Image->CurrentFrameX == 9)
		{
			m_State = C_STATE::ATTACK;
			//_Image->CurrentFrameX = 0;
		}
	}
}

void CAssassin::Attack()
{
	if (m_CurrTarget == nullptr)
	{
		m_State = C_STATE::FIND;
		return;
	}
	float _charsingTime = ASSA_AP;
	static float _waitForArr = 0;

	_waitForArr += _TIMEMANAGER->GetOneFrameElapsedTime();

	if (_waitForArr >= _charsingTime)
	{
		//������ �ѹ� �������϶� ���� �Ѵ�
		if (_Image->CurrentFrameX == 7 && (_Image->CurrentFrameY == 3 || _Image->CurrentFrameY == 4))
		{
			m_CurrTarget->Attacked(m_AP);
			_waitForArr = 0.0f;
		}
	}

	//�¿� ��������Ʈ
	if (_Image->CurrentFrameX == _Image->MaxFrameX - 1)
	{
		_Transform._Position.x - m_CurrTarget->_Transform._Position.x >= 0 ?
			m_Dir = C_DIR::LEFT : m_Dir = C_DIR::RIGHT;
	}

	//find���� ����
	if (((powf((_Transform._Position.x - m_CurrTarget->_Transform._Position.x), 2)
		+ powf((_Transform._Position.y - m_CurrTarget->_Transform._Position.y), 2)) > m_Range
		&& _Image->CurrentFrameX == _Image->MaxFrameX - 1) || m_CurrTarget->GetIsDead() == TRUE)
	{
		m_State = C_STATE::FIND;
		_Image->CurrentFrameX = 0;
	}
}

bool CAssassin::Passive()
{
	if (ProbCalculator(ASSA_PASSIVEPR) == TRUE)
	{
		//����� ��ϰ� SetDead�� ������ָ� ����� �Ѵ�.
		m_AP = (int)(m_OriginAP * ASSA_PASSIVEAP);
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

void CAssassin::SkillAttack()
{
	if ((_Image->CurrentFrameX == 3 || _Image->CurrentFrameX == 7) && (_Image->CurrentFrameY == 5 || _Image->CurrentFrameY == 6)
		&& m_IsSkillActive == TRUE && m_SkillTargets.empty() == FALSE)
	{
		for (unsigned int i = 0; i < m_SkillTargets.size(); i++)
		{
			if (_Image->CurrentFrameX == 3)
				_EFFECTMANAGER->PlayShot(EffectKind::EFFASSA_SKILL,
					Vector2F(m_SkillTargets[i]->_Transform._Position.x, m_SkillTargets[i]->_Transform._Position.y), 0, SizeF(0.5f, 0.5f), 0.02f);

			if (_Image->CurrentFrameX == 7)
				m_SkillTargets[i]->Attacked(m_AP * ASSA_SKILLATT);
#ifdef _DEBUG
			cout << "�ϻ��� SkillAttack enemy[" << m_SkillTargets[i]->GetEnemyNum() << "] ����" << endl;
#endif
		}
		m_SkillTargets.clear();

		m_IsSkillActive = FALSE;
		m_SkillCool = 0.f;

	}

	//�ִϸ��̼� ���������� �Ǹ� ������Ʈ�� �ٲʹٲ� ���� �ٹڲ�
	//��ư �ٽ� ���� FSM���� �����ش�.
	if ((_Image->CurrentFrameX == _Image->MaxFrameX - 1) && (_Image->CurrentFrameY == 5 || _Image->CurrentFrameY == 6))
	{
		_Image->CurrentFrameX = 0;
		m_State = C_STATE::IDLE;
	}
}

void CAssassin::FindMonster(vector<Enemy*> enemies)
{
	//m_SkillTarget�� ��ų���û����϶� �ܿ��� �׻� ����д�
	if (m_State == C_STATE::SKILLATTACK && m_SkillTargets.empty() == TRUE && m_IsSkillActive == TRUE)
	{
		//��Ƽ������ �ϰ������ 

		//map<float, Enemy*> _Distances;
		multimap<float, Enemy*> _Distances;

		for (unsigned int i = 0; i < enemies.size(); i++)
		{
			if (enemies[i]->GetIsDead() == TRUE)
				continue;

			float _targetDistance = powf(_Transform._Position.x - enemies[i]->_Transform._Position.x, 2)
				+ powf(_Transform._Position.y - enemies[i]->_Transform._Position.y, 2);

			_Distances.insert(pair<float, Enemy*>(_targetDistance, enemies[i]));
		}
		map<float, Enemy*>::iterator _iter;

		int _SkillTargetCnt = 0;
		for (_iter = _Distances.begin(); _iter != _Distances.end(); ++_iter)
		{
			m_SkillTargets.push_back(_iter->second);
			_SkillTargetCnt++;
#ifdef _DEBUG
			cout << "�ϻ��� SkillAttack enemy[" << _iter->second->GetEnemyNum() << "] ����" << endl;
#endif
			if (_SkillTargetCnt >= 5)
				break;

		}
	}

	else
	{
		//���� ���ε���� �Լ�
		if (m_State != C_STATE::FIND)
		{
			return;
		}

		bool _AllEnemyDead = TRUE;
		for (int i = 0; i < enemies.size(); i++)
		{
			if (enemies[i]->GetIsDead() == FALSE)
			{
				_AllEnemyDead = FALSE;
				break;
			}
		}

		if (enemies.size() == 0 || _AllEnemyDead == TRUE)
		{
			m_State = C_STATE::IDLE;
			//_Image->CurrentFrameX = 0;
			return;
		}

		//���� ����� ���͸� ã�� ����
		//������ �ſ� �� �Ÿ� ����
		float _targetDistance = 10000000.f;
		Enemy* _targetEnemy = nullptr;


		for (int i = 0; i < enemies.size(); i++)
		{
			//���� ���� �н�
			if (enemies[i]->GetIsDead() == TRUE)
				continue;

			//�Ÿ� �񱳿��̱� ������ ���� �������� �� �ʿ�� ����.
			float _Distance
				= powf((_Transform._Position.x - enemies[i]->_Transform._Position.x), 2)
				+ powf((_Transform._Position.y - enemies[i]->_Transform._Position.y), 2);

			//�Ÿ��� ���� �����
			if (_targetDistance >= _Distance)
			{
				_targetEnemy = enemies[i];
				_targetDistance = _Distance;
			}
		}

		m_CurrTarget = _targetEnemy;
		//������ Ÿ������ �̵��� ���ΰ�, �ٷ� ������ ���ΰ�
		if (_targetDistance <= m_Range)		//�Ÿ��� ��Ÿ����� ������
		{
			m_State = C_STATE::ATTACK;
			if (m_PrevState != C_STATE::MOVE)
				_Image->CurrentFrameX = 0;
		}
		else
		{
			m_State = C_STATE::MOVE;
			if (m_PrevState != C_STATE::MOVE)
				_Image->CurrentFrameX = 0;
		}
	}

}

void CAssassin::Attacked(int deal)
{
	if (m_IsShield) return;
	if (_Image->CurrentFrameX >= 8 && _Image->CurrentFrameX <= 17 && m_State == C_STATE::MOVE) return;

	m_HP -= deal;

	if (m_HP <= 0)
	{
		m_HP = 0;
	}

	m_IsAttacked = true;
}

void CAssassin::MoveTo(D2D1_VECTOR_2F targetPos)
{
	if (m_IsSet == false)
		return;

	D2D1_VECTOR_2F _DirVec;

	//Ÿ�ٰ� ���� �Ÿ� �� x, y
	float _dx = targetPos.x - _Transform._Position.x;
	float _dy = targetPos.y - _Transform._Position.y;

	//�� �Ÿ��� ���� ��ŭ
	_DirVec.x = _dx / sqrtf(_dx * _dx + _dy * _dy);
	_DirVec.y = _dy / sqrtf(_dx * _dx + _dy * _dy);

	//���⺤�Ͱ� 
	_DirVec.x <= 0 ?
		m_Dir = C_DIR::LEFT : m_Dir = C_DIR::RIGHT;
	_DirVec.x <= 0 ?
		_Image->CurrentFrameY = 1 : _Image->CurrentFrameY = 2;

	if (_Image->CurrentFrameX <= 8)
	{
		//���ڸ���
	}
	else
	{
		_Transform._Position.x += _DirVec.x * m_Velo;
		_Transform._Position.y += _DirVec.y * m_Velo;
	}

	//���ϰ��� Ʈ��� ����, �ƴϸ� ���  moveto�� ������ �Ѵ�.
	//���ǹ� ���߿� �����Ұ�. ��ư ������ ���ո�ɿ� �����ϱ� ���� ���

	if (fabs(_Transform._Position.x - targetPos.x) <= 200.f &&
		fabs(_Transform._Position.y - targetPos.y) <= 200.f)
	{
		m_FixFrame = 21;

		if (fabs(_Transform._Position.x - targetPos.x) <= 150.f &&
			fabs(_Transform._Position.y - targetPos.y) <= 150.f && _Image->CurrentFrameX == 21)
		{
			m_IsSet = false;
			m_State = C_STATE::IDLE;
			_Image->CurrentFrameX = 0;
		}
	}
}

HRESULT CAssassin::Init()
{
	m_Kind = PersonKind::Assassin;
	_IsActive = TRUE;
	_Image = g_D2DEngine.LoadFrameImage("Resource/FrameImageSample/Assassin_TotalAni.png", 22, 7);
	_EFFECTMANAGER->AddEffect(EffectKind::EFFASSA_SKILL, "Resource/FrameImageSample/AssassinSkill_TotalAni.png", 1.0f, 18, 1, true);
	_Image->CurrentFrameY = 0;
	_Transform._Position = Vector2F((float)(GetSystemMetrics(SM_CXSCREEN) / 2 - 256.f), (float)(GetSystemMetrics(SM_CYSCREEN) / 2));
	_Transform._Scale = SizeF(0.5f, 0.5f);
	GameObject::RectColliderSetting(SizeF(RECTSIZE, RECTSIZE), _CAMERAMANAGER->GetCamPos());

	_Image->AnimationSpeed = 0.15f;

	m_IsDead = FALSE;
	m_Passive = FALSE;
	m_Dir = C_DIR::LEFT;
	m_State = C_STATE::IDLE;
	m_PrevState = m_State;
	m_CurrTarget = nullptr;

	m_FixFrame = 9;
	m_HPMax = ASSA_HP;
	m_HP = m_HPMax;
	m_DP = 10;
	m_OriginAP = ASSA_AP;
	m_AP = m_OriginAP;
	m_Velo = ASSA_VELO;
	m_Range = (int)pow(256 * ASSA_ATTRANGE, 2);
	m_IsSkillActive = FALSE;
	m_SkillCool = 0.f;
	m_SkillTargets.clear();

	return S_OK;
}

void CAssassin::Release()
{

}

void CAssassin::Reset()
{
	_Image->CurrentFrameY = 0;
	_Image->CurrentFrameX = 0;
	_Transform._Position = Vector2F((float)(GetSystemMetrics(SM_CXSCREEN) / 2 - 256.f), (float)(GetSystemMetrics(SM_CYSCREEN) / 2));
	GameObject::RectColliderSetting(SizeF(RECTSIZE, RECTSIZE), _CAMERAMANAGER->GetCamPos());
	_Image->AnimationSpeed = 0.15f;
	m_AP = m_OriginAP;
	m_Velo = ASSA_VELO;
	m_Passive = FALSE;
	m_Dir = C_DIR::LEFT;
	m_State = C_STATE::IDLE;
	m_PrevState = m_State;
	m_CurrTarget = nullptr;
	m_IsSkillActive = FALSE;
	m_SkillCool = 0.f;
	m_SkillTargets.clear();
	m_FixFrame = 9;

}

void CAssassin::Update()
{
	m_PrevState = m_State;

	//m_SkillCool += _TIMEMANAGER->GetOneFrameElapsedTime();
	//if (m_SkillCool >= ASSA_SKILLCOOL)
	//{
	//	m_IsSkillActive = TRUE;
	//}

	//if (_KEYMANAGER->KeyDown(VK_SPACE))
	//{
	//}
	//�� �����Ӹ��� �׾����� Ȯ���Ѵ�
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
		//if (m_PrevState != C_STATE::MOVETO && !(m_PrevState == C_STATE::MOVE && _Image->CurrentFrameX <= 8))
		if (m_PrevState != C_STATE::MOVETO)
			_Image->CurrentFrameX = 0;

		m_State = C_STATE::MOVETO;
	}
	m_Passive = Passive();

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
		//y�� ���� ���° �̹�������, �� �� ������¥�� �ִϸ��̼�����
		if (m_PrevState == C_STATE::MOVE)
		{
			m_Dir == C_DIR::LEFT ?
				_Image->CurrentFrameY = 1 : _Image->CurrentFrameY = 2;
			_Image->AnimationSpeed = 0.15f;
			_Image->CurrentFrameX = 10;
		}
		else if (_Image->CurrentFrameX == _Image->MaxFrameX - 1 || _Image->CurrentFrameX == 0)
		{
			_Image->AnimationSpeed = 0.08f;
			m_Dir == C_DIR::LEFT ?
				_Image->CurrentFrameY = 5 : _Image->CurrentFrameY = 6;
			//	�������϶�					//�����϶�
			_Image->MaxFrameX = 18;
		}

	}
	break;
	case C_STATE::MOVE:
	{
		Move();

		m_FixFrame = 9;
		m_Dir == C_DIR::LEFT ?
			_Image->CurrentFrameY = 1 : _Image->CurrentFrameY = 2;
		_Image->MaxFrameX = 22;
		_Image->AnimationSpeed = 0.15f;
	}
	break;
	case C_STATE::ATTACK:
	{
		Attack();

		if (m_PrevState == C_STATE::MOVE)
		{
			m_Dir == C_DIR::LEFT ?
				_Image->CurrentFrameY = 1 : _Image->CurrentFrameY = 2;
			_Image->CurrentFrameX = 10;
			_Image->AnimationSpeed = 0.15f;
		}
		else if (_Image->CurrentFrameX == _Image->MaxFrameX - 1 || _Image->CurrentFrameX == 0)
		{
			_Image->AnimationSpeed = 0.08f;
			m_Dir == C_DIR::LEFT ?
				_Image->CurrentFrameY = 3 : _Image->CurrentFrameY = 4;
			_Image->MaxFrameX = 18;
		}

	}
	break;
	case C_STATE::IDLE:
	{
		Idle();
		if (m_PrevState == C_STATE::MOVE)
		{
			m_Dir == C_DIR::LEFT ?
				_Image->CurrentFrameY = 1 : _Image->CurrentFrameY = 2;
			_Image->CurrentFrameX = 10;
			_Image->AnimationSpeed = 0.15f;
		}
		else if (_Image->CurrentFrameX == _Image->MaxFrameX - 1 || _Image->CurrentFrameX == 0)
		{
			_Image->CurrentFrameY = 0;
			_Image->MaxFrameX = 2;
		}


	}
	break;
	case C_STATE::FIND:
	{
		if (m_PrevState == C_STATE::MOVE)
		{
			m_Dir == C_DIR::LEFT ?
				_Image->CurrentFrameY = 1 : _Image->CurrentFrameY = 2;
			_Image->CurrentFrameX = 10;
			_Image->AnimationSpeed = 0.15f;
		}
	}
	break;
	case C_STATE::MOVETO:
	{
		MoveTo(_GAMEMANAGER->_PlayerPosition);
		_Image->MaxFrameX = 22;
		_Image->AnimationSpeed = 0.15f;
	}
	break;
	}
	GameObject::RectColliderSetting(SizeF(RECTSIZE, RECTSIZE), _CAMERAMANAGER->GetCamPos());
}

void CAssassin::Render()
{
	if (m_IsDead == FALSE && _IsActive == TRUE)
	{
		if (m_State == C_STATE::MOVE || m_State == C_STATE::MOVETO)
		{
			g_D2DEngine.DrawFrameImageOnce(_CAMERAMANAGER->GetCamPos(), _TIMEMANAGER->GetOneFrameElapsedTime(), _Transform, _Image, FALSE, m_FixFrame);
		}
		else
		{
			g_D2DEngine.DrawFrameImage(_CAMERAMANAGER->GetCamPos(), _TIMEMANAGER->GetOneFrameElapsedTime(), _Transform, _Image);

		}
	}


#ifdef _DEBUG
	if (_GAMEMANAGER->GetShowDataInfo())
	{

		g_D2DEngine.DrawRect(_RectCollider, BrushColor::Yellow, false);
		g_D2DEngine.DrawTextOut("HP : %d", m_HP, _Image->ShowRect.right, _Image->ShowRect.top - 20.f, BrushColor::Yellow);
		g_D2DEngine.DrawTextOut("Assassin", _Image->ShowRect.right, _Image->ShowRect.top, BrushColor::Yellow);
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
		g_D2DEngine.DrawTextOut("Current FN : %d", _Image->CurrentFrameX, _Image->ShowRect.right, _Image->ShowRect.top + 80.0f, BrushColor::Yellow);
	}
#endif
}
