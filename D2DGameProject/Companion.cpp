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
	:m_CharacterName("이름없음"), m_Dir(C_DIR::RIGHT), m_State(C_STATE::IDLE),
	m_CurrTarget(nullptr),
	m_DP(0), m_Range(0), m_SkillCool(0), m_Passive(FALSE)
{

}

Companion::~Companion()
{

}

bool Companion::ProbCalculator(float percent)
{
	//시드를 매번 다르게 준다.
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

	//타겟과 나의 거리 차 x, y
	float _dx = targetPos.x - _Transform._Position.x;
	float _dy = targetPos.y - _Transform._Position.y;

	//를 거리로 나눈 만큼
	_DirVec.x = _dx / sqrtf(_dx * _dx + _dy * _dy);
	_DirVec.y = _dy / sqrtf(_dx * _dx + _dy * _dy);

	_Transform._Position.x += _DirVec.x * m_Velo;
	_Transform._Position.y += _DirVec.y * m_Velo;

	//방향벡터가 
	_DirVec.x <= 0 ?
		m_Dir = C_DIR::LEFT : m_Dir = C_DIR::RIGHT;
	_DirVec.x <= 0 ?
	_Image->CurrentFrameY = 1 : _Image->CurrentFrameY = 2;


	//리턴값이 트루면 정지, 아니면 계속  moveto에 들어오게 한다.
	//조건문 나중에 수정할것. 여튼 힐러의 집합명령에 대응하기 위한 기능
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

	//가장 가까운 몬스터를 찾기 위함
	//임의의 매우 먼 거리 선정
	float _targetDistance = 10000000.f;
	Enemy* _targetEnemy = nullptr;


	for (int i = 0; i < enemies.size(); i++)
	{
		//죽은 적은 패스
		if (enemies[i]->GetIsDead() == TRUE)
			continue;

		//거리 비교용이기 때문에 굳이 제곱근을 할 필요는 없다.
		float _Distance
			= powf((_Transform._Position.x - enemies[i]->_Transform._Position.x), 2)
			+ powf((_Transform._Position.y - enemies[i]->_Transform._Position.y), 2);

		//거리가 가장 가까운
		if (_targetDistance >= _Distance)
		{
			_targetEnemy = enemies[i];
			_targetDistance = _Distance;
		}
	}

	m_CurrTarget = _targetEnemy;
	//선정한 타겟으로 이동할 것인가, 바로 공격할 것인가
	if (_targetDistance <= m_Range)		//거리가 사거리보다 가까우면
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

	//타겟과 나의 거리 차 x, y
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

	//를 거리로 나눈 만큼
	_DirVec.x = _dx / sqrtf(_dx * _dx + _dy * _dy);
	_DirVec.y = _dy / sqrtf(_dx * _dx + _dy * _dy);

	_Transform._Position.x += _DirVec.x * m_Velo;
	_Transform._Position.y += _DirVec.y * m_Velo;

	//방향벡터가 
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

	//각 자식에서 프레임 이미지 로드할 것
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




//무야~호~~!!!