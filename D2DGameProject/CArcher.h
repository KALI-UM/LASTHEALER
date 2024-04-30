#pragma once


#define ARROWCNT				30
class Enemy;
class PArrow;

class CArcher : public Companion
{
public:
	CArcher();
	~CArcher();

protected:
	D2D1_VECTOR_2F	m_ArrPoint;			//move���� ��������� �Ѵ�.
	PArrow* m_Arrows[ARROWCNT];
public:

	//�� ���� �� Ÿ�� ���͸� ��ȯ�ϴ� �Լ�
	virtual void FindMonster(vector<Enemy*> enemies)override;
	void CheckArrows(vector<Enemy*> enemies);

	virtual void Move() override;
	virtual void Attack() override;
	virtual bool Passive() override;
	void PassiveOff();
	virtual void SkillAttack() override;

	HRESULT Init() override;
	void Release() override;
	void Reset() override;
	void Update() override;
	void Render() override;

};