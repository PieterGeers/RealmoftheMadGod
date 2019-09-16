#include "stdafx.h"
#include "Mana.h"
#include "SoundEffect.h"

Mana::Mana(int mana)
	: m_Mana{ mana}
	, m_MaxMana{mana}
	, m_AccTime{ 0 }
	, m_pNoMana{std::make_shared<SoundEffect>("Resources/Sounds/NoMana.ogg")}
{
}

bool Mana::UseMana()
{
	if (m_Mana >= 50)
	{
		m_Mana -= 50;
		return true;
	}
	m_pNoMana->Play(0);
	return false;
}
void Mana::Regenerate(float elapsedSec)
{
	m_AccTime += elapsedSec;
	if (m_AccTime >= 1.f)
	{
		if (m_Mana < m_MaxMana)
		{
			m_Mana += 5;
			if (m_Mana > m_MaxMana)
			{
				m_Mana = m_MaxMana;
			}
		}
		m_AccTime -= 1.f;
	}
}
void Mana::Draw(Point2f pos, float width, float height) const
{
	float ManaPercentage{ m_Mana/ float(m_MaxMana) };

	utils::SetColor({ 0.4f, 0.5f, 1.f, 1.f });
	utils::FillRect({ pos }, width * ManaPercentage, height);
}

void Mana::SetMaxMana()
{
	m_MaxMana += 10;
	m_Mana = m_MaxMana;
}

void Mana::Set(int Mana)
{
	m_Mana = Mana;
}

int Mana::Get()
{
	return m_Mana;
}

int Mana::GetMax()
{
	return m_MaxMana;
}

void Mana::SetMaxMana(int level)
{
	m_MaxMana += 10 * level;
	m_Mana = m_MaxMana;
}

void Mana::SetVolume(int volume)
{
	m_pNoMana->SetVolume(volume);
}