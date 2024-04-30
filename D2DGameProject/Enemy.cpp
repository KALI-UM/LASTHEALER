#include "pch.h"
#include "Enemy.h"

Enemy::Enemy()
	: GameObject::GameObject(),
	m_HP(0), m_HPMax(0), m_DP(0), m_AP(0), m_AVelo(0), m_Velo(0), m_AttackCool(0), m_CountDieTime(0.0f),
	m_RangeCheck(false), m_IsDead(false),
	m_EnemyNum(0), m_Dir(E_DIR::RIGHT), m_State(E_STATE::IDLE),
	m_CurrTarget(nullptr)
{

}

Enemy::~Enemy()
{

}

HRESULT Enemy::Init()
{
	return S_OK;
}

void Enemy::Release()
{
	m_CurrTarget = nullptr;
}

void Enemy::Reset()
{
}

void Enemy::Update()
{
}

void Enemy::Render()
{
}

int Enemy::GetHp()
{
	return 0;
}

void Enemy::Attacked(int deal)
{
	m_HP -= deal;

	if (m_HP <= 0)
	{
		m_HP = 0;
	}
}

void Enemy::Find(vector<Person*> Persons)
{
}

void Enemy::Move()
{
}

void Enemy::AttackMelee()
{
}

void Enemy::AttackRange()
{
}

void Enemy::AnimationChange(int showFrameY, int showMaxFrameX)
{
	_Image->CurrentFrameY = showFrameY;
	_Image->MaxFrameX = showMaxFrameX;
}
