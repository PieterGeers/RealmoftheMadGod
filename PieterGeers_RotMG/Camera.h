#pragma once

class Camera
{
public:
	Camera(float width, float height);

	void SetBoundaries(const Rectf& boundaries);
	void Draw(const Rectf& toTrack) const;

	Point2f GetPosition(const Rectf& toTrack) const;

private:
	void Clamp(Point2f& bottomLeftPos) const;

	Point2f Track(const Rectf& toTrack) const;

	float m_Width, m_Height;
	Rectf m_Boundaries;
};

