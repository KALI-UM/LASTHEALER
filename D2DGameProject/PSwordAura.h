#pragma once
#include "ProjectileObject.h"
class Enemy;

class PSwordAura : public ProjectileObject
{
public:
	PSwordAura();
	PSwordAura(int AP);
	~PSwordAura();

private:
	int				m_FixFrame;
	bool			m_IsGone;			//사라지기 직전
	int				m_AP;
	vector<Enemy*>	m_SkillTargets;

public:
	bool CheckCollision(vector<Enemy*> enemies);
	virtual void Shoot(D2D1_VECTOR_2F targetPos);

	HRESULT Init()override;
	void Release()override;
	void Reset()override;
	void Update()override;
	void Render() override;
};

