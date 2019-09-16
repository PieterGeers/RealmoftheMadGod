#pragma once
#include <memory>
#include "Items.h"

class SoundEffect;
class Texture;
class LootBags
{
public:
	LootBags(const Point2f& pos, int volume);

	void Draw(const Rectf& playerShape) const;
	void Update(float elapsedSec);
	
	bool Life();
	bool IsPlayerInRange(const Point2f& playerPos) const;
	Items::Equipment GetItem();
	Items::Equipment GetExtraItem();
	
	void SetVolume(int volume);
	
private:
	void SpawnLootInLootbag();

	std::shared_ptr<Texture> m_pLootBag;
	std::shared_ptr<Texture> m_pItemHolder;
	std::shared_ptr<Items> m_pItem;
	std::shared_ptr<Items> m_pItemExtra;
	std::shared_ptr<SoundEffect> m_pLootBagDrop;

	float m_AccTime;
	bool m_Life;
	Point2f m_BagPosition;	
};

