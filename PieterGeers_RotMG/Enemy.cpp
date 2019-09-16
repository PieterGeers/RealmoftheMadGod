#include "stdafx.h"
#include "Enemy.h"
#include "Bullet.h"
#include "Texture.h"
#include "SoundEffect.h"

Enemy::Enemy(Point2f pos, int volume)
	: m_MidPosition{pos}
	, m_AccTime{0}
	, m_EnemyHealth{0}
	, m_TravelTime{0}
	, m_AccTimeMovement{0}
	, m_pEnemyHit{std::make_shared<SoundEffect>("Resources/Sounds/EnemyHit.ogg")}
	, m_pEnemyShoot{std::make_shared<SoundEffect>("Resources/Sounds/EnemyBullet.ogg")}
	, m_Speed{200}
{
	int i{ rand() % 100 };
	if (i >= 0 && i < 5)
	{
		if (rand() % 2 == 1){m_Enemy = EnemieClass::godSnake;}
		else{m_Enemy = EnemieClass::godTree;}
	}
	else
	{
		i = rand() % 4;
		if (i == 0){m_Enemy = EnemieClass::enemyArcher;}
		else if (i == 1){m_Enemy = EnemieClass::babydragon;}
		else if (i == 2){m_Enemy = EnemieClass::enemyWizard;}
		else if (i == 3){m_Enemy = EnemieClass::lion;}
	}
	m_pEnemyHit->SetVolume(volume);
	m_pEnemyShoot->SetVolume(volume);
	InitializeEnemie();
}

void Enemy::Draw() const
{
	if (m_pBullets.size() > 0)
	{
		for (std::shared_ptr<Bullet> bullet : m_pBullets)
		{
			bullet->Draw();
		}
	}
	if (m_Direction == Direction::left)
	{
		glPushMatrix();
		glScalef(-1.f, 1.f, 1.f);
		glTranslatef( -m_MidPosition.x * 2, 0.f, 0.f);
		m_pEnemy->Draw({ m_MidPosition.x - m_pEnemy->GetWidth() / 2, m_MidPosition.y - m_pEnemy->GetHeight() / 2 }, { 0,0,m_pEnemy->GetWidth(),m_pEnemy->GetHeight() });
		glPopMatrix();
	}
	else
	{
		m_pEnemy->Draw({ m_MidPosition.x - m_pEnemy->GetWidth() / 2, m_MidPosition.y - m_pEnemy->GetHeight() / 2 }, { 0,0,m_pEnemy->GetWidth(),m_pEnemy->GetHeight() });
	}
	m_EnemyHealth.Draw({ GetShape() });
}

void Enemy::Update(float elapsedSec, Point2f AvatarPos, const std::shared_ptr<Map>& level)
{
	if (utils::IsPointInCircle({ AvatarPos }, { {m_MidPosition}, 400.f }))
	{
		m_AccTime += elapsedSec;
		if (m_AccTime > 0.5f)
		{
			switch (m_Enemy)
			{
			case Enemy::EnemieClass::enemyArcher:
				m_pBullets.push_back(std::make_shared<Bullet>(m_MidPosition, Bullet::CharacterBullet::enemyArcher));
				break;
			case Enemy::EnemieClass::enemyWizard:
				m_pBullets.push_back(std::make_shared<Bullet>(m_MidPosition, Bullet::CharacterBullet::enemyWizard));
				break;
			case Enemy::EnemieClass::lion:
				m_pBullets.push_back(std::make_shared<Bullet>(m_MidPosition, Bullet::CharacterBullet::lion));
				break;
			case Enemy::EnemieClass::babydragon:
				m_pBullets.push_back(std::make_shared<Bullet>(m_MidPosition, Bullet::CharacterBullet::babydragon));
				break;
			case Enemy::EnemieClass::godSnake:
				m_pBullets.push_back(std::make_shared<Bullet>(m_MidPosition, Bullet::CharacterBullet::godSnake));
				break;
			case Enemy::EnemieClass::godTree:
				m_pBullets.push_back(std::make_shared<Bullet>(m_MidPosition, Bullet::CharacterBullet::godTree));
				break;
			}
			CheckShootDirection({ AvatarPos });
			m_AccTime -= 0.5f;
		}
	}
	if (m_pBullets.size() > 0)
	{
		for (int i{ 0 }; i <= int(m_pBullets.size() - 1); ++i)
		{
			m_pBullets[i]->Update(elapsedSec);
			if (m_pBullets[i]->CheckTravelledDistance() == true)
			{
				m_pBullets.front().reset();
				m_pBullets.pop_front();
			}
		}
	}
	Movement(elapsedSec, AvatarPos, level);
}

void Enemy::Movement(float elapsedSec, Point2f enemyPos, const std::shared_ptr<Map>& level)
{
	if (utils::IsPointInCircle({ enemyPos }, { { m_MidPosition }, m_PlayerInRange }) )
	{
		m_PosVelocity.x = m_Speed*elapsedSec;
		m_PosVelocity.y = m_Speed*elapsedSec;
		m_NegVelocity.x = m_Speed*elapsedSec;
		m_NegVelocity.y = m_Speed*elapsedSec;
	
		if (m_AccTimeMovement == 0.f)
		{
			m_TravelTime = (rand() % 100) / 100.f;
			m_Direction = Direction(rand() % 5);
			if (m_Direction == Direction::none && m_TravelTime > 0.25f)
			{
				m_TravelTime = 0.15f;
			}
		}
		m_AccTimeMovement += elapsedSec;
		if (m_AccTimeMovement > m_TravelTime)
		{
			m_AccTimeMovement = 0.f;
		}
		level->HandleCollision(GetShape(), m_PosVelocity, m_NegVelocity);
		switch (m_Direction)
		{
		case Enemy::Direction::none:
			break;
		case Enemy::Direction::up:
			m_MidPosition.y += m_PosVelocity.y;
			break;
		case Enemy::Direction::down:
			m_MidPosition.y -= m_NegVelocity.y;
			break;
		case Enemy::Direction::left:
			m_MidPosition.x -= m_NegVelocity.x;
			break;
		case Enemy::Direction::right:
			m_MidPosition.x += m_PosVelocity.x;
			break;
		default:
			break;
		}
	}
}

void Enemy::CheckShootDirection(const Point2f& toPos)
{
	Vector2f xAs{ 1,0 };
	Vector2f shootVector{ { m_MidPosition },{ toPos } };

	float angle{ xAs.AngleWith(shootVector) };
	m_pBullets[m_pBullets.size() - 1]->Shoot(angle);
	m_pEnemyShoot->Play(0);

}

Rectf Enemy::GetShape() const
{
	return Rectf{ m_MidPosition.x - m_pEnemy->GetWidth() / 2, m_MidPosition.y - m_pEnemy->GetHeight() / 2, m_pEnemy->GetWidth(), m_pEnemy->GetHeight() };
}

std::deque<std::shared_ptr<Bullet>> Enemy::GetBullets()
{
	return m_pBullets;
}

void Enemy::Hit(int playerLevel)
{
	m_EnemyHealth.TakeDamage(m_BaseDamage + m_AdditionalDamage*playerLevel);
	m_pEnemyHit->Play(0);
}

int Enemy::GetHealth()
{
	return m_EnemyHealth.Get();
}

Point2f Enemy::GetMidPos()
{
	return m_MidPosition;
}

void Enemy::SetVolume(int volume)
{
	m_pEnemyHit->SetVolume(volume);
	m_pEnemyShoot->SetVolume(volume);
}

void Enemy::InitializeEnemie()
{
	switch (m_Enemy)
	{
	case Enemy::EnemieClass::enemyArcher:
		m_pEnemy = std::make_shared<Texture>("Resources/Characters/Sandsman_Archer.png");
		m_EnemyHealth.Initialize(100);
		m_Shape = { 0,0,m_pEnemy->GetWidth(), m_pEnemy->GetHeight() };
		break;
	case Enemy::EnemieClass::enemyWizard:
		m_pEnemy = std::make_shared<Texture>("Resources/Characters/Sandsman_Sorcerer.png");
		m_EnemyHealth.Initialize(100);
		m_Shape = { 0,0,m_pEnemy->GetWidth(), m_pEnemy->GetHeight() };
		break;
	case Enemy::EnemieClass::lion:
		m_pEnemy = std::make_shared<Texture>("Resources/Characters/Werelion.png");
		m_EnemyHealth.Initialize(150);
		m_Shape = { 0,0,m_pEnemy->GetWidth(), m_pEnemy->GetHeight() };
		break;
	case Enemy::EnemieClass::babydragon:
		m_pEnemy = std::make_shared<Texture>("Resources/Characters/Drake_Baby.png");
		m_EnemyHealth.Initialize(250);
		m_Shape = { 0,0,m_pEnemy->GetWidth(), m_pEnemy->GetHeight() };
		break;
	case Enemy::EnemieClass::godSnake:
		m_pEnemy = std::make_shared<Texture>("Resources/Characters/Snake_God.png");
		m_EnemyHealth.Initialize(700);
		m_Shape = { 0,0,m_pEnemy->GetWidth(), m_pEnemy->GetHeight() };
		break;
	case Enemy::EnemieClass::godTree:
		m_pEnemy = std::make_shared<Texture>("Resources/Characters/Ent_God.png");
		m_EnemyHealth.Initialize(700);
		m_Shape = { 0,0,m_pEnemy->GetWidth(), m_pEnemy->GetHeight() };
		break;
	}
}

int Enemy::GetClass()
{
	return int(m_Enemy);
}