#pragma once
#include "Person.h"


#define POS _Transform._Position
#define X	_Transform._Position.x
#define Y	_Transform._Position.y

enum class E_DIR;
//enum class E_STATE;

enum class E_STATE
{
	IDLE,
	MOVE,
	ATTACK,
	MOVEATTACK,
	SPLIT_DEAD,
	STUN,
	FIND,
	FINDHEALER,
	DASH,
	STUMP,
	SUMMON,
	DEAD
};

class Enemy : public GameObject
{
protected:
	int			m_HP;			//체력
	int			m_HPMax;		//최대체력
	int			m_DP;			//방어력
	int			m_AP;			//공격력
	float		m_AVelo;		//공격속도
	float		m_Velo;			//이동속도
	float		m_AttackCool;	//공격쿨
	float		m_CountDieTime;	//죽은뒤 지난 시간

	bool		m_RangeCheck;	//공격범위
	bool		m_IsDead;		//생사

	int			m_EnemyNum;		//검기 쳐맞았는지 안맞았는지 확인하는 변수
	E_DIR		m_Dir;			//왼쪽을 보는지 오른쪽을 보는지
	E_STATE		m_State;		//상태

	Person*		m_CurrTarget;	//현재 공격상대

public:
	Enemy();
	~Enemy();

	HRESULT Init() override;
	void Release() override;
	void Reset() override;
	void Update() override;
	virtual void Update(vector<Person*> persons) = 0;
	void Render() override;

	int GetHp();

	virtual void Attacked(int deal);
	void SetDead() { m_HP = 0; m_IsDead = true; }
	void SetPos(D2D1_VECTOR_2F pos)
	{ 
		_Transform._Position.x = pos.x - 750.0f; 
		_Transform._Position.y = pos.y - 350.0f;
	}

	int GetEnemyNum() { return m_EnemyNum; }
	void SetEnemyNum(int i) { m_EnemyNum = i; }
	bool GetIsDead() { return m_IsDead; };
	virtual void SetAlive() 
	{
		m_HP = m_HPMax;	
		m_IsDead = false;
		_IsActive = true;
		m_State = E_STATE::IDLE;
	}

	virtual void Find(vector<Person*> Persons);
	virtual void Move();
	virtual void AttackMelee();		//근거리 공격
	virtual void AttackRange();		//원거리 공격

	void AnimationChange(int showFrameY, int showMaxFrameX);

	float GetHPPersent()
	{
		return m_HP / (m_HPMax * 0.01f);
	}
};

enum class E_DIR
{
	LEFT,
	RIGHT
};

