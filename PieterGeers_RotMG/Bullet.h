#pragma once
#include <memory>
#include "Vector2f.h"
#include "Player.h"

class Texture; 
class Bullet
{
public:
	enum class CharacterBullet
	{
		wizard,
		knight,
		archer,
		enemyArcher,
		enemyWizard,
		lion,
		babydragon,
		godSnake,
		godTree
	};

	Bullet(const Point2f& launchPos, const CharacterBullet& bullet);

	void Draw() const;
	void Update(float elapsedSec);
	void Shoot(float angle);

	bool CheckTravelledDistance();
	bool CheckCollision(Rectf HitShape);


private:
	std::shared_ptr<Texture> m_pBullet;
	
	float m_Angle;
	const float m_Speed{500.f};
	bool m_IsActivated;
	bool m_HitTarget;
	Point2f m_BulletPos, m_Translate, m_LaunchPos;
	Vector2f m_Velocity;
	Rectf m_BulletRect;
	Circlef m_Border;
	CharacterBullet m_Bullet;

};

