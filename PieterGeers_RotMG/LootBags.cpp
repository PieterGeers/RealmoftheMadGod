#include "stdafx.h"
#include "LootBags.h"
#include "Texture.h"
#include "Items.h"
#include "SoundEffect.h"

LootBags::LootBags(const Point2f& pos, int volume)
	: m_BagPosition{pos}
	, m_Life{true}
	, m_AccTime{0}
	, m_pItemHolder{ std::make_shared<Texture>("Resources/Items/itemslots.png") }
	, m_pLootBagDrop{std::make_shared<SoundEffect>("Resources/Sounds/LootBag.ogg")}
{
	SpawnLootInLootbag();
	m_pLootBagDrop->Play(0);
	m_pLootBagDrop->SetVolume(volume);
}

void LootBags::Draw(const Rectf& playerShape) const
{
	if (IsPlayerInRange({ playerShape.left + playerShape.width / 2, playerShape.bottom + playerShape.height / 2 }))
	{
		m_pItemHolder->Draw({ m_BagPosition.x + m_pLootBag->GetWidth() / 2, m_BagPosition.y + m_pLootBag->GetHeight() / 2 }, { 0,0,m_pItemHolder->GetWidth(), m_pItemHolder->GetHeight() });
		m_pItem->Draw({ m_BagPosition.x + m_pLootBag->GetWidth() / 2 + 2, m_BagPosition.y + m_pLootBag->GetHeight() / 2 + 2,m_pItemHolder->GetWidth() / 2 - 4, m_pItemHolder->GetHeight() - 4});
		if (m_pItemExtra->GetEquipment() != Items::Equipment::none)
		{
			m_pItemExtra->Draw({ m_BagPosition.x + m_pLootBag->GetWidth() / 2 + 2 + m_pItemHolder->GetWidth() / 2, m_BagPosition.y + m_pLootBag->GetHeight() / 2 + 2,m_pItemHolder->GetWidth() / 2 - 4, m_pItemHolder->GetHeight() - 4 });
		}
	}
	m_pLootBag->Draw({ m_BagPosition.x - m_pLootBag->GetWidth() / 2, m_BagPosition.y - m_pLootBag->GetHeight() / 2 }, { 0,0,m_pLootBag->GetWidth(), m_pLootBag->GetHeight() });
	
}

bool LootBags::IsPlayerInRange(const Point2f& playerPos) const
{
	if (utils::IsPointInCircle({ playerPos }, { m_BagPosition , 50 }))
	{
		return true;
	}
	return false;
}

bool LootBags::Life()
{
	return m_Life;
}

void LootBags::Update(float elapsedSec)
{
	m_AccTime += elapsedSec;
	if (m_AccTime >= 10)
	{
		m_Life = false;
	}
}

Items::Equipment LootBags::GetItem()
{
	return m_pItem->GetEquipment();
}

Items::Equipment LootBags::GetExtraItem()
{
	return m_pItemExtra->GetEquipment();
}

void LootBags::SpawnLootInLootbag()
{
	int chance{ rand() % 101 };
	if (chance == 100)
	{
		m_pLootBag = std::make_shared<Texture>("Resources/Items/purplebag.png");
		int upgradedChance{ rand() % 8 + 1 };
		if (upgradedChance == 1){m_pItem = std::make_shared<Items>(Items::Equipment::UpgradedArmor);}
		else if (upgradedChance == 2){m_pItem = std::make_shared<Items>(Items::Equipment::UpgradedBowArcher);}
		else if (upgradedChance == 3){m_pItem = std::make_shared<Items>(Items::Equipment::UpgradedQuiverArcher);}
		else if (upgradedChance == 4){m_pItem = std::make_shared<Items>(Items::Equipment::UpgradedRobeWizard);}
		else if (upgradedChance == 5){m_pItem = std::make_shared<Items>(Items::Equipment::UpgradedShieldKnight);}
		else if (upgradedChance == 6){m_pItem = std::make_shared<Items>(Items::Equipment::UpgradedSpecialWizard);}
		else if (upgradedChance == 7){m_pItem = std::make_shared<Items>(Items::Equipment::UpgradedStaffWizard);}
		else if (upgradedChance == 8){m_pItem = std::make_shared<Items>(Items::Equipment::UpgradedSwordKnight);}
		m_pItemExtra = std::make_shared<Items>(Items::Equipment::none);
	}
	else
	{
		m_pLootBag = std::make_shared<Texture>("Resources/Items/brownbag.png");
		int PotionChance(rand() % 11);
		if (PotionChance == 1)
		{
			m_pItem = std::make_shared<Items>(Items::Equipment::HealingPotion);
			m_pItemExtra = std::make_shared<Items>(Items::Equipment::ManaPotion);
		}
		else
		{
			int r(rand() % 2);
			if (r == 0)
			{
				m_pItem = std::make_shared<Items>(Items::Equipment::HealingPotion);
				m_pItemExtra = std::make_shared<Items>(Items::Equipment::none);

			}
			else if (r == 1)
			{
				m_pItem = std::make_shared<Items>(Items::Equipment::ManaPotion);
				m_pItemExtra = std::make_shared<Items>(Items::Equipment::none);

			}
		}
	}
}

void LootBags::SetVolume(int volume)
{
	m_pLootBagDrop->SetVolume(volume);
}
