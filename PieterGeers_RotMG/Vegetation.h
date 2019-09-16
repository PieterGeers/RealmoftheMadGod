#pragma once
#include <memory>
#include "Texture.h"
#include <fstream>
#include <iomanip>
#include <string>
#include <iostream>

class Vegetation
{
public:
	enum class Plants
	{
		other,
		tree,
		cactus,
		bush
	};

	Vegetation();
	void DrawForeground() const; //player must run behind the cacti and trees  
	void DrawBackground() const; //player can run on top/ over bushes
	void Collision(Rectf avatar, Vector2f& actorVelocity1, Vector2f& actorVelocity2);

private:
	std::shared_ptr<Texture> m_pTree;
	std::shared_ptr<Texture> m_pCactus;
	std::shared_ptr<Texture> m_pBush;

	Plants m_Plants[32][32];
	Point2f m_PlantsPos[32][32];
	Rectf m_Shape[32][32];

	void ReadVegetationFromFile();
};

