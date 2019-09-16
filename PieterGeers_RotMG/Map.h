#pragma once
#include <vector>
#include <memory>
#include "Vector2f.h"
#include "SVGParser.h"
#include "Texture.h"

class Vegetation;
class Map
{
public:

	Map();
	
	void DrawMap() const;
	void DrawWater() const;
	void DrawVegetationForeground() const;
	void HandleCollision(Rectf actorshape, Vector2f& actorVelocity1, Vector2f& acterVelocity2) const;

	Rectf getShape() const;

private:
	std::shared_ptr<Vegetation> m_pVegetation;
	std::unique_ptr<Texture> m_pWater;
	std::unique_ptr<Texture> m_pMap;

	std::vector<std::vector<Point2f>> m_Vertices;

	Rectf m_Shape;
};

