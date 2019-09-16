#include "stdafx.h"
#include "UI.h"
#include "Inventory.h"
#include "Texture.h"
#include "SoundStream.h"

UI::UI(Point2f pos)
	: m_pInventory{std::make_shared<Inventory>(pos)}
	, m_pStartScreen{std::make_shared<Texture>("Resources/GUI/StartScreen.png")}
	, m_pClassScreen{std::make_shared<Texture>("Resources/GUI/ChooseClass.png")}
	, m_pLoadFromFile{std::make_shared<Texture>("Resources/GUI/LoadGame.png")}
	, m_pControls{std::make_shared<Texture>("Resources/GUI/Controls.png")}
	, m_pBack{std::make_shared<Texture>("Resources/GUI/Back.png")}
	, m_pPlay{std::make_shared<Texture>("Play", "Resources/Myriad Pro Regular.ttf", 80, Color4f{ 1.f,1.f,1.f,1.f })}
	, m_pNewGame{ std::make_shared<Texture>("New Game", "Resources/Myriad Pro Regular.ttf", 40, Color4f{1.f,1.f,1.f,1.f}) }
	, m_pLoadGame{ std::make_shared<Texture>("Load Game", "Resources/Myriad Pro Regular.ttf", 40, Color4f{1.f,1.f,1.f,1.f}) }
	, m_pOptions{ std::make_shared<Texture>("Options", "Resources/Myriad Pro Regular.ttf", 40, Color4f{1.f,1.f,1.f,1.f}) }
	, m_pHelp{ std::make_shared<Texture>("Controls", "Resources/Myriad Pro Regular.ttf", 40, Color4f{1.f,1.f,1.f,1.f}) }
	, m_pVolume{ std::make_shared<Texture>("Volume", "Resources/Myriad Pro Regular.ttf", 40, Color4f{1.f,1.f,1.f,1.f}) }
	, m_pYouDied{std::make_shared<Texture>("YOU DIED", "Resources/Myriad Pro Regular.ttf", 40, Color4f{ 1.f,1.f,1.f,1.f })}
	, m_pVolumeAmount{ std::make_shared<Texture>("100", "Resources/Myriad Pro Regular.ttf", 40, Color4f{1.f,1.f,1.f,1.f}) }
	, m_pStartSound{std::make_shared<SoundStream>("Resources/Sounds/StartupScreen.ogg")}
	, m_VolumePercent{100.f}
	, m_PauseAll{true}
	, m_DrawStartScreen{true}
	, m_DrawControls{false}
	, m_DrawOptions{false}
	, m_DrawClass{false}
	, m_StartLoadFromFile{false}
	, m_Initialize{true}
	, m_StartNewGame{ false }
	, m_DrawVolumeOptions{false}
	, m_ChangeVolume{false}
	, m_IsDeath{false}
{
	m_Play = { m_pStartScreen->GetWidth() / 2 - m_pPlay->GetWidth() / 2, 10,m_pPlay->GetWidth(), m_pPlay->GetHeight() };
	m_NewGame = { m_pLoadFromFile->GetWidth() / 2 - m_pNewGame->GetWidth() / 2, 400,m_pNewGame->GetWidth(), m_pNewGame->GetHeight() };
	m_LoadGame = { m_pLoadFromFile->GetWidth() / 2 - m_pLoadGame->GetWidth() / 2, 350,m_pLoadGame->GetWidth(), m_pLoadGame->GetHeight() };
	m_Options = { m_pLoadFromFile->GetWidth() / 2 - m_pOptions->GetWidth() / 2, 300,m_pOptions->GetWidth(), m_pOptions->GetHeight() };
	m_Help = { m_pLoadFromFile->GetWidth() / 2 - m_pHelp->GetWidth() / 2, 250,m_pHelp->GetWidth(), m_pHelp->GetHeight() };
	m_Back = { 0, m_pLoadFromFile->GetHeight() - 32 - m_pBack->GetHeight(), m_pBack->GetWidth(), m_pBack->GetHeight() };
	m_Volume = { m_pStartScreen->GetWidth() / 2 - 200, m_pStartScreen->GetHeight() / 2 - 20, 400, 40 };
	CheckIfSaveFile();
	m_pStartSound->Play(true);
}

void UI::Draw(const Point2f& AvatarPos, const Health& PlayerHealth, const Mana& PlayerMana, const Xp& PlayerXp) const
{
	if (!m_PauseAll)
	{
		m_pInventory->Draw(AvatarPos, PlayerHealth, PlayerMana, PlayerXp);
	}
	if (m_PauseAll && !m_IsDeath)
	{
		if (m_DrawStartScreen)
		{
			m_pStartScreen->Draw();
			m_pPlay->Draw({ m_pStartScreen->GetWidth() / 2 - m_pPlay->GetWidth() / 2, 10 }, { 0,0,m_pPlay->GetWidth(), m_pPlay->GetHeight() });
		}
		if (m_DrawOptions)
		{
			m_pLoadFromFile->Draw();
			m_pNewGame->Draw({ m_pLoadFromFile->GetWidth() / 2 - m_pNewGame->GetWidth() / 2, 400 }, { 0,0,m_pNewGame->GetWidth(), m_pNewGame->GetHeight() });
			m_pLoadGame->Draw({ m_pLoadFromFile->GetWidth() / 2 - m_pLoadGame->GetWidth() / 2, 350 }, { 0,0,m_pLoadGame->GetWidth(), m_pLoadGame->GetHeight() });
			m_pOptions->Draw({ m_pLoadFromFile->GetWidth() / 2 - m_pOptions->GetWidth() / 2, 300 }, { 0,0,m_pOptions->GetWidth(), m_pOptions->GetHeight() });
			m_pHelp->Draw({ m_pLoadFromFile->GetWidth() / 2 - m_pHelp->GetWidth() / 2, 250 }, { 0,0,m_pHelp->GetWidth(), m_pHelp->GetHeight() });
		}
		if (m_DrawControls)
		{
			m_pControls->Draw();
		}
		if (m_DrawClass)
		{
			m_pClassScreen->Draw();
		}
		if (m_DrawVolumeOptions)
		{
			m_pLoadFromFile->Draw();
			m_pVolume->Draw({ m_pLoadFromFile->GetWidth() / 2 - m_pVolume->GetWidth() / 2, 400 });
			utils::SetColor(Color4f{ 0.f,0.f,0.f,1.f });
			utils::DrawRect(m_Volume, 2);
			utils::SetColor(Color4f{ 1.f,1.f,1.f,1.f });
			utils::FillRect({ m_Volume.left, m_Volume.bottom, m_Volume.width / 100 * m_VolumePercent, m_Volume.height });
			m_pVolumeAmount->Draw({ m_Volume.left + m_Volume.width, m_Volume.bottom });
		}
		if (!m_DrawStartScreen && !m_DrawOptions)
		{
			m_pBack->Draw({ 0, m_pLoadFromFile->GetHeight() - 32 - m_pBack->GetHeight() }, { 0,0,m_pBack->GetWidth(), m_pBack->GetHeight() });
		}
	}
	else if (m_IsDeath)
	{
		m_pLoadFromFile->Draw();
		m_pYouDied->Draw({ m_pLoadFromFile->GetWidth() / 2 - m_pYouDied->GetWidth() / 2, m_pLoadFromFile->GetHeight() / 2 - m_pYouDied->GetHeight() / 2 }, { 0,0,m_pYouDied->GetWidth(), m_pYouDied->GetHeight() });
	}
}

std::shared_ptr<Inventory>& UI::GetInventory()
{
	return m_pInventory;
}

bool UI::IsGamePause()
{
	return m_PauseAll;
}

bool UI::StartLoadFromFile()
{
	return m_StartLoadFromFile;
}

void UI::SetGamePause(bool pause)
{
	m_PauseAll = pause;
}

Player::PlayerClass UI::SetPlayerClass()
{
	return m_Class;
}

void UI::ProcessMouseDownEvent(const SDL_MouseButtonEvent& e, const Player::PlayerClass& playerclass)
{
	if (m_PauseAll)
	{
		if (m_DrawStartScreen)
		{
			if (utils::IsPointInRect({ float(e.x), float(640 - e.y) }, { m_Play }))
			{
				m_DrawStartScreen = false;
				m_DrawOptions = true;
			}
		}
		else if (m_DrawOptions)
		{
			if (m_Initialize)
			{
				if (utils::IsPointInRect({ float(e.x), float(640 - e.y) }, { m_NewGame }))
				{
					m_DrawOptions = false;
					m_DrawClass = true;
				}
				if (m_IsSaveFile)
				{
					if (utils::IsPointInRect({ float(e.x), float(640 - e.y) }, { m_LoadGame }))
					{
						m_DrawOptions = true;
						m_StartLoadFromFile = true;
						m_PauseAll = false;
						m_Initialize = false;
						m_pStartSound->Stop();
					}
				}
			}
			if (utils::IsPointInRect({ float(e.x), float(640 - e.y) }, { m_Options }))
			{
				m_DrawOptions = false;
				m_DrawVolumeOptions = true;
			}
			if (utils::IsPointInRect({ float(e.x), float(640 - e.y) }, { m_Help }))
			{
				m_DrawOptions = false;
				m_DrawControls = true;
			}
		}
		else if (m_DrawClass)
		{
			if (utils::IsPointInRect({ float(e.x), float(640 - e.y) }, { 0,0, 300,550 }))
			{
				m_Class = Player::PlayerClass::wizard;
				m_Initialize = false;
				m_PauseAll = false;
				m_StartNewGame = true;
				m_DrawClass = false;
				m_DrawOptions = true;
				m_pStartSound->Stop();
			}
			else if (utils::IsPointInRect({ float(e.x), float(640 - e.y) }, { 300,0, 300,550 }))
			{
				m_Class = Player::PlayerClass::knight;
				m_Initialize = false;
				m_PauseAll = false;
				m_StartNewGame = true;
				m_DrawClass = false;
				m_DrawOptions = true;
				m_pStartSound->Stop();
			}
			else if (utils::IsPointInRect({ float(e.x), float(640 - e.y) }, { 600,0, 300,550 }))
			{
				m_Class = Player::PlayerClass::archer;
				m_Initialize = false;
				m_PauseAll = false;
				m_StartNewGame = true;
				m_DrawClass = false;
				m_DrawOptions = true;
				m_pStartSound->Stop();
			}
			if (utils::IsPointInRect({ float(e.x), float(640 - e.y) }, { m_Back }))
			{
				m_DrawControls = false;
				m_DrawOptions = true;
				m_DrawClass = false;
				m_DrawVolumeOptions = false;
			}
		}
		else if (m_DrawVolumeOptions)
		{
			if (utils::IsPointInRect({ float(e.x), float(640 - e.y) }, { m_Volume }))
			{
				float temp{ e.x - m_Volume.left };
				m_VolumePercent = temp / m_Volume.width * 100;
				m_pVolumeAmount = std::make_shared<Texture>(std::to_string(int(m_VolumePercent)), "Resources/Myriad Pro Regular.ttf", 40, Color4f{ 1.f,1.f,1.f,1.f });
				m_pStartSound->SetVolume(int(m_VolumePercent));
				m_ChangeVolume = true;
			}
			if (utils::IsPointInRect({ float(e.x), float(640 - e.y) }, { m_Back }))
			{
				m_DrawControls = false;
				m_DrawOptions = true;
				m_DrawClass = false;
				m_DrawVolumeOptions = false;
			}
		}
		else if (!m_DrawStartScreen)
		{
			if (utils::IsPointInRect({ float(e.x), float(640 - e.y) }, { m_Back }))
			{
				m_DrawControls = false;
				m_DrawOptions = true;
				m_DrawClass = false;
				m_DrawVolumeOptions = false;
			}
		}
		else if (m_IsDeath)
		{
			m_DrawOptions = false;
		}
	}
	else if (!m_PauseAll)
	{
		m_pInventory->ProcessMouseDownEvent(e, playerclass);
	}
}

void UI::CheckIfSaveFile()
{
	std::ifstream InputFile("Resources/LoadGame.txt");
	if (!InputFile)
	{
		std::cerr << "LoadGame: File could not be opened for reading\n";
		return;
	}
	std::string Line;
	std::getline(InputFile, Line);
	std::size_t pos{ Line.find("Game:") };
	std::string Check{ Line.substr(pos + 5) };
	std::cout << Check;
	std::stringstream ssCheck = std::stringstream(Check);
	ssCheck >> std::boolalpha >> m_IsSaveFile;
}

void UI::SetStartLoadFromFile(bool LoadFromFile)
{
	m_StartLoadFromFile = LoadFromFile;
}

bool UI::StartNewGame()
{
	return m_StartNewGame;
}

void UI::SetStartNewGame(bool StartNewGame)
{
	m_StartNewGame = StartNewGame;
}

bool UI::IsGameInitialized()
{
	return !m_Initialize;
}


bool UI::GetChangeVolume()
{
	return m_ChangeVolume;
}

void UI::SetChangeVolume(bool volume)
{
	m_ChangeVolume = volume;
}

int UI::GetVolume()
{
	return int(m_VolumePercent);
}

void UI::SetBoolInitialize(bool initialize)
{
	m_Initialize = initialize;
}

void UI::SetBoolDeath(bool isDeath)
{
	m_IsDeath = isDeath;
}