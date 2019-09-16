#pragma once
#include <memory>

class SoundEffect;
class Xp
{
public:
	Xp();
	bool AddXp(int value);
	void Draw(Point2f pos, float width, float height) const;
	int GetLevel() const;
	int GetXp() const;
	void SetVolume(int volume);

	void SetLevel(int level);
	void SetXp(int currentXp);


private:
	int m_Level, m_MaxXp, m_Xp;
	bool m_LevelUp;

	std::shared_ptr<SoundEffect> m_pLevelUpSound;
};

