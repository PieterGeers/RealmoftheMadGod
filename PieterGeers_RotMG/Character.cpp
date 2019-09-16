#include "stdafx.h"
#include "Character.h"
#include "Player.h"
#include "Enemy.h"
#include "LootBags.h"
#include "SoundEffect.h"
#include "UI.h"

Character::Character()
	: m_pPlayer{ std::make_shared<Player>(Point2f{800,300}, Player::PlayerClass::archer) }
	, m_AccTime{0}
	, m_pUsePotion{ std::make_shared<SoundEffect>("Resources/Sounds/UsePotion.ogg") }
	, m_pError{std::make_shared<SoundEffect>("Resources/Sounds/Error.ogg")}
	, m_pDeathSound{std::make_shared<SoundEffect>("Resources/Sounds/DeathSound.ogg")}
	, m_IsAlive{true}
{
	CreateEnemies(5);
}

void Character::CreateEnemies(int amount)
{
	int x{};
	int y{};
	int volume{ 100 };
	for (int i{}; i < amount; ++i)
	{
		x = rand() % 1728 + 160;
		y = rand() % 768 + 864;
		m_pEnemys.push_back(std::make_shared<Enemy>(Point2f{ float(x), float(y) }, volume)); 
	}
}


void Character::Draw() const
{
	m_pPlayer->Draw();
	if (m_pEnemys.size() != 0)
	{
		for (std::shared_ptr<Enemy> enemy : m_pEnemys)
		{
			enemy->Draw();
		}
	}
	if (m_pLootBags.size() != 0)
	{
		for (std::shared_ptr<LootBags> lootBag : m_pLootBags)
		{
			lootBag->Draw(m_pPlayer->GetShape());
		}
	}
}

void Character::Update(float elapsedSec, const std::shared_ptr<Map>& level, const std::shared_ptr<UI>& UI)
{
	m_pPlayer->Update(elapsedSec, level);
	UpdateLootBags(elapsedSec);
	m_AccTime += elapsedSec;
	if (m_AccTime >= 5.0f)
	{
		if (m_pEnemys.size() != 10)
		{
			int x{ rand() % 1728 + 160 };
			int y{ rand() % 768 + 864 };
			m_pEnemys.push_back(std::make_shared<Enemy>(Point2f{ float(x), float(y) }, m_pPlayer->GetVolume()));
		}
		m_AccTime = 0;
	}
	if (m_pEnemys.size() != 0)
	{
		for (unsigned i{}; i != m_pEnemys.size(); ++i)
		{
			m_pEnemys[i]->Update(elapsedSec, m_pPlayer->GetMidPos(), level);
			if (m_pEnemys[i]->GetHealth() <= 0)
			{
				if (rand() % 100 + 1 <= 40)
				{
					m_pLootBags.push_back(std::make_shared<LootBags>(m_pEnemys[i]->GetMidPos(), m_pPlayer->GetVolume()));
				}
				if (m_pEnemys.size() != 1 && i != 0)
				{
					m_pEnemys[i].swap(m_pEnemys[0]);
				}
				m_pEnemys.pop_front();
				m_pPlayer->GiveXp();
				if (i == m_pEnemys.size())
				{
					i = 0;
				}
				if (m_pEnemys.size() == 0)
				{
					return;
				}
			}
		}
		BulletHit();
	}
	if (m_pPlayer->IsDeath() && m_IsAlive)
	{
		m_pDeathSound->Play(0);
		m_IsAlive = false;
		UI->SetGamePause(true);
		UI->SetBoolDeath(true);
	}
}

void Character::UpdateLootBags(float elapsedSec)
{
	if (m_pLootBags.size() != 0)
	{
		for (unsigned i{}; i != m_pLootBags.size(); ++i)
		{
			m_pLootBags[i]->Update(elapsedSec);
			if (m_pLootBags[i]->Life() == false)
			{
				m_pLootBags.pop_front();
				if (m_pLootBags.size() == 0)
				{
					return;
				}
			}
		}
	}
}

void Character::BulletHit()
{
	for (unsigned i{}; i != m_pEnemys.size(); ++i)
	{
		for (unsigned j{}; j != m_pPlayer->GetBullets().size(); ++j)
		{
			if (m_pPlayer->GetBullets()[j]->CheckCollision(m_pEnemys[i]->GetShape()))
			{
				m_pEnemys[i]->Hit(m_pPlayer->GetXp().GetLevel());
			}
		}
	}
	for (unsigned i{}; i != m_pEnemys.size(); ++i)
	{
		for (unsigned j{}; j != m_pEnemys[i]->GetBullets().size(); ++j)
		{
			if (m_pEnemys[i]->GetBullets()[j]->CheckCollision(m_pPlayer->GetShape()))
			{
				Enemy::EnemieClass EnemyClass{ Enemy::EnemieClass(m_pEnemys[i]->GetClass()) };
				switch (EnemyClass)
				{
				case Enemy::EnemieClass::enemyArcher:
					m_pPlayer->Hit(10);
					break;
				case Enemy::EnemieClass::enemyWizard:
					m_pPlayer->Hit(10);
					break;
				case Enemy::EnemieClass::lion:
					m_pPlayer->Hit(15);
					break;
				case Enemy::EnemieClass::babydragon:
					m_pPlayer->Hit(20);
					break;
				case Enemy::EnemieClass::godSnake:
					m_pPlayer->Hit(50);
					break;
				case Enemy::EnemieClass::godTree:
					m_pPlayer->Hit(50);
					break;
				}
			}
		}
	}
}

std::shared_ptr<Player> Character::GetPlayer()
{
	return m_pPlayer;
}

void Character::ProcessKeyDownEvent(const SDL_KeyboardEvent & e, std::shared_ptr<Inventory>& inventory)
{
	m_pPlayer->ProcessKeyDownEvent(e);
	if (e.keysym.sym == SDLK_e)
	{
		ItemPickUp(inventory);
	}
	if (e.keysym.sym == SDLK_z)
	{
		if (inventory->GetAmountHealing() > 0)
		{
			if (m_pPlayer->UseHPPotion())
			{
				inventory->SubstractHealingPot();
				m_pUsePotion->Play(0);
			}
			else
			{
				m_pError->Play(0);
			}
		}
		else
		{
			m_pError->Play(0);
		}
	}
	if (e.keysym.sym == SDLK_x)
	{
		if (inventory->GetAmountMana() > 0)
		{
			if (m_pPlayer->UseManaPotion())
			{
				inventory->SubstractManaPot();
				m_pUsePotion->Play(0);
			}
			else
			{
				m_pError->Play(0);
			}
		}
		else
		{
			m_pError->Play(0);
		}
	}
}

void Character::ItemPickUp(std::shared_ptr<Inventory>& inventory)
{
	if (m_pLootBags.size() != 0)
	{
		for (unsigned i{}; i != m_pLootBags.size(); ++i)
		{
			if (m_pLootBags[i]->IsPlayerInRange(m_pPlayer->GetMidPos()))
			{
				if (m_pLootBags[i]->GetItem() == Items::Equipment::HealingPotion && 
					m_pLootBags[i]->GetExtraItem() == Items::Equipment::none && inventory->GetAmountHealing() < 6)
				{
					inventory->AddHealingPot();
					if (m_pLootBags.size() != 1 && i != 0)
					{
						m_pLootBags[i].swap(m_pLootBags[0]);
					}
					m_pLootBags.pop_front();
					if (i == m_pLootBags.size())
					{
						i = 0;
					}
					if (m_pLootBags.size() == 0)
					{
						return;
					}
				}
				else if (m_pLootBags[i]->GetItem() == Items::Equipment::ManaPotion && 
					m_pLootBags[i]->GetExtraItem() == Items::Equipment::none && inventory->GetAmountMana() < 6)
				{
					inventory->AddManaPot();
					if (m_pLootBags.size() != 1 && i != 0)
					{
						m_pLootBags[i].swap(m_pLootBags[0]);
					}
					m_pLootBags.pop_front();
					if (i == m_pLootBags.size())
					{
						i = 0;
					}
					if (m_pLootBags.size() == 0)
					{
						return;
					}
				}
				else if (m_pLootBags[i]->GetItem() == Items::Equipment::HealingPotion && 
					m_pLootBags[i]->GetExtraItem() == Items::Equipment::ManaPotion  && inventory->GetAmountHealing() < 6 ||
					m_pLootBags[i]->GetItem() == Items::Equipment::HealingPotion && 
					m_pLootBags[i]->GetExtraItem() == Items::Equipment::ManaPotion  && inventory->GetAmountMana() < 6)
				{
					if (inventory->GetAmountHealing() < 6)
					{
						inventory->AddHealingPot();
		      		}
					if (inventory->GetAmountMana() < 6)
					{
						inventory->AddManaPot();
					}
					if (m_pLootBags.size() != 1 && i != 0)
					{
						m_pLootBags[i].swap(m_pLootBags[0]);
					}
					m_pLootBags.pop_front();
					if (i == m_pLootBags.size())
					{
						i = 0;
					}
					if (m_pLootBags.size() == 0)
					{
						return;
					}
				}
				else if (m_pLootBags[i]->GetItem() != Items::Equipment::HealingPotion && m_pLootBags[i]->GetItem() != Items::Equipment::ManaPotion)
				{
					if (inventory->GetAmountItems() < 12)
					{
						inventory->AddItem(m_pLootBags[i]->GetItem());
					}
					if (m_pLootBags.size() != 1 && i != 0)
					{
						m_pLootBags[i].swap(m_pLootBags[0]);
					}
					m_pLootBags.pop_front();
					if (i == m_pLootBags.size())
					{
						i = 0;
					}
					if (m_pLootBags.size() == 0)
					{
						return;
					}
				}
			}
		}
	}
}

void Character::SetVolume(int volume)
{
	m_pPlayer->SetVolume(volume);
	m_pError->SetVolume(volume);
	if (m_pEnemys.size() != 0)
	{
		for (std::shared_ptr<Enemy> enemy : m_pEnemys)
		{
			enemy->SetVolume(volume);
		}
	}
	if (m_pLootBags.size() != 0)
	{
		for (std::shared_ptr<LootBags> lootbag : m_pLootBags)
		{
			lootbag->SetVolume(volume);
		}
	}
	m_pUsePotion->SetVolume(volume);
	m_pDeathSound->SetVolume(volume);
}

bool Character::GetIsDeath()
{
	return m_pPlayer->IsDeath();
}