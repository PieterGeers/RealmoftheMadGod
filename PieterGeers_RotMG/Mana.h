#pragma once
#include <memory>

class SoundEffect;
class Mana
{
public:
	Mana(int mana);

	void Regenerate(float elapsedSec);
	void Draw(Point2f pos, float width, float height) const;
	
	bool UseMana();
	void SetMaxMana();
	void SetMaxMana(int level);
	void Set(int Mana);
	int Get();
	int GetMax();
	void SetVolume(int volume);

private:
	std::shared_ptr<SoundEffect> m_pNoMana;

	int m_Mana, m_MaxMana;
	float m_AccTime;
};

