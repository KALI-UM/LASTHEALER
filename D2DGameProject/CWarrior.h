#pragma once
class PSwordAura;
#define SAURACNT	10
class CWarrior : public Companion
{
public:
	CWarrior();
	~CWarrior();

protected:
	PSwordAura* m_SwodAura[10];

public:
	void CheckSwordAura(vector<Enemy*> enemies);
	virtual void Move() override;
	virtual void FindMonster(vector<Enemy*> enemies);
	virtual void Attack() override;
	virtual bool Passive() override;
	virtual void SkillAttack() override;

	HRESULT Init() override;
	void Release() override;
	void Reset() override;
	void Update() override;
	void Render() override;
};

