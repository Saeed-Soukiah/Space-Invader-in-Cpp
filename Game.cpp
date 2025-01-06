#include "Game.h"
#include "raylib.h"
#include <fstream>
#include <iostream>

using namespace std;

Game::Game()
{
	music = LoadMusicStream("Sounds/music.ogg");
	explosionSound = LoadSound("Sounds/explosion.ogg");
	PlayMusicStream(music);
	InitGame();
}

Game::~Game()
{
	Alien::UnloadImages();
	UnloadMusicStream(music);
	UnloadSound(explosionSound);
}

void Game::Draw()
{
	spaceship.Draw();
	for (auto& laser : spaceship.lasers) {
		laser.Draw();
	}
	for (auto& Obstacle : obstacles) {
		Obstacle.Draw();
	}
	for (auto& alien : aliens) {
		alien.Draw();
	}
	for (auto& laser : alienLasers) {
		laser.Draw();
	}
	mysteryship.Draw();
}

void Game::Update()
{
	if (run)
	{
		double currentTime = GetTime();
		if (currentTime - TimeLastSpawn > MysteryShipSpawnInterval) {
			mysteryship.Spawn();
			TimeLastSpawn = GetTime();
			MysteryShipSpawnInterval = GetRandomValue(10, 20);
		}
		for (auto& laser : spaceship.lasers) {
			laser.Update();
		}
		MoveAliens();
		AlienShootLaser();

		for (auto& laser : alienLasers) {
			laser.Update();
		}

		DeleteInactiveLasers();
		mysteryship.Update();
		CheckForCollisions();
	}
	else {
		if (IsKeyDown(KEY_ENTER)) {
			Reset();
			InitGame();
		}
	}
}

void Game::HandleInput()
{
	if (run) {
		if (IsKeyDown(KEY_LEFT)) {
			spaceship.MoveLeft();
		}
		else if (IsKeyDown(KEY_RIGHT)) {
			spaceship.MoveRight();
		}
		else if (IsKeyDown(KEY_SPACE)) {
			spaceship.FireLaser();
		}
	}
}

void Game::DeleteInactiveLasers()
{
	for (auto it = spaceship.lasers.begin(); it != spaceship.lasers.end();) {
		if (!it->active) {
			it = spaceship.lasers.erase(it);
		}
		else
		{
			++it;
		}
	}
	for (auto it = alienLasers.begin(); it != alienLasers.end();) {
		if (!it->active) {
			it = alienLasers.erase(it);
		}
		else
		{
			++it;
		}
	}
}

std::vector<Alien> Game::CreateAliens()
{
	std::vector<Alien> aliens;
	for (int row = 0; row < 5; row++) {
		for (int column = 0; column < 11; column++) {
			int alienType;
			if (row == 0) {
				alienType = 3;
			}
			else if (row == 1 || row == 2) {
				alienType = 2;
			}
			else {
				alienType = 1;
			}
			float x = 75 + column * 55;
			float y = 110 + row * 55;
			aliens.push_back(Alien(alienType, { x,y }));
		}
	}
	return aliens;
}

void Game::MoveAliens()
{
	for (auto& alien : aliens) {
		if (alien.position.x + alien.alienImages[alien.type].width > GetScreenWidth() - 25) {
			aliensDirection = -1;
			MoveDownAliens(4);
		}
		if (alien.position.x < 25) {
			aliensDirection = 1;
			MoveDownAliens(4);
		}
		alien.Update(aliensDirection);
	}
}

void Game::MoveDownAliens(int distance)
{
	for (auto& alien : aliens) {
		alien.position.y += distance;
	}
}

void Game::AlienShootLaser()
{
	double CurrentTime = GetTime();
	if (CurrentTime - TimeLastAlienFired >= alienLaserShootInterval && !aliens.empty()) {
		int randomIndex = GetRandomValue(0, aliens.size() - 1);
		Alien& alien = aliens[randomIndex];
		alienLasers.push_back(Laser({ alien.position.x + alien.alienImages[alien.type - 1].width / 2,
										alien.position.y + alien.alienImages[alien.type - 1].height }, 6));
		TimeLastAlienFired = GetTime();
	}

	
}

std::vector<Obstacle> Game::CreateObstacles()
{
	int ObstacleWidth = Obstacle::grid[0].size() * 3;
	float gap = (GetScreenWidth() - (4 * ObstacleWidth)) / 5;
	for (int i = 0; i < 4; i++) {
		float offsetX = (i + 1) * gap + i * ObstacleWidth;
		obstacles.push_back(Obstacle({ offsetX,float(GetScreenHeight() - 200) }));
	}
	return obstacles;
}

void Game::CheckForCollisions()
{
	//spaceship lasers
	for (auto& laser : spaceship.lasers) {
		auto it = aliens.begin();
		while (it != aliens.end()) {
			if (CheckCollisionRecs(it -> GetRect(),laser.GetRect()))
			{
				PlaySound(explosionSound);
				if (it -> type == 1)
				{
					score += 100;
				}
				else if(it -> type == 2)
				{
					score += 200;
				}
				else if(it -> type == 3){
					score += 300;
				}
				CheckForHighScore();

				it = aliens.erase(it);
				laser.active = false;
			}
			else {
				++it;
			}
		}

		for (auto& Obstacle : obstacles) {
			auto it = Obstacle.blocks.begin();
			while (it != Obstacle.blocks.end()) {
				if (CheckCollisionRecs(it->GetRect(), laser.GetRect())) {
					it = Obstacle.blocks.erase(it);
					laser.active = false;
				}
				else {
					++it;
				}
			}
		}

		if (CheckCollisionRecs(mysteryship.GetRect(), laser.GetRect())) {
			mysteryship.alive = false;
			laser.active = false;
			score += 500;
			CheckForHighScore();
			PlaySound(explosionSound);
		}
	}
	//Alien Lasers

	for (auto& laser : alienLasers) {
		if (CheckCollisionRecs(laser.GetRect(), spaceship.GetRect())) {
			laser.active = false;
			lives--;
			if(lives == 0) {
				GameOver();
			}
		}
		for (auto& Obstacle : obstacles) {
			auto it = Obstacle.blocks.begin();
			while (it != Obstacle.blocks.end()) {
				if (CheckCollisionRecs(it->GetRect(), laser.GetRect())) {
					it = Obstacle.blocks.erase(it);
					laser.active = false;
				}
				else {
					++it;
				}
			}
		}
	}

	//Alien Collision With Obstacles
	for (auto& alien : aliens) {
		for (auto& obstacle : obstacles) {
			auto it = obstacle.blocks.begin();
			while (it != obstacle.blocks.end()) {
				if (CheckCollisionRecs(it->GetRect(), alien.GetRect())) {
					it = obstacle.blocks.erase(it);
				}
				else {
					it++;
				}
			}
		}
		if (CheckCollisionRecs(alien.GetRect(), spaceship.GetRect())) {
			GameOver();
		}
	}
}

void Game::GameOver()
{
	run = false;
}

void Game::Reset()
{
	spaceship.Reset();
	aliens.clear();
	alienLasers.clear();
	obstacles.clear();
}

void Game::InitGame()
{
	obstacles = CreateObstacles();
	aliens = CreateAliens();
	aliensDirection = 1;
	TimeLastAlienFired = 0;
	TimeLastSpawn = 0.0;
	MysteryShipSpawnInterval = GetRandomValue(10, 20);
	lives = 3;
	score = 0;
	highScore = loadHighScoreFromFile();
	run = true;
}

void Game::CheckForHighScore()
{
	if (score > highScore) {
		highScore = score;
		saveHighScoreToFile(highScore);
	}
}

void Game::saveHighScoreToFile(int highscore)
{
	ofstream highscoreFile("highscore.txt");
	if (highscoreFile.is_open()) {
		highscoreFile << highScore;
		highscoreFile.close();
	}
	else {
		cerr << "Failed to Save Highscore To File." << endl;
	}
}

int Game::loadHighScoreFromFile()
{
	int loadedHighScore = 0;
	ifstream highscoreFile("highscore.txt");
	if (highscoreFile.is_open()) {
		highscoreFile >> loadedHighScore;
		highscoreFile.close();
	}
	else {
		cerr << "Failed To Load Highscore From File." << endl;
	}
	return loadedHighScore;
}
