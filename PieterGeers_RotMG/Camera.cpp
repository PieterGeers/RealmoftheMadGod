#include "stdafx.h"
#include "Camera.h"


Camera::Camera(float width, float height)
	: m_Width{width}
	, m_Height{height}
	, m_Boundaries{0,0,width, height}
{
}

void Camera::SetBoundaries(const Rectf& boundaries)
{
	m_Boundaries = boundaries;
}

Point2f Camera::Track(const Rectf& toTrack) const
{
	return Point2f{toTrack.left + toTrack.width/2 - m_Width/2, toTrack.bottom + toTrack.height/2 - m_Height/2};
}

void Camera::Clamp(Point2f& bottomLeftPos) const
{
	if (bottomLeftPos.x < 0)
	{
		bottomLeftPos.x = 0;
	}
	if (bottomLeftPos.y < 0)
	{
		bottomLeftPos.y = 0;
	}
	if (bottomLeftPos.x + m_Width > m_Boundaries.width)
	{
		bottomLeftPos.x = m_Boundaries.width - m_Width;
	}
	if (bottomLeftPos.y + m_Height > m_Boundaries.height)
	{
		bottomLeftPos.y = m_Boundaries.height - m_Height;
	}
}

void Camera::Draw(const Rectf& toTrack) const 
{
	Point2f track{ Track(toTrack) };
	Clamp(track);
	utils::SetColor({ 1,0,0,1 });
	utils::DrawRect({ track.x, track.y }, m_Width, m_Height, 5.f);
}

Point2f Camera::GetPosition(const Rectf& toTrack) const
{
	Point2f track{ Track(toTrack) };
	Clamp(track);
	return track;
}