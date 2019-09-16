#include "stdafx.h"
#include "RotMG.h"
#include "Map.h"
#include "UI.h"
#include "Character.h"


RotMG::RotMG(const Point2f& pos)
	: m_pMap{std::make_shared<Map>()}
	, m_pUI{std::make_shared<UI>(pos)}
	, m_pCharacter{std::make_shared<Character>()}
	, m_Camera{640,640}
	, m_InitializePlayer{true}
{
}

void RotMG::Initialize()
{
	m_Camera.SetBoundaries(m_pMap->getShape());
}

void RotMG::CleanUp()
{
	WriteToFile();
}

void RotMG::Draw() const
{
	if (!m_pUI->IsGamePause())
	{
		m_pMap->DrawWater();
		glPushMatrix();
		glTranslatef(-m_Camera.GetPosition(m_pCharacter->GetPlayer()->GetShape()).x, -m_Camera.GetPosition(m_pCharacter->GetPlayer()->GetShape()).y, 0);
		m_pMap->DrawMap();
		m_pCharacter->Draw();
		m_pMap->DrawVegetationForeground();
		glPopMatrix();
	}
	m_pUI->Draw(m_pCharacter->GetPlayer()->GetMidPos(), m_pCharacter->GetPlayer()->GetHealth(), m_pCharacter->GetPlayer()->GetMana(), m_pCharacter->GetPlayer()->GetXp());
	if (!m_pUI->IsGamePause())
	{
		m_pCharacter->GetPlayer()->DrawXp();
	}
}

void RotMG::Update(float elapsedSec)
{
	if (m_pUI->GetChangeVolume())
	{
		m_pCharacter->SetVolume(m_pUI->GetVolume());
		m_pUI->SetChangeVolume(false);
	}
	if (!m_pUI->IsGamePause())
	{
		if (m_pUI->StartNewGame())
		{
			m_pCharacter->GetPlayer()->SetClass(m_pUI->SetPlayerClass());
			m_pUI->GetInventory()->InitializeBasicItems(m_pUI->SetPlayerClass());
			m_pUI->SetStartNewGame(false);
		}
		if (m_pUI->StartLoadFromFile())
		{
			LoadFromFile();
			m_pUI->SetStartLoadFromFile(false);
		}
		m_pCharacter->Update(elapsedSec, m_pMap, m_pUI);
		m_pCharacter->GetPlayer()->SetTranslationCamera(m_Camera.GetPosition(m_pCharacter->GetPlayer()->GetShape()));
	}
}

void RotMG::ProcessMouseMotionEvent(const SDL_MouseMotionEvent& e)
{
	if (!m_pUI->IsGamePause())
	{
		m_pCharacter->GetPlayer()->ProcessMouseMotionEvent(e);
	}
}

void RotMG::ProcessMouseDownEvent(const SDL_MouseButtonEvent& e)
{
	if (!m_pUI->IsGamePause())
	{
		m_pCharacter->GetPlayer()->ProcessMouseDownEvent(e);
	}
	m_pUI->ProcessMouseDownEvent(e, Player::PlayerClass(m_pCharacter->GetPlayer()->GetClass()));
}

void RotMG::ProcessMouseUpEvent(const SDL_MouseButtonEvent& e)
{
	if (!m_pUI->IsGamePause())
	{
		m_pCharacter->GetPlayer()->ProcessMouseUpEvent(e);
	}
}

void RotMG::ProcessKeyDownEvent(const SDL_KeyboardEvent& e)
{
	if (!m_pUI->IsGamePause())
	{
		m_pCharacter->ProcessKeyDownEvent(e, m_pUI->GetInventory());
	}
	if (e.keysym.sym == SDLK_ESCAPE && m_pUI->IsGameInitialized())
	{
		m_pUI->SetGamePause(!m_pUI->IsGamePause());
	}
}

void RotMG::WriteToFile()
{
	std::ofstream OutputFile("Resources/LoadGame.txt");
	if (!OutputFile)
	{
		std::cerr << "SaveGame: File could not be opened for reading\n";
		return;
	}
	if ((m_pCharacter->GetPlayer()->GetXp().GetXp() > 0 || m_pCharacter->GetPlayer()->GetXp().GetLevel() > 1) && m_pCharacter->GetIsDeath() == false)
	{
		OutputFile << "Game:true\nClass:" << m_pCharacter->GetPlayer()->GetClass() << 
			"\nLevel:" << m_pCharacter->GetPlayer()->GetXp().GetLevel() << 
			"\nCurrentXp:" << m_pCharacter->GetPlayer()->GetXp().GetXp() <<  std::endl;
		OutputFile << "Heal:" << m_pUI->GetInventory()->GetAmountHealing() << "\nMana:" << m_pUI->GetInventory()->GetAmountMana() << std::endl;
		for (unsigned i{}; i != m_pUI->GetInventory()->GetItems().size(); ++i)
		{
			OutputFile << "Item" << i << ":" << int(m_pUI->GetInventory()->GetItems()[i].GetEquipment()) << std::endl;
		}
	}
	else
	{
		OutputFile << "Game:false";
	}
	
}


void RotMG::LoadFromFile()
{
	std::ifstream OutputFile("Resources/LoadGame.txt");
	if (!OutputFile)
	{
		std::cerr << "SaveGame: File could not be opened for reading\n";
		return;
	}

	std::string Line;
	int i{ 0 };
	while (std::getline(OutputFile, Line))
	{
		std::size_t Pos{ Line.find(':') };
		std::size_t Level{Line.find("Level")};
		std::size_t CurrentXp{Line.find("CurrentXp")};
		std::size_t Class{Line.find("Class")};
		std::size_t Heal{Line.find("Heal")};
		std::size_t Mana{Line.find("Mana")};
		std::size_t Item{ Line.find("Item") };

		if (Level != Line.npos)
		{
			std::string sLevel{ Line.substr(Pos + 1) };
			m_pCharacter->GetPlayer()->SetLevel(std::stoi(sLevel));
		}
		else if (CurrentXp != Line.npos)
		{
			std::string sCurrentXp{ Line.substr(Pos + 1) };
			m_pCharacter->GetPlayer()->SetXp(std::stoi(sCurrentXp));
		}
		else if (Class != Line.npos)
		{
			std::string sClass{ Line.substr(Pos + 1) };
			m_pCharacter->GetPlayer()->SetClass(Player::PlayerClass(std::stoi(sClass)));
		}
		else if (Heal != Line.npos)
		{
			std::string sHeal{ Line.substr(Pos + 1) };
			m_pUI->GetInventory()->SetHealing(stoi(sHeal));
		}
		else if (Mana != Line.npos)
		{
			std::string sMana{ Line.substr(Pos + 1) };
			m_pUI->GetInventory()->SetMana(stoi(sMana));
		}
		else if (Item != Line.npos)
		{
			std::string sItem{ Line.substr(Pos + 1) };
			m_pUI->GetInventory()->SetItems(i, Items(Items::Equipment(std::stoi(sItem))));
			++i;
		}
	}
}