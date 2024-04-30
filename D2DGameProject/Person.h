#pragma once
class Enemy;
enum class PersonKind
{
	Player, Archer, Warrior, Assassin, Knight, Defualt
};

class Person : public GameObject
{
protected:
	int			m_HP;			// 체력
	bool		m_IsDead;		// 죽음여부
	int			m_HPMax;		// 최대체력
	PersonKind	m_Kind;
	bool		m_IsAttacked;	// 피격 당했는지 여부
	int			m_PlusAttPower;	// 공격력 버프받아 증가할 값
	int			m_OriginAP;		// 패시브땜시 생긴 원래 공격력 보존용 변수
	int			m_AP;			// 현재 공격력
	bool		m_IsShield;		// 쉴드 활성화 여부
	float		m_Velo;			//이동속도

public:
	bool		m_IsSet;
	bool		m_IsSkillActive;		//스킬 사용가능상태인가?
	bool		m_IsBossCollision;

public:
	Person() : GameObject::GameObject()
	{
		m_Kind = PersonKind::Defualt;
		m_HP = 100;
		m_IsDead = false;
		m_HPMax = 100;
		m_IsSet = false;
		m_IsAttacked = false;
		m_PlusAttPower = 0;
		m_OriginAP = 0;
		m_AP = 0;
		m_IsShield = false;
		m_IsBossCollision = false;
		m_IsSkillActive = false;
		m_Velo = 0.0f;
	}
	~Person() { }

	HRESULT Init() override { return S_OK; }
	void Release() override { }
	void Reset() override { }
	void Update() override { }
	void Render() override { }
	virtual void FindMonster(vector<Enemy*> enemies) {};

public:
	float GetHPPersent() 
	{ 	
		return m_HP / (m_HPMax * 0.01f);
	}

	void SetAttUPToChoiceUI(int power) { m_AP += power; }
	void SetHPUPToChoiceUI(int hp) { m_HPMax += hp; }
	void SetVLUPToChoiceUI(float velo) { m_Velo += velo; }

	// 현재 체력
	int GetHP() { return m_HP; }

	// 공격력 증가 수치
	void SetAttPowerUp(int attPower)
	{
		m_PlusAttPower = attPower;

		m_AP += m_PlusAttPower;
	}

	// 버프 원래대로 되돌리기
	void ReturnToOriginState()
	{
		m_IsShield = false;

		m_AP -= m_PlusAttPower;

		m_PlusAttPower = 0;
	}

	// 공격력 
	int GetAttPower() { return m_AP; }
	void SetAttPower(int attNum) { m_AP = attNum; }
	void SetAlive() { m_HP = m_HPMax; m_IsDead = FALSE; };
	void SetPos(D2D1_VECTOR_2F pos) { _Transform._Position = pos; };

	// 쉴드 
	bool GetShield() { return m_IsShield; }
	void SetShield(bool isShield) { m_IsShield = isShield; }
	//bool GetIsAttacked() { return m_IsAttacked; }

	void GetHeal(int healAmount)
	{
		//죽었거나 풀피였을 때는 False리턴
		if (m_IsDead == TRUE || m_HP == m_HPMax)
		{
			return;
		}

		if (m_HP + healAmount >= m_HPMax)
		{
			m_HP = m_HPMax;
		}
		else
		{
			m_HP += healAmount;
		}
	}

	virtual void Attacked(int deal)
	{
		if (m_IsShield) return;		

		m_HP -= deal;

		if (m_HP <= 0)
		{
			m_HP = 0;
		}

		m_IsAttacked = true;
	}

	bool GetIsDead() { return m_IsDead; }
	PersonKind GetPersonKind() { return m_Kind; };
};

