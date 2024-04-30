#pragma once
#include "Person.h"
#define RECTSIZE		150.f
enum class C_DIR;
enum class C_STATE;
class Enemy;

class Companion : public Person
{
public:
	Companion(string name);
	Companion();
	~Companion();

protected:
	string		m_CharacterName;
	C_DIR		m_Dir;
	C_STATE		m_State;

	Enemy*		m_CurrTarget;	//현재 공격상대
	int			m_DP;			//방어력

	int			m_Range;		//사거리의 제곱
	float		m_SkillCool;
	bool		m_Passive;

public:
	bool ProbCalculator(float percent);
	virtual void MoveTo(D2D1_VECTOR_2F targetPos);

	virtual void FindMonster(vector<Enemy*> enemies);
	virtual void Idle();
	virtual void Attack();
	virtual void Move();
	virtual bool Passive();
	virtual void SkillAttack();

public:
	HRESULT Init()	override;
	void Release()	override;
	void Reset()	override;
	void Update()	override;
	void Render()	override;

};

enum class C_DIR
{
	LEFT,
	RIGHT
};
enum class C_STATE
{
	IDLE,
	MOVE,
	ATTACK,
	SKILLATTACK,
	FIND,
	DEAD,
	MOVETO
};
