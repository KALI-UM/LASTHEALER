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
	int			m_HP;			//ü��
	int			m_HPMax;		//�ִ�ü��
	int			m_DP;			//����
	int			m_AP;			//���ݷ�
	float		m_AVelo;		//���ݼӵ�
	float		m_Velo;			//�̵��ӵ�
	float		m_AttackCool;	//������
	float		m_CountDieTime;	//������ ���� �ð�

	bool		m_RangeCheck;	//���ݹ���
	bool		m_IsDead;		//����

	int			m_EnemyNum;		//�˱� �ĸ¾Ҵ��� �ȸ¾Ҵ��� Ȯ���ϴ� ����
	E_DIR		m_Dir;			//������ ������ �������� ������
	E_STATE		m_State;		//����

	Person*		m_CurrTarget;	//���� ���ݻ��

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
	virtual void AttackMelee();		//�ٰŸ� ����
	virtual void AttackRange();		//���Ÿ� ����

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

