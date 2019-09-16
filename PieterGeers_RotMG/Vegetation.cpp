#include "stdafx.h"
#include "Vegetation.h"


Vegetation::Vegetation()
	: m_pBush{std::make_shared<Texture>("Resources/Map/bush.png")}
	, m_pCactus{std::make_shared<Texture>("Resources/Map/cactus.png")}
	, m_pTree{std::make_shared<Texture>("Resources/Map/tree.png")}
{
	for (int i{}; i < 32; ++i)
	{
		for (int j{}; j < 32; ++j)
		{
			m_PlantsPos[j][i] = { float(j * 64), float(i * 64) };
		}
	}
	ReadVegetationFromFile();
}

void Vegetation::DrawForeground() const
{
	for (int i{}; i < 32; ++i)
	{
		for (int j{}; j < 32; ++j)
		{
			if (m_Plants[j][i] == Plants::tree)
			{
				m_pTree->Draw({ m_PlantsPos[j][i].x - 32.f, m_PlantsPos[j][i].y , m_pTree->GetWidth() * 2.f, m_pTree->GetHeight() * 2.f }, 
					{ 0,0,m_pTree->GetWidth(), m_pTree->GetHeight() });
			}
			else if (m_Plants[j][i] == Plants::cactus)
			{
				m_pCactus->Draw({ m_PlantsPos[j][i].x - 32.f, m_PlantsPos[j][i].y , m_pCactus->GetWidth() * 2.f, m_pCactus->GetHeight() * 2.f },
				{ 0,0,m_pCactus->GetWidth(), m_pCactus->GetHeight() });
			}
		}
	}
}

void Vegetation::DrawBackground() const
{
	for (int i{}; i < 32; ++i)
	{
		for (int j{}; j < 32; ++j)
		{
			if (m_Plants[j][i] == Plants::bush)
			{
				m_pBush->Draw({ m_PlantsPos[j][i].x, m_PlantsPos[j][i].y , m_pBush->GetWidth(), m_pBush->GetHeight() },
				{ 0,0,m_pBush->GetWidth(), m_pBush->GetHeight() });
			}
		}
	}
}

void Vegetation::Collision(Rectf avatar, Vector2f& actorVelocity1, Vector2f& actorVelocity2)
{
	for (int i{}; i < 32; ++i)
	{
		for (int j{}; j < 32; ++j)
		{
			if (m_Plants[j][i] != Plants::other && m_Plants[j][i] != Plants::bush)
			{
				if (utils::IsOverlapping({ avatar.left+1, avatar.bottom }, { avatar.left + avatar.width-1, avatar.bottom }, m_Shape[j][i]))
				{
					actorVelocity2.y *= 0;
				}
				if (utils::IsOverlapping({ avatar.left + avatar.width, avatar.bottom +1}, { avatar.left + avatar.width, avatar.bottom + avatar.height -1}, m_Shape[j][i]))
				{
					actorVelocity1.x *= 0;
				}
				if (utils::IsOverlapping({ avatar.left +1, avatar.bottom + avatar.height }, { avatar.left + avatar.width-1, avatar.bottom + avatar.height}, m_Shape[j][i]))
				{
					actorVelocity1.y *= 0;
				}
				if (utils::IsOverlapping({ avatar.left, avatar.bottom +1}, { avatar.left, avatar.bottom + avatar.height -1}, m_Shape[j][i]))
				{
					actorVelocity2.x *= 0;
				}
			}
		}
	}
}


void Vegetation::ReadVegetationFromFile()
{
	std::ifstream in{ "Resources/Map/Vegetation.txt" };
	if (in)
	{
		int i{};
		int j{};

		std::string str;
		while (std::getline(in, str, ','))
		{
			if (str == "x" || str == "0" || str == "i")
			{
				m_Plants[i][j] = Plants::other;
				m_Shape[i][j] = { 0,0,0,0 };
				++i;
			}
			else if (str == "t")
			{
				m_Plants[i][j] = Plants::tree;
				m_Shape[i][j] = { m_PlantsPos[i][j].x + 12, m_PlantsPos[i][j].y+20, 40 ,24 };
				++i;
			}
			else if (str == "c")
			{
				m_Plants[i][j] = Plants::cactus;
				m_Shape[i][j] = { m_PlantsPos[i][j].x + 18, m_PlantsPos[i][j].y + 20, 28 ,24 };
				++i;
			}
			else if (str == "b")
			{
				m_Plants[i][j] = Plants::bush;
				m_Shape[i][j] = { 0,0,0,0};
				++i;
			}
			if (i == 31)
			{
				++j;
				i = 0;
			}
		}
	}
}