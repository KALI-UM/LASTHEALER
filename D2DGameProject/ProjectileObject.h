#pragma once

enum class P_DIR;

//투사체 오브젝트들의 부모
//타겟포스를 통해 현재 방향벡터 방향으로 꺾어준다.
//좌우 이펙트가 따로 있다는 전제가 있어야 한다.

class ProjectileObject : public GameObject
{
public:
	ProjectileObject();
	~ProjectileObject();

protected:
	bool			m_IsLoaded;					//장전이 되어있나, 즉 발사 가능하냐? = true 이외는 모두 false
	D2D1_VECTOR_2F	m_TargetNVec;				//발사하는 사이에 몬스터가 이동하면 명중실패해야 하기 때문ㅔ
	float			m_Velo;
	P_DIR			m_Dir;
	float			m_RotateAngle;

public:

	HRESULT Init() override;
	void Release() override;
	void Reset() override;
	void Update() override;					//업데이트는 각각 자식에서 다시 해주세요
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