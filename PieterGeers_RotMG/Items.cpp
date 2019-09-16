#include "stdafx.h"
#include "Items.h"
#include "Texture.h"

Items::Items(Equipment item)
	: m_Item{item}
{
	if (item != Equipment::none)
	{
		m_pItem = std::make_shared<Texture>("Resources/Items/" + std::to_string(int(item)) + ".png");
	}
}


void Items::Draw(const Rectf& pos) const
{
	m_pItem->Draw({ pos }, { 0,0,m_pItem->GetWidth(), m_pItem->GetHeight() });
}

Items::Equipment Items::GetEquipment() const
{
	return m_Item;
}