#include "stdafx.h"
#include "AniPlayer.h"

AniPlayer::AniPlayer(const std::string& name)
	: GameObject(name)
{
}

void AniPlayer::SetPosition(const sf::Vector2f& pos)
{
	position = pos;
	body.setPosition(position);
}

void AniPlayer::SetRotation(float angle)
{
	rotation = angle;
	body.setRotation(rotation);
}

void AniPlayer::SetScale(const sf::Vector2f& s)
{
	scale = s;
	body.setScale(scale);
}

void AniPlayer::SetOrigin(Origins preset)
{
	originPreset = preset;
	if (originPreset != Origins::Custom)
	{
		Utils::SetOrigin(body, originPreset);
	}
}

void AniPlayer::SetOrigin(const sf::Vector2f& newOrigin)
{
	originPreset = Origins::Custom;
	origin = Utils::SetOrigin(body, originPreset);
}

void AniPlayer::Init()
{
	animator.SetTarget(&body);

	std::string sheetId = "graphics/sprite_sheet.png";
	sf::IntRect coord(0, 0, 120, 120);
	// Idle
	idle.id = "Idle";
	idle.fps = 10;
	idle.loopType = AnimationLoopTypes::Loop;
	for (int i = 0; i < 8; ++i)
	{
		coord.left = i * 120;
		idle.frames.push_back(AnimationFrame(sheetId, coord));
	}
	// Run
	run.id = "Run";
	run.fps = 10;
	run.loopType = AnimationLoopTypes::Loop;

	coord = { 0, 120, 120, 120 };
	for (int i = 0; i < 8; ++i)
	{
		coord.left = i * 120;
		run.frames.push_back(AnimationFrame(sheetId, coord));
	}
	run.frames.push_back(AnimationFrame(sheetId, {0, 240, 120, 120}));

	// Jump
	jump.id = "Jump";
	jump.fps = 10;
	jump.loopType = AnimationLoopTypes::Single;

	coord = { 0, 360, 120, 120 };
	for (int i = 0; i < 7; ++i)
	{
		coord.left = i * 120;
		jump.frames.push_back(AnimationFrame(sheetId, coord));
	}
}

void AniPlayer::Release()
{
}

void AniPlayer::Reset()
{
	animator.Play(&idle);
	SetOrigin(Origins::BC);
}

void AniPlayer::Update(float dt)
{
	animator.Update(dt);

	float h = 0.f;
	if (isGrounded)
	{
		h = InputMgr::GetAxis(Axis::Horizontal);
		velocity.x = h * speed;
	}
	if (isGrounded && InputMgr::GetKeyDown(sf::Keyboard::Space))
	{
		isGrounded = false;
		velocity.y = -500.f;
		animator.Play(&jump);
	}
	if (!isGrounded)
	{
		velocity += gravity * dt;
	}
	position += velocity * dt;
	if (position.y > 0.f)
	{
		velocity.y = 0.f;
		position.y = 0.f;
		isGrounded = true;
	}
	SetPosition(position);
	if (h != 0.f)
	{
		SetScale(h > 0.f ? sf::Vector2f(1.0f, 1.0) : sf::Vector2f(- 1.f, 1.0f));
	}

	// Ani
	if (animator.GetCurrentClipId() == "Idle")
	{
		if (h != 0.f)
		{
			animator.Play(&run);
		}
	}
	else if (animator.GetCurrentClipId() == "Run")
	{
		if (h == 0.f)
		{
			animator.Play(&idle);
		}
	}
	else if (animator.GetCurrentClipId() == "Jump" && isGrounded)
	{
		if (h == 0.f)
		{
			animator.Play(&idle);
		}
		else
		{
			animator.Play(&run);
		}
	}
}

void AniPlayer::Draw(sf::RenderWindow& window)
{
	window.draw(body);
}
