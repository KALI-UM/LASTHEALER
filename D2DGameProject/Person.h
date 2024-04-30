#pragma once
class Enemy;
enum class PersonKind
{
	Player, Archer, Warrior, Assassin, Knight, Defualt
};

class Person : public GameObject
{
protected:
	int			m_HP;			// ü��
	bool		m_IsDead;		// ��������
	int			m_HPMax;		// �ִ�ü��
	PersonKind	m_Kind;
	bool		m_IsAttacked;	// �ǰ� ���ߴ��� ����
	int			m_PlusAttPower;	// ���ݷ� �����޾� ������ ��
	int			m_OriginAP;		// �нú궫�� ���� ���� ���ݷ� ������ ����
	int			m_AP;			// ���� ���ݷ�
	bool		m_IsShield;		// ���� Ȱ��ȭ ����
	float		m_Velo;			//�̵��ӵ�

public:
	bool		m_IsSet;
	bool		m_IsSkillActive;		//��ų ��밡�ɻ����ΰ�?
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

	// ���� ü��
	int GetHP() { return m_HP; }

	// ���ݷ� ���� ��ġ
	void SetAttPowerUp(int attPower)
	{
		m_PlusAttPower = attPower;

		m_AP += m_PlusAttPower;
	}

	// ���� ������� �ǵ�����
	void ReturnToOriginState()
	{
		m_IsShield = false;

		m_AP -= m_PlusAttPower;

		m_PlusAttPower = 0;
	}

	// ���ݷ� 
	int GetAttPower() { return m_AP; }
	void SetAttPower(int attNum) { m_AP = attNum; }
	void SetAlive() { m_HP = m_HPMax; m_IsDead = FALSE; };
	void SetPos(D2D1_VECTOR_2F pos) { _Transform._Position = pos; };

	// ���� 
	bool GetShield() { return m_IsShield; }
	void SetShield(bool isShield) { m_IsShield = isShield; }
	//bool GetIsAttacked() { return m_IsAttacked; }

	void GetHeal(int healAmount)
	{
		//�׾��ų� Ǯ�ǿ��� ���� False����
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

