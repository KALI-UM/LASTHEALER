#pragma once


class KNIG_AreaEffect : public GameObject
{
public:
	KNIG_AreaEffect() : GameObject::GameObject(), _PlayTime(0.f), _TimeCount(0.f)
	{}
	~KNIG_AreaEffect() {}

protected:
	float _PlayTime;
	float _TimeCount;
public:
	void SetPos(D2D1_VECTOR_2F pos) { _Transform._Position = pos; };
	HRESULT Init() override
	{
		_Image = g_D2DEngine.LoadFrameImage("Resource/FrameImageSample/Knight_EffectAni_Layer.png", 5, 1);
		_Transform._Position = Vector2F(0.f, 0.f);
		//_Transform._Scale = SizeF(0.5f, 0.5f);
		_IsActive = false;
		_PlayTime = 2.0f;

		return S_OK;
	}

	void Release() override { }
	void Reset() override
	{
		_Image->CurrentFrameX = 0;
		_TimeCount = 0.0f;
		_Transform._Position = Vector2F(0.f, 0.f);
		_IsActive = false;
	}

	void Update() override
	{
		if (!_IsActive) return;

		_TimeCount += _TIMEMANAGER->GetOneFrameElapsedTime();

		if (_TimeCount >= _PlayTime)
		{
			_TimeCount = 0.0f;
			_IsActive = false;
		}

		GameObject::CircleInfomationSetting(_CAMERAMANAGER->GetCamPos());
	}

	void Render() override
	{
		if (!_IsActive) return;

		g_D2DEngine.DrawFrameImage(_CAMERAMANAGER->GetCamPos(), _TIMEMANAGER->GetOneFrameElapsedTime(), _Transform, _Image);

	}
};

class CKnight : public Companion
{
public:
	CKnight();
	~CKnight();
protected:
	vector<Enemy*>			m_SkillTargets;
	KNIG_AreaEffect* m_AreaEffect;
public:
	virtual void Move();
	virtual void Attacked(int deal);
	virtual void FindMonster(vector<Enemy*> enemies) override;
	virtual void Attack();
	virtual bool Passive();
	virtual void SkillAttack();

public:
	HRESULT Init()	override;
	void Release()	override;
	void Reset()	override;
	void Update()	override;
	void Render()	override;
};