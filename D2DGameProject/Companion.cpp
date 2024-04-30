#include "pch.h"

#include "Enemy.h"
#include "Companion.h"

Companion::Companion(string name)
	:m_CharacterName(name), m_Dir(C_DIR::RIGHT), m_State(C_STATE::IDLE),
	m_CurrTarget(nullptr),
	m_DP(0), m_Range(0), m_SkillCool(0), m_Passive(FALSE)
{

}

Companion::Companion()
	:m_CharacterName("�̸�����"), m_Dir(C_DIR::RIGHT), m_State(C_STATE::IDLE),
	m_CurrTarget(nullptr),
	m_DP(0), m_Range(0), m_SkillCool(0), m_Passive(FALSE)
{

}

Companion::~Companion()
{

}

bool Companion::ProbCalculator(float percent)
{
	//�õ带 �Ź� �ٸ��� �ش�.
	srand(GetTickCount64());
	float CompareNum = rand() / (float)RAND_MAX * 100.0f;

	if (percent >= CompareNum)
		return TRUE;

	return FALSE;
}

void Companion::MoveTo(D2D1_VECTOR_2F targetPos)
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

	_Transform._Position.x += _DirVec.x * m_Velo;
	_Transform._Position.y += _DirVec.y * m_Velo;

	//���⺤�Ͱ� 
	_DirVec.x <= 0 ?
		m_Dir = C_DIR::LEFT : m_Dir = C_DIR::RIGHT;
	_DirVec.x <= 0 ?
	_Image->CurrentFrameY = 1 : _Image->CurrentFrameY = 2;


	//���ϰ��� Ʈ��� ����, �ƴϸ� ���  moveto�� ������ �Ѵ�.
	//���ǹ� ���߿� �����Ұ�. ��ư ������ ���ո�ɿ� �����ϱ� ���� ���
	if (fabs(_Transform._Position.x - targetPos.x) <= 150.f &&
		fabs(_Transform._Position.y - targetPos.y) <= 150.f && _Image->CurrentFrameX==_Image->MaxFrameX-1)
	{
		m_IsSet = false;

		m_State = C_STATE::IDLE;
		_Image->CurrentFrameX = 0;
	}
}


void Companion::FindMonster(vector<Enemy*> enemies)
{
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
		_Image->CurrentFrameX = 0;
		_Image->AnimationSpeed = 0.1f;

	}
	else
	{
		m_State = C_STATE::MOVE;
		_Image->CurrentFrameX = 0;
		_Image->AnimationSpeed = 0.1f;
	}
}

void Companion::Idle()
{
	m_State = C_STATE::FIND;
	_Image->AnimationSpeed = 0.15f;
}

void Companion::Move()
{
	D2D1_VECTOR_2F _DirVec;

	//Ÿ�ٰ� ���� �Ÿ� �� x, y
	if (m_CurrTarget == nullptr)
	{
		m_State = C_STATE::FIND;
		return;
	}
	if (m_CurrTarget->GetIsDead() == TRUE)
	{
		m_State = C_STATE::FIND;
		//_Image->CurrentFrameX = 0;
		return;
	}

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


	if ((_dx * _dx + _dy * _dy) <= m_Range && _Image->CurrentFrameX == _Image->MaxFrameX - 1)
	{
		m_State = C_STATE::ATTACK;
		_Image->CurrentFrameX = 0;
	}
}

void Companion::Attack()
{

}


bool Companion::Passive()
{

	return FALSE;
}

void Companion::SkillAttack()
{

}

HRESULT Companion::Init()
{

	//�� �ڽĿ��� ������ �̹��� �ε��� ��
	return S_OK;
}

void Companion::Release()
{

}

void Companion::Reset()
{

}

void Companion::Update()
{

}


void Companion::Render()
{

}




//����~ȣ~~!!!