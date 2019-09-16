#pragma once
#include "Bullet.h"
#include "Inventory.h"


class UI;
class SoundEffect;
class LootBags;
class Player;
class Enemy;
class Character
{
public:
	Character();

	void Update(float elapsedSec, const std::shared_ptr<Map>& level, const std::shared_ptr<UI>& UI);
	void Draw() const;
	void ProcessKeyDownEvent(const SDL_KeyboardEvent & e, std::shared_ptr<Inventory>& inventory);
	
	void SetVolume(int volume);
	bool GetIsDeath();

	std::shared_ptr<Player> GetPlayer();

private:
	std::shared_ptr<Player> m_pPlayer;
	std::deque<std::shared_ptr<Enemy>> m_pEnemys;
	std::deque<std::shared_ptr<LootBags>> m_pLootBags;
	std::shared_ptr<SoundEffect> m_pUsePotion;
	std::shared_ptr<SoundEffect> m_pError;
	std::shared_ptr<SoundEffect> m_pDeathSound;

	float m_AccTime;
	bool m_IsAlive;

	void CreateEnemies(int amount);
	void BulletHit();
	void UpdateLootBags(float elapsedSec);
	void ItemPickUp(std::shared_ptr<Inventory>& inventory);
};

