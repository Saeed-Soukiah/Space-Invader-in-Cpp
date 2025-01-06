#include "SpaceShip.h"

SpaceShip::SpaceShip()
{
	Image = LoadTexture("Graphics/spaceship.png");
	Position.x = (GetScreenWidth() - Image.width) /2;
	Position.y = GetScreenHeight() - Image.height - 100;
	LastFireTime = 0.0;
	laserSound = LoadSound("Sounds/laser.ogg");
}

SpaceShip::~SpaceShip()
{
	UnloadTexture(Image);
	UnloadSound(laserSound);
}

void SpaceShip::Draw()
{
	DrawTextureV(Image, Position, WHITE);
}

void SpaceShip::MoveLeft()
{
	if (Position.x < 25) {
		Position.x = 25;
	}
	Position.x -= 7;
}

void SpaceShip::MoveRight()
{
	if (Position.x  > GetScreenWidth()- Image.width -25) {
		Position.x = GetScreenWidth() - Image.width -25;
	}
	Position.x += 7;
}

void SpaceShip::FireLaser()
{
	if (GetTime() - LastFireTime >= 0.32) {
		lasers.push_back(Laser({ Position.x + Image.width / 2 - 2,Position.y }, -6));
		LastFireTime = GetTime();
		PlaySound(laserSound);
	}	
}

Rectangle SpaceShip::GetRect()
{
	return { Position.x,Position.y,float(Image.width),float(Image.height) };
}

void SpaceShip::Reset()
{
	Position.x = (GetScreenWidth() - Image.width) / 2.0f;
	Position.y = GetScreenHeight() - Image.height - 100;
	lasers.clear();
}
