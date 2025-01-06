#pragma once
#include "raylib.h"
#include "Laser.h"
#include <vector>
using namespace std;
class SpaceShip
{
public:
	SpaceShip();
	~SpaceShip();
	void Draw();
	void MoveLeft();
	void MoveRight();
	void FireLaser();
	vector<Laser> lasers;
	Rectangle GetRect();
	void Reset();
private:
	Texture2D Image;
	Vector2 Position;
	double LastFireTime;
	Sound laserSound;
};

