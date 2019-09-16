#include "stdafx.h"
#include "Game.h"
#include "RotMG.h"

Game::Game( const Window& window ) 
	: m_Window{ window }
	, m_pGame{ std::make_shared<RotMG>(Point2f{640,0 }) }
{
	Initialize( );
}

Game::~Game( )
{
	Cleanup( );
}

void Game::Initialize()
{
	m_pGame->Initialize();
}

void Game::Cleanup( )
{
	m_pGame->CleanUp();
}

void Game::Update( float elapsedSec )
{
	m_pGame->Update(elapsedSec);
}

void Game::Draw( ) const
{
	ClearBackground( );
	m_pGame->Draw();
}

void Game::ProcessKeyDownEvent( const SDL_KeyboardEvent & e )
{
	m_pGame->ProcessKeyDownEvent(e);
}

void Game::ProcessKeyUpEvent( const SDL_KeyboardEvent&)
{
}

void Game::ProcessMouseMotionEvent(const SDL_MouseMotionEvent& e)
{
	m_pGame->ProcessMouseMotionEvent(e);
}

void Game::ProcessMouseDownEvent(const SDL_MouseButtonEvent& e)
{
	m_pGame->ProcessMouseDownEvent(e);
}

void Game::ProcessMouseUpEvent( const SDL_MouseButtonEvent& e )
{
	m_pGame->ProcessMouseUpEvent(e);
}

void Game::ClearBackground( ) const
{
	glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
	glClear( GL_COLOR_BUFFER_BIT );
}
