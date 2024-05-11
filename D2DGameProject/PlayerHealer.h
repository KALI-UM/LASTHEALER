#pragma once
#include "Person.h"

class QuadTree;
namespace user
{
	enum class BuffKind { Shield, AttUp, HealUp };

	class BuffEffect : public GameObject
	{
	public:
		BuffKind _BuffKind;
		int _HealPower;
		int _AttPower;
		float _PlayTime;
		float _TimeCount;

	public:
		BuffEffect() : GameObject::GameObject(),
			_BuffKind(BuffKind::Shield), _HealPower(0), _AttPower(0), _PlayTime(0.0f), _TimeCount(0.0f)
		{ }
		~BuffEffect() { }

		HRESULT Init() override
		{
			_Image = g_D2DEngine.LoadFrameImage("Resource/Player/Healer_buff.png", 5, 3);
			_IsActive = false;

			return S_OK;
		}

		void Release() override { }

		void Reset() override
		{
			_HealPower = 0;
			_AttPower = 0;
			_PlayTime = 0.0f;
			_TimeCount = 0.0f;
			_IsActive = false;
		}

		void Update() override
		{
			if (!_IsActive) return;

			_TimeCount += _TIMEMANAGER->GetOneFrameElapsedTime();

			if (_TimeCount >= _PlayTime)
			{
				Reset();
			}
		}

		void Render() override
		{
			if (!_IsActive) return;

			g_D2DEngine.DrawFrameImage(_CAMERAMANAGER->GetCamPos(), _TIMEMANAGER->GetOneFrameElapsedTime(), _Transform, _Image);
		}

	public:
		void Spawn(int chainCount, BuffKind currentBuffAreaKind, float playTime)
		{
			switch (currentBuffAreaKind)
			{
			case user::BuffKind::Shield:
				break;

			case user::BuffKind::AttUp:
				_AttPower = chainCount * BUFF_ATT_NUM_FACTOR;
				break;

			case user::BuffKind::HealUp:
				_HealPower = chainCount * BUFF_HEAL_NUM_FACTOR;
				break;
			}

			_PlayTime = playTime;

			_Image->CurrentFrameY = static_cast<int>(currentBuffAreaKind);

			_IsActive = true;
		}
	};

	class BuffArea : public GameObject
	{
	public:
		BuffKind _BuffAreaKind;
		float _PlayTime;
		float _TimeCount;
		int _ChainCount;

	public:
		BuffArea() : GameObject::GameObject(), _BuffAreaKind(BuffKind::HealUp),
			_PlayTime(0.f), _TimeCount(0.0f), _ChainCount(0) { }

		~BuffArea() { }

		HRESULT Init() override
		{
			_Image = g_D2DEngine.LoadFrameImage("Resource/Player/Healer_buff_layer.png", 6, 3);
			_Transform._Scale = SizeF(0.5f, 0.5f);
			_Transform._Radius = _Image->FrameWidth * 0.215f;
			_IsActive = false;

			return S_OK;
		}

		void Release() override { }
		void Reset() override
		{
			_Image->CurrentFrameX = 0;
			_TimeCount = 0.0f;
			_IsActive = false;
			_ChainCount = 0;
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

#ifdef _DEBUG
			if (_GAMEMANAGER->GetShowDataInfo())
			{
				if (!_IsCollision)
					g_D2DEngine.DrawCircle(_CircleCollider, BrushColor::Black);
				else
					g_D2DEngine.DrawCircle(_CircleCollider, BrushColor::Yellow);
			}
#endif
		}
	};

	enum class AniState
	{
		LeftIdle, RightIdle,
		LeftRun, RightRun,
		LeftBaseSkill, RightBaseSkill,
		LeftUniqSkill, RightUniqSkill,
		Size
	};

	enum class Direction
	{
		Left, Right
	};

	class PlayerHealer : public Person
	{
	private:
		BuffArea* m_BuffArea;
		Direction m_Direction;
		AniState m_AniState;
		float m_Velocity;

		bool m_Grave;

		vector<BuffEffect*> m_BuffEffect;

	public:
		PlayerHealer();
		~PlayerHealer();

		HRESULT Init() override;
		void Release() override;
		void Reset() override;
		void Update() override;
		void Render() override;

	private:
		void MovementControl();
		void AnimationChange(AniState currentFrameY, int maxFrameX, float velocityX = 0.0f, float velocityY = 0.0f);

	public:
		void SkillBlockChoiceAnimation(bool& isChoiceBlock, int choiceBlockKind, int chainCount, vector<Person*>& nakamas);
		void NakamaBuffUpdate(vector<Person*>& nakamas);
		BuffArea* GetJangPan() { return m_BuffArea; }

		QuadTree* m_QuadTree;
	};
}