#include "stdafx.h"
#include "Inventory.h"
#include "Texture.h"

Inventory::Inventory(Point2f Pos)
	: m_pInventoryBase{ std::make_unique<Texture>("Resources/GUI/Inventory.png") }
	, m_pDeleteItem{std::make_unique<Texture>("Resources/GUI/Trashcan.png")}
	, m_Position{ Pos }
	, m_pMp{ std::make_shared<Texture>("MP", "Resources/Myriad Pro Regular.ttf", 18, Color4f{ 1.f,1.f,1.f,1.f }) }
	, m_pHp{ std::make_shared<Texture>("HP", "Resources/Myriad Pro Regular.ttf", 18, Color4f{ 1.f,1.f,1.f,1.f }) }
	, m_pHpPot{ std::make_shared<Texture>("Resources/Items/17.png") }
	, m_pMpPot{ std::make_shared<Texture>("Resources/Items/18.png") }
	, m_pAmountHP{ std::make_shared<Texture>("0 / 6", "Resources/Myriad Pro Regular.ttf", 18, Color4f{ 1.f,1.f,1.f,1.f }) }
	, m_pAmountMP{ std::make_shared<Texture>("0 / 6", "Resources/Myriad Pro Regular.ttf", 18, Color4f{ 1.f,1.f,1.f,1.f }) }
	, m_AmountHP{ 0 }
	, m_AmountMP{ 0 }
	, m_Items{}
	, m_Selected{false}
	, m_SelectedItem{-1}
{
	m_Items.reserve(12);
	for (int i{}; i < 4; ++i)
	{
		m_ItemPositions[0 + i] = { Pos.x +2.f + 64.f*i, 226.f, 60.f,60.f };
		m_ItemPositions[4 + i] = { Pos.x + 2.f + 64.f*i, 130.f, 60.f,60.f };
		m_ItemPositions[8 + i] = { Pos.x +2.f + 64.f*i, 68.f, 60.f,60.f };
	}
	m_DeleteItem = { 864, 196, m_pDeleteItem->GetWidth(), m_pDeleteItem->GetHeight() };

}

void Inventory::Draw(const Point2f& AvatarPos, const Health& PlayerHealth, const Mana& PlayerMana, const Xp& PlayerXp) const
{
	m_pInventoryBase->Draw({ m_Position }, { 0,0,m_pInventoryBase->GetWidth(), m_pInventoryBase->GetHeight() });
	utils::SetColor({ 1.f, 1.f, 0.f, 1.f });
	utils::DrawPoint({ 640.f + (AvatarPos.x / 8), 384.f + (AvatarPos.y / 8) }, 3);
	//XP
	utils::SetColor({ 0.15f,0.14f,0.14f,1.f });
	utils::DrawRect({ m_Position.x + 6.f, 295.f, 248.f, 20.f }, 3);
	PlayerXp.Draw({ m_Position.x + 6.f, 349.f }, 248.f, 20.f);
	//HP
	utils::SetColor({ 0.15f,0.14f,0.14f,1.f });
	utils::DrawRect({ m_Position.x + 6.f, 322.f, 248.f, 20.f }, 3);
	PlayerHealth.Draw({ m_Position.x + 6.f, 322.f }, 248.f, 20.f);
	m_pHp->Draw({ m_Position.x + 6.f, 322.f }, { 0,0, m_pHp->GetWidth(), m_pHp->GetHeight() });
	m_pHpPot->Draw({ m_Position.x + 6.f, 34, 28,28 }, { 0,0,m_pHpPot->GetWidth(), m_pHpPot->GetHeight() });
	m_pAmountHP->Draw({ m_Position.x + 6.f + m_pHpPot->GetWidth(), 36 }, { 0,0,m_pAmountHP->GetWidth(), m_pAmountHP->GetHeight() });
	//MP
	utils::SetColor({ 0.15f,0.14f,0.14f,1.f });
	utils::DrawRect({ m_Position.x + 6.f, 349.f, 248.f, 20.f }, 3);
	PlayerMana.Draw({ m_Position.x + 6.f, 295.f }, 248.f, 20.f);
	m_pMp->Draw({ m_Position.x + 6.f, 295.f }, { 0,0, m_pMp->GetWidth(), m_pMp->GetHeight() });
	m_pMpPot->Draw({ m_Position.x + 134.f, 34, 28,28 }, { 0,0,m_pMpPot->GetWidth(), m_pMpPot->GetHeight() });
	m_pAmountMP->Draw({ m_Position.x + 134.f + m_pMpPot->GetWidth(), 36 }, { 0,0,m_pAmountMP->GetWidth(), m_pAmountMP->GetHeight() });
	//draw items in slots 
	for (int i{}; i < 12; ++i)
	{
		if (m_Items[i].GetEquipment() != Items::Equipment::none)
		{
			m_Items[i].Draw({ m_ItemPositions[i] });
		}
	}
	m_pDeleteItem->Draw({m_DeleteItem.left, m_DeleteItem.bottom});
}

void Inventory::AddHealingPot()
{
	++m_AmountHP;
	m_pAmountHP = std::make_shared<Texture>(std::to_string(m_AmountHP) + " / 6", "Resources/Myriad Pro Regular.ttf", 18, Color4f{ 1.f,1.f,1.f,1.f });
}
void Inventory::SubstractHealingPot()
{
	--m_AmountHP;
	m_pAmountHP = std::make_shared<Texture>(std::to_string(m_AmountHP) + " / 6", "Resources/Myriad Pro Regular.ttf", 18, Color4f{ 1.f,1.f,1.f,1.f });
}
void Inventory::AddManaPot()
{
	++m_AmountMP;
	m_pAmountMP = std::make_shared<Texture>(std::to_string(m_AmountMP) + " / 6", "Resources/Myriad Pro Regular.ttf", 18, Color4f{ 1.f,1.f,1.f,1.f });
}
void Inventory::SubstractManaPot()
{
	--m_AmountMP;
	m_pAmountMP = std::make_shared<Texture>(std::to_string(m_AmountMP) + " / 6", "Resources/Myriad Pro Regular.ttf", 18, Color4f{ 1.f,1.f,1.f,1.f });
}
int Inventory::GetAmountMana()
{
	return m_AmountMP;
}
int Inventory::GetAmountHealing()
{
	return m_AmountHP;
}

void Inventory::AddItem(Items::Equipment path)
{
	for (int i{ 4 }; i <12 ; ++i)
	{
		if (m_Items[i].GetEquipment() == Items::Equipment::none)
		{
			m_Items[i] = Items(path);
			return;
		}
	}
}
int Inventory::GetAmountItems()
{
	int amount{ 4 };
	for (int i{4}; i < 12; ++i)
	{
		if (m_Items[i].GetEquipment() != Items::Equipment::none)
		{
			++amount;
		}
	}
	return amount;
}

void Inventory::InitializeBasicItems(Player::PlayerClass Class)
{
	switch (Class)
	{
	case Player::PlayerClass::wizard:
		m_Items.push_back(Items(Items::Equipment::DefaultStaffWizard));
		m_Items.push_back(Items(Items::Equipment::DefaultSpecialWizard));
		m_Items.push_back(Items(Items::Equipment::DefaultRobeWizard));
		break;
	case Player::PlayerClass::knight:
		m_Items.push_back(Items(Items::Equipment::DefaultSwordKnight));
		m_Items.push_back(Items(Items::Equipment::DefaultShieldKnight));
		m_Items.push_back(Items(Items::Equipment::DefaultArmor));
		break;
	case Player::PlayerClass::archer:
		m_Items.push_back(Items(Items::Equipment::DefaultBowArcher));
		m_Items.push_back(Items(Items::Equipment::DefaultQuiverArcher));
		m_Items.push_back(Items(Items::Equipment::DefaultArmor));
		break;
	}
	for (int i{3}; i < 12; ++i)
	{
		m_Items.push_back(Items(Items::Equipment::none));
	}
}

std::vector<Items> Inventory::GetItems()
{
	return m_Items;
}

void Inventory::SetItems(int idx, Items equipment)
{
	if (idx < 12)
	{
		m_Items.push_back(equipment);
	}
}

void Inventory::SetHealing(int heal)
{
	m_AmountHP = heal;
	m_pAmountHP = std::make_shared<Texture>(std::to_string(m_AmountHP) + " / 6", "Resources/Myriad Pro Regular.ttf", 18, Color4f{ 1.f,1.f,1.f,1.f });
}

void Inventory::SetMana(int mana)
{
	m_AmountMP = mana;
	m_pAmountMP = std::make_shared<Texture>(std::to_string(m_AmountMP) + " / 6", "Resources/Myriad Pro Regular.ttf", 18, Color4f{ 1.f,1.f,1.f,1.f });
}

void Inventory::ProcessMouseDownEvent(const SDL_MouseButtonEvent& e, const Player::PlayerClass& playerclass)
{
	InventorySwap({float(e.x), float(640 - e.y)}, playerclass);
}

void Inventory::InventorySwap(const Point2f& pos, const Player::PlayerClass& playerclass)
{
	if (m_Selected)
	{
		if (utils::IsPointInRect(pos, m_DeleteItem))
		{
			m_Items[m_SelectedItem] = Items(Items::Equipment::none);
			m_Selected = false;
			return;
		}
		switch (playerclass)
		{
		case Player::PlayerClass::wizard:
			if (utils::IsPointInRect(pos, m_ItemPositions[0]) && m_Items[m_SelectedItem].GetEquipment() == Items::Equipment::UpgradedStaffWizard)
			{
				std::swap(m_Items[0], m_Items[m_SelectedItem]);
				m_Selected = false;
				return;
			}
			else if (utils::IsPointInRect(pos, m_ItemPositions[1]) && m_Items[m_SelectedItem].GetEquipment() == Items::Equipment::UpgradedSpecialWizard)
			{
				std::swap(m_Items[1], m_Items[m_SelectedItem]);
				m_Selected = false;
				return;
			}
			else if (utils::IsPointInRect(pos, m_ItemPositions[2]) && m_Items[m_SelectedItem].GetEquipment() == Items::Equipment::UpgradedRobeWizard)
			{
				std::swap(m_Items[2], m_Items[m_SelectedItem]);
				m_Selected = false;
				return;
			}
			break;
		case Player::PlayerClass::knight:
			if (utils::IsPointInRect(pos, m_ItemPositions[0]) && m_Items[m_SelectedItem].GetEquipment() == Items::Equipment::UpgradedSwordKnight)
			{
				std::swap(m_Items[0], m_Items[m_SelectedItem]);
				m_Selected = false;
				return;
			}
			else if (utils::IsPointInRect(pos, m_ItemPositions[1]) && m_Items[m_SelectedItem].GetEquipment() == Items::Equipment::UpgradedShieldKnight)
			{
				std::swap(m_Items[1], m_Items[m_SelectedItem]);
				m_Selected = false;
				return;
			}
			else if (utils::IsPointInRect(pos, m_ItemPositions[2]) && m_Items[m_SelectedItem].GetEquipment() == Items::Equipment::UpgradedArmor)
			{
				std::swap(m_Items[2], m_Items[m_SelectedItem]);
				m_Selected = false;
				return;
			}
			break;
		case Player::PlayerClass::archer:
			if (utils::IsPointInRect(pos, m_ItemPositions[0]) && m_Items[m_SelectedItem].GetEquipment() == Items::Equipment::UpgradedBowArcher)
			{
				std::swap(m_Items[0], m_Items[m_SelectedItem]);
				m_Selected = false;
				return;
			}
			else if (utils::IsPointInRect(pos, m_ItemPositions[1]) && m_Items[m_SelectedItem].GetEquipment() == Items::Equipment::UpgradedQuiverArcher)
			{
				std::swap(m_Items[1], m_Items[m_SelectedItem]);
				m_Selected = false;
				return;
			}
			else if (utils::IsPointInRect(pos, m_ItemPositions[2]) && m_Items[m_SelectedItem].GetEquipment() == Items::Equipment::UpgradedArmor)
			{
				std::swap(m_Items[2], m_Items[m_SelectedItem]);
				m_Selected = false;
				return;
			}
			break;
		}
	}
	for (int i{}; i < 12; ++i)
	{
		if (!m_Selected)
		{
			if (utils::IsPointInRect(pos, m_ItemPositions[i]) && m_Items[i].GetEquipment() != Items::Equipment::none && i > 3)
			{
				m_SelectedItem = i;
				m_Selected = true;
				return;
			}
		}
		else if (m_Selected)
		{
			if (utils::IsPointInRect(pos, m_ItemPositions[i]) && i > 3)
			{
				std::swap(m_Items[i], m_Items[m_SelectedItem]);
				m_Selected = false;
				return;
			}
		}
	}
}