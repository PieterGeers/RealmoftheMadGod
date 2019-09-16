#pragma once
#include <memory>

#include "Player.h"
#include "Health.h"
#include "Mana.h"
#include "Xp.h"

#include <string>
#include <fstream>
#include <sstream>

class SoundStream;
class Texture;
class Inventory;
class UI
{
public:
	UI(Point2f pos);
	void Draw(const Point2f& AvatarPos, const Health& PlayerHealth, const Mana& PlayerMana, const Xp& PlayerXp) const;
	void ProcessMouseDownEvent(const SDL_MouseButtonEvent& e, const Player::PlayerClass& playerclass);
	std::shared_ptr<Inventory>& GetInventory();

	bool IsGamePause();
	void SetGamePause(bool pause);
	bool StartLoadFromFile();
	void SetStartLoadFromFile(bool LoadFromFile);

	bool StartNewGame();
	void SetStartNewGame(bool StartNewGame);
	Player::PlayerClass SetPlayerClass();

	bool GetChangeVolume();
	void SetChangeVolume(bool volume);

	int GetVolume();
	bool IsGameInitialized();
	void SetBoolInitialize(bool initialize);

	void SetBoolDeath(bool isDeath);
private:
	std::shared_ptr<Inventory> m_pInventory;

	//Pictures
	std::shared_ptr<Texture> m_pStartScreen;
	std::shared_ptr<Texture> m_pClassScreen;
	std::shared_ptr<Texture> m_pLoadFromFile;
	std::shared_ptr<Texture> m_pControls;
	std::shared_ptr<Texture> m_pBack;

	//Text
	std::shared_ptr<Texture> m_pPlay;
	std::shared_ptr<Texture> m_pNewGame;
	std::shared_ptr<Texture> m_pLoadGame;
	std::shared_ptr<Texture> m_pOptions;
	std::shared_ptr<Texture> m_pHelp;
	std::shared_ptr<Texture> m_pVolume;
	std::shared_ptr<Texture> m_pVolumeAmount;
	std::shared_ptr<Texture> m_pYouDied;

	std::shared_ptr<SoundStream> m_pStartSound;

	//clickable rectf
	Rectf m_Play;
	Rectf m_NewGame;
	Rectf m_LoadGame;
	Rectf m_Options;
	Rectf m_Help;
	Rectf m_Back;
	Rectf m_Volume;

	bool m_PauseAll;
	bool m_DrawStartScreen;
	bool m_DrawOptions;
	bool m_DrawControls;
	bool m_DrawClass;
	bool m_StartLoadFromFile;
	bool m_StartNewGame;
	bool m_Initialize;
	bool m_IsSaveFile;
	bool m_DrawVolumeOptions;
	bool m_ChangeVolume;
	bool m_IsDeath;

	Player::PlayerClass m_Class;
	float m_VolumePercent;

	void CheckIfSaveFile();
};

