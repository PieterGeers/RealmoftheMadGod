#pragma once
#include "Texture.h"

class Health
{
public:
	Health(int health);
	void Draw(const Rectf& avatarRect) const;
	void Draw(Point2f pos, float width, float height) const;
	void Regenerate(float elapsedSec);
	void TakeDamage(int damage);
	void Set(int HP);
	void SetMaxHP();
	void SetMaxHp(int level);
	int Get();
	int GetMax();
	void Initialize(int health);

private:
	int m_Health, m_MaxHealth;
	const int m_AmountRegenPS{ 5 };
	float m_AccTime;

};

