#pragma once
#include <deque>
#include <memory>

#include "Vector2f.h"
#include "Map.h"
#include "Health.h"
#include "Mana.h"
#include "Xp.h"
#include "Vegetation.h"

class SoundEffect;
class Texture;
class Bullet;
class Player
{
public:
	enum class AvatarState
	{
		running,
		special,
		attack,
		none
	};
	enum class Direction
	{
		right,
		left,
		down,
		up
	};
	enum class PlayerClass
	{
		wizard,
		knight,
		archer,
		none
	};
	explicit Player(const Point2f& pos, const PlayerClass& playerclass);

	Point2f GetMidPos() const;
	void Draw() const;
	void DrawXp() const;
	void Update(float elapsedSec, const std::shared_ptr<Map>& level);
	void ProcessMouseDownEvent(const SDL_MouseButtonEvent& e);
	void ProcessMouseUpEvent(const SDL_MouseButtonEvent& e);
	void ProcessMouseMotionEvent(const SDL_MouseMotionEvent& e);
	void ProcessKeyDownEvent(const SDL_KeyboardEvent & e);
	
	Rectf GetShape() const;
	void SetTranslationCamera(const Point2f& cameraLocation);
	std::deque<std::shared_ptr<Bullet>> GetBullets();
	int GetVolume();
	void Hit(int damage);
	Health GetHealth() const;
	Mana GetMana() const;
	Xp GetXp() const;
	void GiveXp();
	bool UseHPPotion();
	bool UseManaPotion();
	int GetClass();
	void SetVolume(int volume);
	bool IsDeath();


	void SetClass(const PlayerClass& player);
	void SetLevel(int level);
	void SetXp(int xp);

private:
	std::unique_ptr<Texture> m_pPlayerTexture;
	std::deque<std::shared_ptr<Bullet>> m_pBullets;
	std::shared_ptr<SoundEffect> m_pPlayerShoot;
	std::shared_ptr<SoundEffect> m_pPlayerHit;

	int m_Frame;
	float m_AccTime, m_SpriteWidth{ 96 }, m_SpriteHeight{ 48 }, m_AccShootTime, m_Speed{ 200 };
	bool m_MouseHold, m_ShootWhenHold;
	Point2f m_MidPos, m_TranslationCamera;
	Vector2f m_PosVelocity, m_NegVelocity;

	Texture m_Xp;
	Direction m_MyDirection;
	AvatarState m_AvatarState;
	Health m_AvatarHealth;
	Mana m_AvatarMana;
	Xp m_AvatarXp;
	PlayerClass m_PlayerClass;

	void CheckShootDirection(const Point2f& toPos);
	void UpdateDirections();
	void DoSpecialAttack();
};

