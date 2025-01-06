#pragma once
#include "SpaceShip.h"
#include "Obstacle.h"
#include "Alien.h"
#include "MysteryShip.h"
class Game
{
public:
	Game();
	~Game();
	void Draw();
	void Update();
	void HandleInput();
	bool run;
	int lives;
	int score;
	int highScore;
	Music music;
private:
	void DeleteInactiveLasers();
	std::vector<Alien> CreateAliens();
	void MoveAliens();
	void MoveDownAliens(int distance);
	void AlienShootLaser();
	SpaceShip spaceship;
	std::vector<Obstacle> obstacles;
	std::vector<Obstacle> CreateObstacles();
	std::vector<Alien> aliens;
	int aliensDirection;
	std::vector<Laser> alienLasers;
	constexpr static float alienLaserShootInterval = 0.35;
	float TimeLastAlienFired;
	MysteryShip mysteryship;
	float MysteryShipSpawnInterval;
	float TimeLastSpawn;
	void CheckForCollisions();
	void GameOver();
	void Reset();
	void InitGame();
	void CheckForHighScore();
	void saveHighScoreToFile(int highscore);
	int loadHighScoreFromFile();
	Sound explosionSound;
};

