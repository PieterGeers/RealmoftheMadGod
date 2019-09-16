#pragma once
#include <memory>
#include <deque>

#include "Health.h"
#include "Map.h"
#include "Vector2f.h"


class SoundEffect;
class Bullet;
class Texture;
class Enemy 
{
public: 
	enum class EnemieClass
	{
		enemyArcher,
		enemyWizard,
		lion,
		babydragon,
		godSnake,
		godTree
	};
	enum class Direction
	{
		none,
		up,
		down,
		left,
		right
	};

	explicit Enemy(Point2f pos, int volume);

	void Draw() const;
	void Update(float elapsedSec, Point2f enemyPos, const std::shared_ptr<Map>& level);

	void Hit(int playerLevel);
	void SetVolume(int volume);

	int GetHealth();
	Point2f GetMidPos();
	Rectf GetShape() const;
	std::deque<std::shared_ptr<Bullet>> GetBullets();
	int GetClass();

private:
	void CheckShootDirection(const Point2f& toPos);
	void Movement(float elapsedSec, Point2f AvatarPos, const std::shared_ptr<Map>& level);
	void InitializeEnemie();

	std::deque<std::shared_ptr<Bullet>> m_pBullets;

	std::shared_ptr<SoundEffect> m_pEnemyShoot;
	std::shared_ptr<SoundEffect> m_pEnemyHit;
	std::shared_ptr<Texture> m_pEnemy;

	const int m_BaseDamage{ 20 };
	const int m_AdditionalDamage{ 2 };
	float m_AccTime, m_AccTimeMovement, m_TravelTime, m_Speed;
	const float m_PlayerInRange{ 600.f };
	Point2f m_MidPosition;
	Vector2f m_PosVelocity, m_NegVelocity;
	Rectf m_Shape;
	Health m_EnemyHealth;
	Direction m_Direction;
	EnemieClass m_Enemy;
};

