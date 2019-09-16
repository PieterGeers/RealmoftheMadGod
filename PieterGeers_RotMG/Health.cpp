#include "stdafx.h"
#include "Health.h"


Health::Health(int health)
	: m_Health{health}
	, m_MaxHealth{health}
	, m_AccTime{0}
{

}

void Health::Regenerate(float elapsedSec)
{
	m_AccTime += elapsedSec;
	if (m_AccTime > 1.f)
	{
		if (m_Health < m_MaxHealth)
		{
			m_Health += m_AmountRegenPS;
			if (m_Health > m_MaxHealth)
			{
				m_Health = m_MaxHealth;
			}
		}
		m_AccTime -= 1.f;
	}
}

void Health::TakeDamage(int damage)
{
	m_Health -= damage;
	if (m_Health < 0)
	{
		m_Health = 0;
	}
}

void Health::SetMaxHP()
{
	m_MaxHealth += 25;
	m_Health = m_MaxHealth;
}

void Health::Draw(const Rectf& avatarRect) const
{
	float HealthPercentage{ m_Health / float(m_MaxHealth) };
	utils::SetColor({ 0.f, 0.f, 0.f, 1.f });
	utils::DrawRect({ avatarRect.left, avatarRect.bottom - 6, avatarRect.width, 5 },2.f);
	if (HealthPercentage > .5f)
	{
		utils::SetColor({ 0.f , 1.f, 0.f, 1.f });
	}
	else if (HealthPercentage < .25f)
	{
		utils::SetColor({ 1.0f, 0.0f, 0.0f, 1.0f });
	}
	else
	{
		utils::SetColor({ 1.0f, 0.5f, 0.0f, 1.0f });
	}
	utils::FillRect({ avatarRect.left, avatarRect.bottom - 6, avatarRect.width * HealthPercentage, 5 });
}

void Health::Draw(Point2f pos, float width, float height) const
{
	float HealthPercentage{ m_Health / float(m_MaxHealth) };
	utils::SetColor({ 1.f , 0.2f, 0.2f, 1.f });
	utils::FillRect({ pos.x, pos.y, width * HealthPercentage, height });
}

int Health::Get()
{
	return m_Health;
}

int Health::GetMax()
{
	return m_MaxHealth;
}

void Health::Set(int HP)
{
	m_Health = HP;
}

void Health::SetMaxHp(int level)
{
	m_MaxHealth += (25 * level);
	m_Health = m_MaxHealth;
}

void Health::Initialize(int health)
{
	m_MaxHealth = health;
	m_Health = health;
}