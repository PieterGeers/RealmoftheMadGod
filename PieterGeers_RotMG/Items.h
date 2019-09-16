#pragma once
#include <memory>

class Texture;
class Items
{
public:
	enum class Equipment
	{
		none,

		DefaultStaffWizard,
		UpgradedStaffWizard,
		DefaultSpecialWizard,
		UpgradedSpecialWizard,
		DefaultRobeWizard,
		UpgradedRobeWizard,

		DefaultSwordKnight,
		DefaultShieldKnight,
		UpgradedSwordKnight,
		UpgradedShieldKnight,

		DefaultBowArcher,
		DefaultQuiverArcher,
		UpgradedBowArcher,
		UpgradedQuiverArcher,

		DefaultArmor,
		UpgradedArmor,

		HealingPotion,
		ManaPotion,

		Ring
	};

	explicit Items(Equipment item);
	void Draw(const Rectf& pos) const;
	Equipment GetEquipment() const;

private:
	std::shared_ptr<Texture> m_pItem;
	Equipment m_Item;
};

