#pragma once
#include "ProjectileObject.h"
class Enemy;
class PArrow : public ProjectileObject
{
public:
	PArrow();
	PArrow(int AP);
	~PArrow();

private:
	int				m_FixFrame;
	bool			m_IsSkill;
	bool			m_IsGone;			//������� ����
	D2D1_VECTOR_2F	m_TargetVec;
	int				m_AP;
	vector<Enemy*>	m_SkillTargets;
public:
	bool CheckCollision(vector<Enemy*> enemies);
	virtual void Shoot(D2D1_VECTOR_2F targetPos);
	virtual void MoveTo();
	void Shoot_S();
	D2D1_VECTOR_2F GetDirVec() { return m_TargetNVec; };
	D2D1_VECTOR_2F GetVec() { return m_TargetVec; };
	void SetDegreeVec(float rotateDegree);
	void SetDirVec(D2D1_VECTOR_2F targetPos);	//Ÿ��, Ÿ�ٳ븻���� �Ѵ� ������Ʈ
	void SetAP(int AP) { m_AP = AP; };

	HRESULT Init()override;
	void Release()override;
	void Reset()override;
	void Update()override;
	void Render() override;
};

