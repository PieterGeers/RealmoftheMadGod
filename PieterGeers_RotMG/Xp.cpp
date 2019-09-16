#include "stdafx.h"
#include "Xp.h"
#include "SoundEffect.h"

Xp::Xp()
	: m_MaxXp{100}
	, m_Xp{0}
	, m_Level{1}
	, m_LevelUp{ false }
	, m_pLevelUpSound{std::make_shared<SoundEffect>("Resources/Sounds/LevelUp.ogg")}
{
}

bool Xp::AddXp(int value)
{
	if (m_Xp + value >= m_MaxXp)
	{
		m_MaxXp += 50*(m_Level);
		++m_Level;
		m_Xp = 0;
		m_pLevelUpSound->Play(0);
		return true;
	}
	m_Xp += value;
	return false;
}

void Xp::Draw(Point2f pos, float width, float height) const
{
	float XpPercentage{ m_Xp / float(m_MaxXp) };
	utils::SetColor({ 0.35f, 0.5f, 0.15f, 1.f });
	utils::FillRect({ pos }, width * XpPercentage, height);
}

int Xp::GetLevel() const
{
	return m_Level;
}

int Xp::GetXp() const
{
	return m_Xp;
}

void Xp::SetXp(int currentXp)
{
	m_Xp = currentXp;
}

void Xp::SetLevel(int level)
{
	m_Level = level;
	m_MaxXp = 100 + level * 50;
}

void Xp::SetVolume(int volume)
{
	m_pLevelUpSound->SetVolume(volume);
}