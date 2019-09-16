#pragma once
#include <memory>
#include <deque>

#include "Health.h"
#include "Mana.h"
#include "Xp.h"
#include "Items.h"
#include "Player.h"

class Texture;
class Inventory
{
public:	
	Inventory(Point2f Pos);
	void Draw(const Point2f& AvatarPos, const Health& PlayerHealth, const Mana& PlayerMana, const Xp& PlayerXp) const;
	void ProcessMouseDownEvent(const SDL_MouseButtonEvent& e, const Player::PlayerClass& playerclass);

	void AddHealingPot();
	void SubstractHealingPot();
	void AddManaPot();
	void SubstractManaPot();
	void SetItems(int idx, Items equipment);
	void AddItem(Items::Equipment path);
	int GetAmountMana();
	int GetAmountHealing();
	int GetAmountItems();
	std::vector<Items> GetItems();
	
	void InitializeBasicItems(Player::PlayerClass Class);
	void SetHealing(int heal);
	void SetMana(int mana);

private:
	void InventorySwap(const Point2f& pos, const Player::PlayerClass& playerclass);

	std::unique_ptr<Texture> m_pInventoryBase;
	std::unique_ptr<Texture> m_pDeleteItem;
	std::shared_ptr<Texture> m_pHp, m_pMp, m_pHpPot, m_pMpPot, m_pAmountHP, m_pAmountMP;

	int m_AmountHP, m_AmountMP, m_SelectedItem;
	bool m_Selected;
	Point2f m_Position;
	Rectf m_DeleteItem;
	Rectf m_ItemPositions[12];
	std::vector<Items> m_Items;
};

