#pragma once
#include <memory>
#include <string>
#include <fstream>

#include "Camera.h"

class Map;
class Character;
class UI;

class RotMG
{
public:
	RotMG(const Point2f& pos);
	void Initialize();
	void CleanUp();
	void Draw() const;
	void Update(float elapsedSec);

	void ProcessMouseMotionEvent(const SDL_MouseMotionEvent& e);
	void ProcessMouseDownEvent(const SDL_MouseButtonEvent& e);
	void ProcessMouseUpEvent(const SDL_MouseButtonEvent& e);
	void ProcessKeyDownEvent(const SDL_KeyboardEvent& e);


private:
	void LoadFromFile();
	void WriteToFile();

	std::shared_ptr<Map> m_pMap;
	std::shared_ptr<UI> m_pUI;
	std::shared_ptr<Character> m_pCharacter;

	Camera m_Camera;

	bool m_InitializePlayer;
};

