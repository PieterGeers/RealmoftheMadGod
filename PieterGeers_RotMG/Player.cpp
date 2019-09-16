#include "stdafx.h"
#include "Player.h"
#include "Texture.h"
#include "Bullet.h"
#include "SoundEffect.h"

Player::Player(const Point2f& pos, const PlayerClass& playerclass)
	: m_MidPos{pos}
	, m_MyDirection{Direction::down}
	, m_AvatarState{ AvatarState::none }
	, m_Frame{0}
	, m_AccTime{0}
	, m_MouseHold{false}
	, m_ShootWhenHold{false}
	, m_AccShootTime{0}
	, m_TranslationCamera{0,0}
	, m_NegVelocity{0,0}
	, m_PosVelocity{0,0}
	, m_AvatarXp{}
	, m_AvatarHealth{0}
	, m_AvatarMana{0}
	, m_PlayerClass{playerclass}
	, m_Xp{ "Lvl 1", "Resources/Myriad Pro Regular.ttf", 18,{ 1.f,1.f,1.f,1.f } }
	, m_pPlayerHit{std::make_shared<SoundEffect>("Resources/Sounds/PlayerHit.ogg")}
	, m_pPlayerShoot{std::make_shared<SoundEffect>("Resources/Sounds/PlayerBullet.ogg")}
{
}

Point2f Player::GetMidPos() const
{
	return m_MidPos;
}
void Player::Update(float elapsedSec, const std::shared_ptr<Map>& level)
{
	m_PosVelocity.x = m_Speed*elapsedSec;
	m_PosVelocity.y = m_Speed*elapsedSec;
	m_NegVelocity.x = m_Speed*elapsedSec;
	m_NegVelocity.y = m_Speed*elapsedSec;

	level->HandleCollision(GetShape(), m_PosVelocity, m_NegVelocity);

	m_AvatarHealth.Regenerate(elapsedSec);
	m_AvatarMana.Regenerate(elapsedSec);

	UpdateDirections();

	if (m_AvatarState == AvatarState::running)
	{
		m_AccTime += elapsedSec;
		if (m_AccTime > 0.2f)
		{
			++m_Frame;
			if (m_Frame > 2)
			{
				m_Frame = 0;
			}
			m_AccTime -= 0.2f;
		}
	}
	else if (m_AvatarState == AvatarState::attack)
	{
		m_AccTime += elapsedSec;
		if (m_AccTime > 0.1f)
		{
			++m_Frame;
			if (m_Frame > 4)
			{
				m_Frame = 3;
				if (m_MouseHold == false)
				{
					m_AvatarState = AvatarState::none;
				}
			}
			m_AccTime -= 0.1f;
		}
	}
	if (m_MouseHold == true)
	{
		m_AccShootTime += elapsedSec;
		if (m_AccShootTime >= 0.2f)
		{
			m_ShootWhenHold = true;
			m_AccShootTime -= 0.2f;
		}
	}
	if (m_pBullets.size() > 0)
	{
		for (int i{0}; i <= int(m_pBullets.size()-1); ++i)
		{
			m_pBullets[i]->Update(elapsedSec);
			if (m_pBullets[i]->CheckTravelledDistance() == true)
			{
				m_pBullets.front().reset();
				m_pBullets.pop_front();
			}	
		}
	}
}
void Player::UpdateDirections()
{
	const Uint8 *pStates = SDL_GetKeyboardState(nullptr);
//-----------------------------------------------------------------------
	//multiply keys pushed
//-----------------------------------------------------------------------
	if (pStates[SDL_SCANCODE_W] && (pStates[SDL_SCANCODE_A]))
	{
		if (m_AvatarState != AvatarState::attack)
		{
			m_MyDirection = Direction::left;
			m_AvatarState = AvatarState::running;
		}
		m_MidPos.y += m_PosVelocity.y;
		m_MidPos.x -= m_NegVelocity.x;
	}
	else if (pStates[SDL_SCANCODE_W] && (pStates[SDL_SCANCODE_D]))
	{
		if (m_AvatarState != AvatarState::attack)
		{
			m_MyDirection = Direction::right;
			m_AvatarState = AvatarState::running;
		}
		m_MidPos.y += m_PosVelocity.y;
		m_MidPos.x += m_PosVelocity.x;
	}
	else if (pStates[SDL_SCANCODE_S] && (pStates[SDL_SCANCODE_A]))
	{
		if (m_AvatarState != AvatarState::attack)
		{
			m_MyDirection = Direction::left;
			m_AvatarState = AvatarState::running;
		}
		m_MidPos.y -= m_NegVelocity.y;
		m_MidPos.x -= m_NegVelocity.x;
	}
	else if (pStates[SDL_SCANCODE_S] && (pStates[SDL_SCANCODE_D]))
	{
		if (m_AvatarState != AvatarState::attack)
		{
			m_MyDirection = Direction::right;
			m_AvatarState = AvatarState::running;
		}
		m_MidPos.y -= m_NegVelocity.y;
		m_MidPos.x += m_PosVelocity.x;
	}
//-----------------------------------------------------------------------
	//single key pressed
//-----------------------------------------------------------------------
	else if (pStates[SDL_SCANCODE_W])
	{
		if (m_AvatarState != AvatarState::attack)
		{
			m_MyDirection = Direction::up;
			m_AvatarState = AvatarState::running;
		}
		m_MidPos.y += m_PosVelocity.y;
	}
	else if (pStates[SDL_SCANCODE_S])
	{
		if (m_AvatarState != AvatarState::attack)
		{
			m_MyDirection = Direction::down;
			m_AvatarState = AvatarState::running;
		}
		m_MidPos.y -= m_NegVelocity.y;
	}
	else if (pStates[SDL_SCANCODE_A])
	{
		if (m_AvatarState != AvatarState::attack)
		{
			m_MyDirection = Direction::left;
			m_AvatarState = AvatarState::running;
		}
		m_MidPos.x -= m_NegVelocity.x;
	}
	else if (pStates[SDL_SCANCODE_D])
	{
		if (m_AvatarState != AvatarState::attack)
		{
			m_MyDirection = Direction::right;
			m_AvatarState = AvatarState::running;
		}
		m_MidPos.x += m_PosVelocity.x;
	}
	else 
	{
		if (m_AvatarState != AvatarState::attack)
		{
			m_AvatarState = AvatarState::none;
		}
	}
//-----------------------------------------------------------------------
	//other keys
//-----------------------------------------------------------------------

}
void Player::Draw() const
{
	if (m_pBullets.size() > 0)
	{
		for (int i{ 0 }; i <= int(m_pBullets.size()-1); ++i)
		{
			m_pBullets[i]->Draw();
		}
	}
	switch (m_AvatarState)
	{
	case Player::AvatarState::running:
		switch (m_MyDirection)
		{
		case Player::Direction::right:
			m_pPlayerTexture->Draw({ m_MidPos.x - m_SpriteWidth / 2, m_MidPos.y - m_SpriteHeight / 2, m_SpriteWidth, m_SpriteHeight}
			, { m_Frame * m_SpriteWidth, 1 * m_SpriteHeight, m_SpriteWidth, m_SpriteHeight });
			break;
		case Player::Direction::left:
			m_pPlayerTexture->Draw({ m_MidPos.x - m_SpriteWidth / 2, m_MidPos.y - m_SpriteHeight / 2, m_SpriteWidth, m_SpriteHeight }
			, { m_Frame * m_SpriteWidth, 2 * m_SpriteHeight, m_SpriteWidth, m_SpriteHeight });
			break;
		case Player::Direction::down:
			m_pPlayerTexture->Draw({ m_MidPos.x - m_SpriteWidth / 2, m_MidPos.y - m_SpriteHeight / 2 , m_SpriteWidth, m_SpriteHeight }
			, { m_Frame * m_SpriteWidth, 3 * m_SpriteHeight, m_SpriteWidth, m_SpriteHeight });
			break;
		case Player::Direction::up:
			m_pPlayerTexture->Draw({ m_MidPos.x - m_SpriteWidth / 2, m_MidPos.y - m_SpriteHeight / 2 , m_SpriteWidth, m_SpriteHeight }
			, { m_Frame * m_SpriteWidth, 4 * m_SpriteHeight, m_SpriteWidth, m_SpriteHeight });
			break;
		}
		break;
	case Player::AvatarState::special:
		switch (m_MyDirection)
		{
		case Player::Direction::right:
			m_pPlayerTexture->Draw({ m_MidPos.x - m_SpriteWidth / 2, m_MidPos.y - m_SpriteHeight / 2 , m_SpriteWidth, m_SpriteHeight }
			, { 0, 1 * m_SpriteHeight, m_SpriteWidth, m_SpriteHeight });
			break;
		case Player::Direction::left:
			m_pPlayerTexture->Draw({ m_MidPos.x - m_SpriteWidth / 2, m_MidPos.y - m_SpriteHeight / 2 , m_SpriteWidth, m_SpriteHeight }
			, { 0, 2 * m_SpriteHeight, m_SpriteWidth, m_SpriteHeight });
			break;
		case Player::Direction::down:
			m_pPlayerTexture->Draw({ m_MidPos.x - m_SpriteWidth / 2, m_MidPos.y - m_SpriteHeight / 2 , m_SpriteWidth, m_SpriteHeight }
			, { 0, 3 * m_SpriteHeight, m_SpriteWidth, m_SpriteHeight });
			break;
		case Player::Direction::up:
			m_pPlayerTexture->Draw({ m_MidPos.x - m_SpriteWidth / 2, m_MidPos.y - m_SpriteHeight / 2 , m_SpriteWidth, m_SpriteHeight }
			, { 0, 4 * m_SpriteHeight, m_SpriteWidth, m_SpriteHeight });
			break;
		}
		break;
	case Player::AvatarState::attack:
		switch (m_MyDirection)
		{
		case Player::Direction::right:
			m_pPlayerTexture->Draw({ m_MidPos.x - m_SpriteWidth / 2, m_MidPos.y - m_SpriteHeight / 2 , m_SpriteWidth, m_SpriteHeight }
			, { m_Frame * m_SpriteWidth, 1 * m_SpriteHeight, m_SpriteWidth, m_SpriteHeight });
			break;
		case Player::Direction::left:
			m_pPlayerTexture->Draw({ m_MidPos.x - m_SpriteWidth / 2, m_MidPos.y - m_SpriteHeight / 2 , m_SpriteWidth, m_SpriteHeight }
			, { m_Frame * m_SpriteWidth, 2 * m_SpriteHeight, m_SpriteWidth, m_SpriteHeight });
			break;
		case Player::Direction::down:
			m_pPlayerTexture->Draw({ m_MidPos.x - m_SpriteWidth / 2, m_MidPos.y - m_SpriteHeight / 2 , m_SpriteWidth, m_SpriteHeight }
			, { m_Frame * m_SpriteWidth, 3 * m_SpriteHeight, m_SpriteWidth, m_SpriteHeight });
			break;
		case Player::Direction::up:
			m_pPlayerTexture->Draw({ m_MidPos.x - m_SpriteWidth / 2, m_MidPos.y - m_SpriteHeight / 2, m_SpriteWidth, m_SpriteHeight }
			, { m_Frame * m_SpriteWidth, 4 * m_SpriteHeight, m_SpriteWidth, m_SpriteHeight });
			break;
		}
		break;
	case Player::AvatarState::none:
		switch (m_MyDirection)
		{
		case Player::Direction::right:
			m_pPlayerTexture->Draw({ m_MidPos.x - m_SpriteWidth / 2, m_MidPos.y - m_SpriteHeight / 2 , m_SpriteWidth, m_SpriteHeight }
			, { 0, 1 * m_SpriteHeight, m_SpriteWidth, m_SpriteHeight });
			break;
		case Player::Direction::left:
			m_pPlayerTexture->Draw({ m_MidPos.x - m_SpriteWidth / 2, m_MidPos.y - m_SpriteHeight / 2 , m_SpriteWidth, m_SpriteHeight }
			, { 0, 2 * m_SpriteHeight, m_SpriteWidth, m_SpriteHeight });
			break;
		case Player::Direction::down:
			m_pPlayerTexture->Draw({ m_MidPos.x - m_SpriteWidth / 2, m_MidPos.y - m_SpriteHeight / 2, m_SpriteWidth, m_SpriteHeight }
			, { 0, 3 * m_SpriteHeight, m_SpriteWidth, m_SpriteHeight });
			break;
		case Player::Direction::up:
			m_pPlayerTexture->Draw({ m_MidPos.x - m_SpriteWidth / 2, m_MidPos.y - m_SpriteHeight / 2 , m_SpriteWidth, m_SpriteHeight }
			, { 0, 4 * m_SpriteHeight, m_SpriteWidth, m_SpriteHeight });
			break;
		}
		break;
	}
	m_AvatarHealth.Draw({ GetShape() });
}
void Player::ProcessMouseDownEvent(const SDL_MouseButtonEvent& e)
{
	if (e.button == SDL_BUTTON_LEFT)
	{
		switch (m_PlayerClass)
		{
		case Player::PlayerClass::wizard:
			m_pBullets.push_back(std::make_shared<Bullet>(m_MidPos, Bullet::CharacterBullet::wizard));
			break;
		case Player::PlayerClass::knight:
			m_pBullets.push_back(std::make_shared<Bullet>(m_MidPos, Bullet::CharacterBullet::knight));
			break;
		case Player::PlayerClass::archer:
			m_pBullets.push_back(std::make_shared<Bullet>(m_MidPos, Bullet::CharacterBullet::archer));
			break;
		}
		CheckShootDirection({ m_TranslationCamera.x + float(e.x), m_TranslationCamera.y + float(640 - e.y) });
		m_AvatarState = AvatarState::attack;
		m_MouseHold = true;
	}
}
void Player::ProcessMouseUpEvent(const SDL_MouseButtonEvent&)
{
	m_MouseHold = false;
	m_AccShootTime = 0;
}
void Player::ProcessMouseMotionEvent(const SDL_MouseMotionEvent& e)
{
	if (m_MouseHold == true && m_ShootWhenHold == true)
	{
		switch (m_PlayerClass)
		{
		case Player::PlayerClass::wizard:
			m_pBullets.push_back(std::make_shared<Bullet>(m_MidPos, Bullet::CharacterBullet::wizard));
			break;
		case Player::PlayerClass::knight:
			m_pBullets.push_back(std::make_shared<Bullet>(m_MidPos, Bullet::CharacterBullet::knight));
			break;
		case Player::PlayerClass::archer:
			m_pBullets.push_back(std::make_shared<Bullet>(m_MidPos, Bullet::CharacterBullet::archer));
			break;
		}
		CheckShootDirection({m_TranslationCamera.x + float(e.x), m_TranslationCamera.y + float(640 - e.y) });
		m_AvatarState = AvatarState::attack;
		m_ShootWhenHold = false;
		
	}
}
void Player::ProcessKeyDownEvent(const SDL_KeyboardEvent & e)
{
	if (e.keysym.sym == SDLK_SPACE)
	{
		DoSpecialAttack();
	}
}
void Player::CheckShootDirection(const Point2f& toPos)
{
	Vector2f xAs{ 1,0 };
	Vector2f shootVector{ {m_MidPos}, {toPos} };

	float angle{ xAs.AngleWith(shootVector) };
	m_pBullets[m_pBullets.size()-1]->Shoot(angle);
	
	m_pPlayerShoot->Play(0);

	if (angle > (-M_PI / 4) && angle < (M_PI / 4))
	{
		m_MyDirection = Direction::right;
	}
	else if (angle > (M_PI / 4) && angle < (3*M_PI / 4))
	{
		m_MyDirection = Direction::up;
	}
	else if (angle < (-M_PI / 4) && angle > (3 * -M_PI / 4))
	{
		m_MyDirection = Direction::down;
	}
	else if (angle > (3*M_PI/4) || angle < (3 * -M_PI / 4))
	{
		m_MyDirection = Direction::left;
	}
}
Rectf Player::GetShape() const 
{
	return Rectf{ m_MidPos.x - m_SpriteWidth /4, m_MidPos.y - m_SpriteHeight / 2, m_SpriteWidth / 2, m_SpriteHeight};
}
void Player::SetTranslationCamera(const Point2f& cameraLocation)
{
	m_TranslationCamera = cameraLocation;
}
std::deque<std::shared_ptr<Bullet>> Player::GetBullets()
{
	return m_pBullets;
}
void Player::Hit(int damage)
{
	m_AvatarHealth.TakeDamage(damage);
	m_pPlayerHit->Play(0);
}
Health Player::GetHealth() const
{
	return m_AvatarHealth;
}
Mana Player::GetMana() const
{
	return m_AvatarMana;
}
Xp Player::GetXp() const
{
	return m_AvatarXp;
}
void Player::GiveXp()
{
	if (m_AvatarXp.AddXp(m_AvatarHealth.GetMax() / 10))
	{
		m_AvatarHealth.SetMaxHP();
		m_Xp = Texture( "Lvl " + std::to_string(m_AvatarXp.GetLevel()),  "Resources/Myriad Pro Regular.ttf", 18,{ 1.f,1.f,1.f,1.f } );
	}
}
void Player::DoSpecialAttack()
{
	if (m_AvatarMana.UseMana())
	{
		for (int i{}; i < 18; ++i)
		{
			switch (m_PlayerClass)
			{
			case Player::PlayerClass::wizard:
				m_pBullets.push_back(std::make_shared<Bullet>(m_MidPos, Bullet::CharacterBullet::wizard));
				break;
			case Player::PlayerClass::knight:
				m_pBullets.push_back(std::make_shared<Bullet>(m_MidPos, Bullet::CharacterBullet::knight));
				break;
			case Player::PlayerClass::archer:
				m_pBullets.push_back(std::make_shared<Bullet>(m_MidPos, Bullet::CharacterBullet::archer));
				break;
			}
			m_pBullets[m_pBullets.size() - 1]->Shoot(float(((i * 20)*M_PI) / 180));
		}
	}

}
void Player::DrawXp() const
{
	m_Xp.Draw({ 640.f + 6.f, 349.f }, { 0,0, m_Xp.GetWidth(), m_Xp.GetHeight() });
}

bool Player::UseHPPotion()
{
	if (m_AvatarHealth.Get() < m_AvatarHealth.GetMax())
	{
		if (m_AvatarHealth.Get() < m_AvatarHealth.GetMax() - 100)
		{
			m_AvatarHealth.Set(m_AvatarHealth.Get() + 100);
			return true;
		}
		else
		{
			m_AvatarHealth.Set(m_AvatarHealth.GetMax());
			return true;
		}
	}
	return false;
}

bool Player::UseManaPotion()
{
	if (m_AvatarMana.Get() < m_AvatarMana.GetMax())
	{
		if (m_AvatarMana.Get() < m_AvatarMana.GetMax() - 100)
		{
			m_AvatarMana.Set(m_AvatarMana.Get() + 100);
			return true;
		}
		else
		{
			m_AvatarMana.Set(m_AvatarMana.GetMax());
			return true;
		}
	}
	return false;
}

void Player::SetClass(const PlayerClass& player)
{
	m_PlayerClass = player;
	switch (m_PlayerClass)
	{
	case Player::PlayerClass::wizard:
		m_AvatarHealth.Initialize(100);
		m_AvatarMana = 100;
		m_pPlayerTexture = std::make_unique<Texture>("Resources/Characters/PlayerWizard.png");
		break;
	case Player::PlayerClass::knight:
		m_AvatarHealth.Initialize(200);
		m_AvatarMana = 100;
		m_pPlayerTexture = std::make_unique<Texture>("Resources/Characters/PlayerKnight.png");
		break;
	case Player::PlayerClass::archer:
		m_AvatarHealth.Initialize(130);
		m_AvatarMana = 100;
		m_pPlayerTexture = std::make_unique<Texture>("Resources/Characters/PlayerArcher.png");
		break;
	}
}

int Player::GetClass()
{
	return int(m_PlayerClass);
}

void Player::SetLevel(int level)
{
	m_AvatarXp.SetLevel(level);
	m_AvatarHealth.SetMaxHp(level);
	m_AvatarMana.SetMaxMana(level);
	m_Xp = Texture("Lvl " + std::to_string(level), "Resources/Myriad Pro Regular.ttf", 18, { 1.f,1.f,1.f,1.f });

}

void Player::SetXp(int xp)
{
	m_AvatarXp.SetXp(xp);
}

void Player::SetVolume(int volume)
{
	m_AvatarMana.SetVolume(volume);
	m_AvatarXp.SetVolume(volume);
	m_pPlayerHit->SetVolume(volume);
	m_pPlayerShoot->SetVolume(volume);
}

int Player::GetVolume()
{
	return m_pPlayerHit->GetVolume();
}

bool Player::IsDeath()
{
	if (m_AvatarHealth.Get() == 0)
	{
		return true;
	}
	return false;
}