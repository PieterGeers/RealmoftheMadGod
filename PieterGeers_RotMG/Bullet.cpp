#include "stdafx.h"
#include "Bullet.h"
#include "Texture.h"

Bullet::Bullet(const Point2f& launchPos, const CharacterBullet& bullet)
	: m_LaunchPos{ launchPos }
	, m_BulletPos{ 0, -8 }
	, m_Translate{ 0,0 }
	, m_Velocity{ 0,0 }
	, m_IsActivated{ false }
	, m_Angle{}
	, m_Border{ {launchPos}, 0 }
	, m_HitTarget{ false }
	, m_Bullet{bullet}
{
	switch (m_Bullet)
	{
	case Bullet::CharacterBullet::wizard:
		m_Border.radius = 400.f;
		m_pBullet = std::make_shared<Texture>("Resources/Bullets/bulletWiz.png");
		break;
	case Bullet::CharacterBullet::knight:
		m_Border.radius = 150.f;
		m_pBullet = std::make_shared<Texture>("Resources/Bullets/bulletKnight.png");
		break;
	case Bullet::CharacterBullet::archer:
		m_Border.radius = 300.f;
		m_pBullet = std::make_shared<Texture>("Resources/Bullets/bulletArcher.png");
		break;
	case Bullet::CharacterBullet::enemyArcher:
		m_Border.radius = 350.f;
		m_pBullet = std::make_shared<Texture>("Resources/Bullets/bulletEnemyArcher.png");
		break;
	case Bullet::CharacterBullet::enemyWizard:
		m_Border.radius = 400.f;
		m_pBullet = std::make_shared<Texture>("Resources/Bullets/bulletEnemyWizard.png");
		break;
	case Bullet::CharacterBullet::lion:
		m_Border.radius = 300.f;
		m_pBullet = std::make_shared<Texture>("Resources/Bullets/bulletLion.png");
		break;
	case Bullet::CharacterBullet::babydragon:
		m_Border.radius = 200.f;
		m_pBullet = std::make_shared<Texture>("Resources/Bullets/bulletBabyDragon.png");
		break;
	case Bullet::CharacterBullet::godSnake:
		m_Border.radius = 350.f;
		m_pBullet = std::make_shared<Texture>("Resources/Bullets/bulletLeviathan.png");
		break;
	case Bullet::CharacterBullet::godTree:
		m_Border.radius = 350.f;
		m_pBullet = std::make_shared<Texture>("Resources/Bullets/bulletEntGod.png");
		break;
	}
}

void Bullet::Draw() const
{
	if (m_IsActivated == true)
	{
		glPushMatrix();
		glTranslatef(m_Translate.x + m_LaunchPos.x, m_Translate.y + m_LaunchPos.y, 0.f);
		glRotatef(float((m_Angle * 180) / M_PI), 0, 0, 1);
		m_pBullet->Draw({ m_BulletPos.x, m_BulletPos.y });
		glPopMatrix();
	}
}

void Bullet::Update(float elapsedSec)
{
	if (m_IsActivated == true)
	{
		m_Translate.x += elapsedSec * m_Velocity.x;
		m_Translate.y += elapsedSec * m_Velocity.y;
	}
}

void Bullet::Shoot(float angle)
{
	m_IsActivated = true;
	m_Angle = angle;
	m_Velocity.x = m_Speed*cos(m_Angle);
	m_Velocity.y = m_Speed*sin(m_Angle);
}

bool Bullet::CheckTravelledDistance()
{
	if (m_IsActivated == true)
	{
		if (utils::IsPointInCircle({ m_Translate.x + m_LaunchPos.x, m_Translate.y + m_LaunchPos.y }, { m_Border }) == false)
		{
			m_IsActivated = false;
			return true;
		}
	}
	return false;
}

bool Bullet::CheckCollision(Rectf HitShape)
{
	if (m_HitTarget == false)
	{
		if (utils::IsPointInRect({ m_LaunchPos.x + m_Translate.x, m_LaunchPos.y + m_Translate.y }, HitShape))
		{
			m_HitTarget = true;
			return true;
		}
	}
	return false;
}