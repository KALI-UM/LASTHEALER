#pragma once
#include "Companion.h"
class CAssassin : public Companion
{
public:
	CAssassin();
	~CAssassin();

protected:

	int				m_FixFrame;
	C_STATE			m_PrevState;
	vector <Enemy*> m_SkillTargets;
public:
	virtual void Idle()override;
	virtual void Move() override;
	virtual void Attack() override;
	virtual bool Passive() override;
	virtual void SkillAttack() override;
	virtual void FindMonster(vector<Enemy*> enemies) override;
	virtual void Attacked(int deal) override;
	virtual void MoveTo(D2D1_VECTOR_2F targetPos) override;


	HRESULT Init()override;
	void Release()override;
	void Reset()override;
	void Update()override;
	void Render() override;
};

