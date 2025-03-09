/*
    Name: Saeed Soukiah
    Date: 12/11/2024
    Author: Saeed Soukiah
*/

#include "raylib.h"
#include "Game.h"
#include <string>

using namespace std;

string FormatWithLeadingZeros(int number, int width) {
    string numberText = to_string(number);
    int leadingZeros = width - numberText.length();
    return numberText = string(leadingZeros, '0') + numberText;
}

int main()
{
    Color grey = { 29,29,27,255 };
    Color yellow = { 243,216,63,255 };
    int offset = 50;
    int WindowWidth = 750;
    int WindowHeight = 700;

    InitWindow(WindowWidth + offset, WindowHeight + 2 * offset, "Space Invader");
    InitAudioDevice();

    Font font = LoadFontEx("Font/monogram.ttf", 64, 0, 0);
    Texture2D spaceshipImage = LoadTexture("Graphics/spaceship.png");

    SetTargetFPS(60);

    Game game;

    while (!WindowShouldClose()) {
        UpdateMusicStream(game.music);
        // 1. Event Handling
        game.HandleInput();
        
        // 2. Updating Positions
        game.Update();
        // 3. Drawing Objects
        BeginDrawing();
        ClearBackground(grey);
        DrawRectangleRoundedLinesEx({ 10, 10, 780, 780 }, 0.18f, 20, 2, yellow);
        DrawLineEx({ 25,730 }, { 775,730 }, 3, yellow);
        
        if (game.run)
        {
            DrawTextEx(font, "Level 01", { 570,740 }, 34, 2, yellow);
        }
        else {
            DrawTextEx(font, "GAME OVER", { 570,740 }, 34, 2, yellow);
        }

        float x = 50.0;

        for (int i = 0; i < game.lives; i++)
        {
            DrawTextureV(spaceshipImage, { x,745 }, WHITE);
            x += 50;
        }

        DrawTextEx(font, "SCORE", { 50,15 }, 34, 2, yellow);

        string scoreText = FormatWithLeadingZeros(game.score, 5);
        DrawTextEx(font, scoreText.c_str(), { 50,40 }, 34, 2, yellow);

        DrawTextEx(font, "HIGH SCORE", { 570,15 }, 34, 2, yellow);
        string highscoreText = FormatWithLeadingZeros(game.highScore, 5);
        DrawTextEx(font, highscoreText.c_str(), { 655,40 }, 34, 2, yellow);

        game.Draw();
        EndDrawing();
    }

    

    CloseWindow();
    CloseAudioDevice();
}
