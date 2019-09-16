#include "stdafx.h"
#include "Map.h"
#include "Vegetation.h"

Map::Map()
	: m_pMap{ std::make_unique<Texture>("Resources/Map/map.png") }
	, m_pWater{std::make_unique<Texture>("Resources/Map/water.png")}
	, m_pVegetation{std::make_shared<Vegetation>()}
{
	m_Shape = { 0,0, m_pMap->GetWidth(), m_pMap->GetHeight() };
	SVGParser::GetVerticesFromSvgFile("Resources/Map/map.svg", m_Vertices);
}

void Map::HandleCollision(Rectf actorShape, Vector2f& actorVelocity1, Vector2f& actorVelocity2) const
{
	utils::HitInfo CollisionLeft, CollisionRight, CollisionTop, CollisionBottom;

	bool IsCollisionLeft, IsCollisionRight, IsCollisionTop, IsCollisionBottom;

	Point2f pLeft{ actorShape.left, actorShape.bottom + actorShape.height / 2 }; 
	Point2f pRight{ actorShape.left + actorShape.width, actorShape.bottom + actorShape.height / 2 }; 
	Point2f pTop{ actorShape.left + actorShape.width / 2, actorShape.bottom + actorShape.height }; 
	Point2f pBottom{ actorShape.left + actorShape.width / 2, actorShape.bottom }; 
	Point2f pMidMid{ actorShape.left + actorShape.width / 2 , actorShape.bottom + actorShape.height / 2 }; 

	IsCollisionLeft = utils::Raycast(m_Vertices[0], pMidMid, pLeft, CollisionLeft);
	IsCollisionBottom = utils::Raycast(m_Vertices[0], pMidMid, pBottom, CollisionBottom);
	IsCollisionRight = utils::Raycast(m_Vertices[0], pMidMid, pRight, CollisionRight);
	IsCollisionTop = utils::Raycast(m_Vertices[0], pMidMid, pTop, CollisionTop);

	if (IsCollisionBottom  )
	{
		actorVelocity2.y *= 0.f;	
	}
	if (IsCollisionLeft )
	{
		actorVelocity2.x *= 0.f;
	}
	if (IsCollisionTop )
	{
		actorVelocity1.y *= 0.f;
	}
	if (IsCollisionRight )
	{
		actorVelocity1.x *= 0.f;
	}
	m_pVegetation->Collision(actorShape, actorVelocity1, actorVelocity2);
}

void Map::DrawMap() const
{
	m_pMap->Draw({ 0,0 }, { 0,0,m_pMap->GetWidth(), m_pMap->GetHeight() });
	m_pVegetation->DrawBackground();
}

void Map::DrawWater() const
{
	m_pWater->Draw({ 0, 0}, { 0,0,m_pWater->GetWidth(), m_pWater->GetHeight() });
	
}

void Map::DrawVegetationForeground() const
{
	m_pVegetation->DrawForeground();
}

Rectf Map::getShape() const
{
	return m_Shape;
}

