#pragma once

enum class P_DIR;

//����ü ������Ʈ���� �θ�
//Ÿ�������� ���� ���� ���⺤�� �������� �����ش�.
//�¿� ����Ʈ�� ���� �ִٴ� ������ �־�� �Ѵ�.

class ProjectileObject : public GameObject
{
public:
	ProjectileObject();
	~ProjectileObject();

protected:
	bool			m_IsLoaded;					//������ �Ǿ��ֳ�, �� �߻� �����ϳ�? = true �ܴ̿� ��� false
	D2D1_VECTOR_2F	m_TargetNVec;				//�߻��ϴ� ���̿� ���Ͱ� �̵��ϸ� ���߽����ؾ� �ϱ� ������
	float			m_Velo;
	P_DIR			m_Dir;
	float			m_RotateAngle;

public:

	HRESULT Init() override;
	void Release() override;
	void Reset() override;
	void Update() override;					//������Ʈ�� ���� �ڽĿ��� �ٽ� ���ּ���
	void Render() override;

	void SetPos(D2D1_VECTOR_2F pos);
	virtual void MoveTo();
	virtual void Shoot(D2D1_VECTOR_2F targetPos);
	bool GetIsLoaded() { return m_IsLoaded; }
	float GetRAngle() { return m_RotateAngle; };
};


enum class P_DIR
{
	LEFT,
	RIGHT
};