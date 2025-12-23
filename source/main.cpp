#include "raylib.h"
#include "raymath.h"
#include <iostream>
#include <string>

int main(){
	std::string playerImageLocation = (std::string)GetWorkingDirectory() + "\\assets\\player.png";
	Image playerImage = LoadImage(playerImageLocation.c_str());
	
	SetTargetFPS(60);
	InitWindow(800, 600, "FlappySquare");

	Texture playerTexture = LoadTextureFromImage(playerImage);
	UnloadImage(playerImage);

	const float FALLSPEED = 9.81f * 40;
	const float JUMPSPEED = -245.0f;
	Vector2 playerSize = { 32, 32 };
	Vector2 playerPosition = { GetScreenWidth() / 2.0f - playerSize.x / 2.0f, GetScreenHeight() / 2.0f - playerSize.y / 2.0f };
	float playerYSpeed = 0;
	float playerRotation = 0;
	const Rectangle sourceRectangle = { 0, 0, playerTexture.width, playerTexture.height };
	Rectangle playerRec = { playerPosition.x, playerPosition.y, playerSize.x, playerSize.y};

	while (!WindowShouldClose()) {

		//logic
		if (IsKeyPressed(KEY_SPACE)) {
			playerYSpeed = JUMPSPEED;
		}

		playerYSpeed += FALLSPEED * GetFrameTime();
		playerPosition.y += playerYSpeed * GetFrameTime();

		//drawing
		BeginDrawing();

		//draw background
		ClearBackground(BLACK);
		
		//draw player
		playerSize = { 32.0f, Lerp(32.0f, 16.0f, Clamp(abs(playerYSpeed) / 250.0f, 0, 1))};
		playerRec = { playerPosition.x, playerPosition.y, playerSize.x, playerSize.y };
		playerRotation = atan2f(playerYSpeed, 200) * (180.0f / PI);
		DrawTexturePro(playerTexture, sourceRectangle, playerRec, {playerSize.x / 2.0f, playerSize.y / 2.0f}, playerRotation, WHITE);
		
		EndDrawing();
	}
	return 0;
}