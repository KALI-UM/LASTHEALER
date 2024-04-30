#include "pch.h"
#include "PlayerHealer.h"

namespace user
{
	PlayerHealer::PlayerHealer()
		: m_BuffArea(nullptr)
	{
		m_Direction = Direction::Left;
		m_Velocity = 10.5f;
		m_AniState = AniState::LeftIdle;
	}

	PlayerHealer::~PlayerHealer()
	{
	}

	HRESULT PlayerHealer::Init()
	{
		_Image = g_D2DEngine.LoadFrameImage("Resource/Player/Healer_Character_Ani.png", 22, 8);
		_Transform._Position = Vector2F((float)(GetSystemMetrics(SM_CXSCREEN) / 2), (float)(GetSystemMetrics(SM_CYSCREEN) / 2));
		_Transform._Scale = SizeF(0.5f, 0.5f);

		AnimationChange(AniState::LeftIdle, 1);

		m_BuffArea = new BuffArea();
		m_BuffArea->Init();

		for (int i = 0; i < 4; i++)
		{
			BuffEffect* _newEffect = new BuffEffect();
			_newEffect->Init();

			m_BuffEffect.push_back(_newEffect);
		}

		return S_OK;
	}

	void PlayerHealer::Release()
	{
		SAFE_CLASS(m_BuffArea);

		for (auto& buff : m_BuffEffect)
			SAFE_CLASS(buff);
	}

	void PlayerHealer::Reset()
	{
		m_BuffArea->Reset();
		_Transform._Position = Vector2F((float)(GetSystemMetrics(SM_CXSCREEN) / 2), (float)(GetSystemMetrics(SM_CYSCREEN) / 2));
		m_Direction = Direction::Left;
		m_Velocity = 10.5f;
		m_AniState = AniState::LeftIdle;
		AnimationChange(AniState::LeftIdle, 1);
	}

	void PlayerHealer::Update()
	{
		_GAMEMANAGER->_PlayerPosition = _Transform._Position;

		MovementControl();

		m_BuffArea->_Transform._Position = _Transform._Position;
		m_BuffArea->Update();

		for (const auto& buff : m_BuffEffect)
			buff->Update();

		GameObject::RectColliderSetting(SizeF(_Image->FrameWidth * 0.25f, _Image->FrameHeight * 0.25f), _CAMERAMANAGER->GetCamPos());

		_CAMERAMANAGER->Update(_Transform._Position, _TIMEMANAGER->GetOneFrameElapsedTime());
	}

	void PlayerHealer::Render()
	{
		g_D2DEngine.DrawFrameImage(_CAMERAMANAGER->GetCamPos(), _TIMEMANAGER->GetOneFrameElapsedTime(), _Transform, _Image);

		for (const auto& buff : m_BuffEffect)
			buff->Render();

#ifdef _DEBUG
		if (_GAMEMANAGER->GetShowDataInfo())
		{
			if (!_IsCollision)
				g_D2DEngine.DrawRect(_RectCollider, BrushColor::Black);
			else
				g_D2DEngine.DrawRect(_RectCollider, BrushColor::Yellow);

			g_D2DEngine.DrawTextOut("PlayerHealer", _Image->ShowRect.right, _Image->ShowRect.top, BrushColor::Yellow);
			g_D2DEngine.DrawTextOut("_Transform._Position.x: %.2f", _Transform._Position.x, _Image->ShowRect.right, _Image->ShowRect.top + 20.0f, BrushColor::White);
			g_D2DEngine.DrawTextOut("_Transform._Position.y: %.2f", _Transform._Position.y, _Image->ShowRect.right, _Image->ShowRect.top + 40.0f, BrushColor::White);
			g_D2DEngine.DrawTextOut("_Transform.GetRotationAngle(): %.2f", _Transform.GetRotationAngle(), _Image->ShowRect.right, _Image->ShowRect.top + 60.0f, BrushColor::White);
			g_D2DEngine.DrawTextOut("_IsCollision: %d", static_cast<int>(_IsCollision), _Image->ShowRect.right, _Image->ShowRect.top + 80.0f, BrushColor::White);
			g_D2DEngine.DrawTextOut("_Image->OriginalWidth: %.2f", _Image->OriginalWidth, _Image->ShowRect.right, _Image->ShowRect.top + 100.0f, BrushColor::White);
			g_D2DEngine.DrawTextOut("_Image->OriginalHeight: %.2f", _Image->OriginalHeight, _Image->ShowRect.right, _Image->ShowRect.top + 120.0f, BrushColor::White);
			g_D2DEngine.DrawTextOut("_Image->FrameWidth: %.2f", _Image->FrameWidth, _Image->ShowRect.right, _Image->ShowRect.top + 140.0f, BrushColor::White);
			g_D2DEngine.DrawTextOut("_Image->FrameHeight: %.2f", _Image->FrameHeight, _Image->ShowRect.right, _Image->ShowRect.top + 160.0f, BrushColor::White);
			g_D2DEngine.DrawTextOut("_Image->CurrentFrameX: %d", _Image->CurrentFrameX, _Image->ShowRect.right, _Image->ShowRect.top + 180.0f, BrushColor::White);
			g_D2DEngine.DrawTextOut("_Image->CurrentFrameY: %d", _Image->CurrentFrameY, _Image->ShowRect.right, _Image->ShowRect.top + 200.0f, BrushColor::White);
			g_D2DEngine.DrawTextOut("_Image->AnimationSpeed: %.2f", _Image->AnimationSpeed, _Image->ShowRect.right, _Image->ShowRect.top + 220.0f, BrushColor::White);
		}
#endif
	}

	void PlayerHealer::MovementControl()
	{
		if (m_AniState >= AniState::LeftBaseSkill && m_AniState < AniState::Size) return;

		if (_KEYMANAGER->KeyPress(VK_LEFT))
		{
			m_Direction = Direction::Left;
			AnimationChange(AniState::LeftRun, 7, -m_Velocity);
		}

		if (_KEYMANAGER->KeyPress(VK_RIGHT))
		{
			m_Direction = Direction::Right;
			AnimationChange(AniState::RightRun, 7, m_Velocity);
		}

		if (_KEYMANAGER->KeyPress(VK_UP))
		{
			if (m_Direction == Direction::Left) AnimationChange(AniState::LeftRun, 7, 0.f, -m_Velocity);
			if (m_Direction == Direction::Right) AnimationChange(AniState::RightRun, 7, 0.f, -m_Velocity);
		}

		if (_KEYMANAGER->KeyPress(VK_DOWN))
		{
			if (m_Direction == Direction::Left) AnimationChange(AniState::LeftRun, 7, 0.f, m_Velocity);
			if (m_Direction == Direction::Right) AnimationChange(AniState::RightRun, 7, 0.f, m_Velocity);
		}

		if (_KEYMANAGER->KeyUp(VK_LEFT))
		{
			_Image->CurrentFrameX = 0;
			m_Direction = Direction::Left;
			AnimationChange(AniState::LeftIdle, 1);
		}

		if (_KEYMANAGER->KeyUp(VK_RIGHT))
		{
			_Image->CurrentFrameX = 0;
			m_Direction = Direction::Right;
			AnimationChange(AniState::RightIdle, 1);
		}

		if (_KEYMANAGER->KeyUp(VK_UP))
		{
			_Image->CurrentFrameX = 0;
			if (m_Direction == Direction::Left) AnimationChange(AniState::LeftIdle, 1);
			if (m_Direction == Direction::Right) AnimationChange(AniState::RightIdle, 1);
		}

		if (_KEYMANAGER->KeyUp(VK_DOWN))
		{
			_Image->CurrentFrameX = 0;
			if (m_Direction == Direction::Left) AnimationChange(AniState::LeftIdle, 1);
			if (m_Direction == Direction::Right) AnimationChange(AniState::RightIdle, 1);
		}

		if (_CAMERAMANAGER->_MoveMin.x > _Transform._Position.x)
			_Transform._Position.x = _CAMERAMANAGER->_MoveMin.x;

		if (_CAMERAMANAGER->_MoveMax.x < _Transform._Position.x)
			_Transform._Position.x = _CAMERAMANAGER->_MoveMax.x;

		if (_CAMERAMANAGER->_MoveMin.y > _Transform._Position.y)
			_Transform._Position.y = _CAMERAMANAGER->_MoveMin.y;

		if (_CAMERAMANAGER->_MoveMax.y < _Transform._Position.y)
			_Transform._Position.y = _CAMERAMANAGER->_MoveMax.y;

	}

	void PlayerHealer::AnimationChange(AniState currentFrameY, int maxFrameX, float velocityX, float velocityY)
	{
		m_AniState = currentFrameY;

		_Image->CurrentFrameY = static_cast<int>(currentFrameY);
		_Image->MaxFrameX = maxFrameX;

		_Transform._Position.x += velocityX;
		_Transform._Position.y += velocityY;
	}

	void PlayerHealer::SkillBlockChoiceAnimation(bool& isChoiceBlock, int choiceBlockKind, int chainCount, vector<Person*>& nakamas)
	{
		if (!isChoiceBlock) return;

		m_Velocity = 0.0f;

		switch (choiceBlockKind)
		{
			// 일반 스킬
		case 0:	// 쉴드
		case 1:	// 공
		case 2: // 힐
			if (m_Direction == Direction::Left)  AnimationChange(AniState::LeftBaseSkill, 13);
			if (m_Direction == Direction::Right)  AnimationChange(AniState::RightBaseSkill, 13);
			break;

			// 특수 스킬
		case 3:
		case 4:
			if (m_Direction == Direction::Left)  AnimationChange(AniState::LeftUniqSkill, 22);
			if (m_Direction == Direction::Right)  AnimationChange(AniState::RightUniqSkill, 22);
			break;
		}

		// 스킬 발동 모션이 맨마지막이라면 
		if (_Image->CurrentFrameX == _Image->MaxFrameX - 1)
		{
			_Image->CurrentFrameX = 0;

			if (m_Direction == Direction::Left) AnimationChange(AniState::LeftIdle, 1);
			if (m_Direction == Direction::Right) AnimationChange(AniState::RightIdle, 1);

			m_Velocity = 10.5f;

			isChoiceBlock = false;

			m_BuffArea->_BuffAreaKind = static_cast<BuffKind>(choiceBlockKind);
			m_BuffArea->_Image->CurrentFrameY = choiceBlockKind;
			m_BuffArea->_TimeCount = 0.0f;
			m_BuffArea->_IsActive = true;
			m_BuffArea->_ChainCount = chainCount;

			// 장판 켜기
			switch (choiceBlockKind)
			{
			case 0:	// 쉴드
				if (chainCount == 1) m_BuffArea->_PlayTime = 1.0f;
				if (chainCount == 2) m_BuffArea->_PlayTime = 2.0f;
				if (chainCount == 3) m_BuffArea->_PlayTime = 5.0f;
				break;

			case 1:	// 공
			case 2: // 힐
				m_BuffArea->_PlayTime = 3.0f;
				break;

			case 3: // 집합 스킬
				for (UINT i = 1; i < nakamas.size(); i++)
				{
					nakamas[i]->m_IsSet = true;
				}

				m_BuffArea->_PlayTime = 0.0f;
				m_BuffArea->_IsActive = false;
				break;

			case 4: // 스킬공격명령
				for (UINT i = 1; i < nakamas.size(); i++)
				{
					nakamas[i]->m_IsSkillActive = true;
				}

				m_BuffArea->_PlayTime = 0.0f;
				m_BuffArea->_IsActive = false;
				break;
			}

#ifdef _DEBUG
			cout << "Using Skill_ " << choiceBlockKind;
			cout << " Chain Count_ " << chainCount << endl;
#endif
		}
	}

	void PlayerHealer::NakamaBuffUpdate(vector<Person*>& nakamas)
	{
		// 버프장판 비활성화되면 수행 X
		if (!m_BuffArea->_IsActive) return;

		for (UINT i = 1; i < nakamas.size(); i++)
		{			
			// 각 동료들의 이펙트 좌표 갱신시키기
			m_BuffEffect[i - 1]->_Transform._Position = nakamas[i]->_Transform._Position;

			// 각 버프가 이미 활성화되어있으면 검사 X
			if (m_BuffEffect[i - 1]->_IsActive) continue;

			// 버프초기화
			nakamas[i]->ReturnToOriginState();

			// 뒤진애는 버프 안줄거임
			if (nakamas[i]->GetIsDead()) continue;

			// 장판과 거리가 가까우면 버프 활성화
			if (g_D2DEngine.Distance(nakamas[i]->_Transform._Position, _Transform._Position) <= m_BuffArea->_Transform._Radius)
			{				
				// 동료들의 버프장판 활성화
				m_BuffEffect[i - 1]->Spawn(m_BuffArea->_ChainCount, m_BuffArea->_BuffAreaKind, m_BuffArea->_PlayTime);
				
				// 버프별 능력치 부여
				switch (m_BuffArea->_BuffAreaKind)
				{
				case user::BuffKind::Shield:
					nakamas[i]->SetShield(true);
#ifdef _DEBUG
					cout << "nakamas[" << i << "]->SetShield: " << nakamas[i]->GetShield() << endl;
#endif
					break;

				case user::BuffKind::AttUp:
					nakamas[i]->SetAttPowerUp(m_BuffEffect[i - 1]->_AttPower);
#ifdef _DEBUG
					cout << "nakamas[" << i << "]->_AttPower: " << nakamas[i]->GetAttPower() << endl;
#endif
					break;

				case user::BuffKind::HealUp:
					nakamas[i]->GetHeal(m_BuffEffect[i - 1]->_HealPower);
#ifdef _DEBUG
					cout << "nakamas[" << i << "]->GetHP: " << nakamas[i]->GetHP() << endl;
#endif
					break;
				}

				break;
			}
		}
	}
}