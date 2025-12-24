#include "raylib.h"
#include "raymath.h"
#include <iostream>
#include <string>
#include <vector>

class wall {
public :
	wall() {
		position = { 850, (float)GetRandomValue(-425, -25) };
	}
	void Logic(const Vector2& playerPos, int& score, float& scoreAnim) {
		if (playerPos.x >= position.x + 16 && !scored) {
			scored = true;
			score++;
			scoreAnim = 0;
		}
		if (position.x <= -40) {
			active = false;
		}
		position.x -= speed * GetFrameTime();
	}
	void Draw() {
		DrawRectangle(position.x, position.y, 32, 425, WHITE);
		DrawRectangle(position.x, position.y + 600, 32, 425, WHITE);
	}
	bool isActive() {
		return active;
	}
	Rectangle ReturnRec(int nr) {
		if (nr == 1) {
			return { position.x, position.y, 32, 425 };
		}
		else {
			return { position.x, position.y + 600, 32, 425 };
		}
	}
private:
	Vector2 position;
	int speed = 200;
	bool active = true;
	bool scored = false;
};

float easeOutExpo(float x){
	return x == 1 ? 1 : 1 - pow(2, -10 * x);
}

int main(){
	std::string playerImageLocation = (std::string)GetWorkingDirectory() + "\\assets\\player.png";
	std::string fontLocation = (std::string)GetWorkingDirectory() + "\\assets\\RobotoMono-SemiBold.ttf";
	Image playerImage = LoadImage(playerImageLocation.c_str());
	SetRandomSeed(time(0));
	SetTargetFPS(60);
	InitWindow(800, 600, "FlappySquare");

	Texture playerTexture = LoadTextureFromImage(playerImage);
	UnloadImage(playerImage);

	Font robotoFont = LoadFont(fontLocation.c_str());

	const float FALLSPEED = 9.81f * 40;
	const float JUMPSPEED = -245.0f;
	Vector2 playerSize = { 32, 32 };
	Vector2 playerPosition = { GetScreenWidth() / 2.0f - playerSize.x / 2.0f, GetScreenHeight() / 2.0f - playerSize.y / 2.0f };
	float playerYSpeed = 0;
	float playerRotation = 0;
	float timeToSpawnWall = 0;
	int score = 0;
	float scoreTextAnim = 1;
	const Rectangle sourceRectangle = { 0, 0, playerTexture.width, playerTexture.height };
	Rectangle playerRec = { playerPosition.x, playerPosition.y, playerSize.x, playerSize.y};
	std::vector<wall> walls;
	while (!WindowShouldClose()) {

		//logic
		if (IsKeyPressed(KEY_SPACE)) {
			playerYSpeed = JUMPSPEED;
		}

		playerYSpeed += FALLSPEED * GetFrameTime();
		playerPosition.y += playerYSpeed * GetFrameTime();
		if (playerPosition.y <= 16) {
			playerYSpeed = 0;
			playerPosition.y = 16;
		}

		for (wall& eWall : walls) {
			eWall.Logic(playerPosition, score, scoreTextAnim);
		}

		if (scoreTextAnim < 1) {
			scoreTextAnim += GetFrameTime() / 1.5f;
		}
		else {
			scoreTextAnim = 1;
		}

		if (timeToSpawnWall <= 0) {
			timeToSpawnWall = GetRandomValue(2, 3);
			walls.push_back(wall());
		}
		else {
			timeToSpawnWall -= GetFrameTime();
		}

		for (wall& eWall : walls) {
			if (CheckCollisionRecs(playerRec, eWall.ReturnRec(1)) || CheckCollisionRecs(playerRec, eWall.ReturnRec(2))) {
				walls.clear();
				playerSize = { 32, 32 };
				playerPosition = { GetScreenWidth() / 2.0f - playerSize.x / 2.0f, GetScreenHeight() / 2.0f - playerSize.y / 2.0f };
				playerYSpeed = 0;
				playerRotation = 0;
				timeToSpawnWall = 0;
				score = 0;
				break;
			}
		}
		//add death when fall

		while (!walls.empty() && !walls.front().isActive()) {
			walls.erase(walls.begin());
		}

		//drawing
		BeginDrawing();

		//draw background
		ClearBackground(BLACK);
		
		//draw player
		playerSize = { 32.0f, Lerp(32.0f, 16.0f, Clamp(abs(playerYSpeed) / 250.0f, 0, 1))};
		playerRec = { playerPosition.x, playerPosition.y, playerSize.x, playerSize.y };
		playerRotation = atan2f(playerYSpeed, 200) * (180.0f / PI);
		DrawTexturePro(playerTexture, sourceRectangle, playerRec, {playerSize.x / 2.0f, playerSize.y / 2.0f}, playerRotation, WHITE);
		
		//draw walls
		for (wall& eWall : walls) {
			eWall.Draw();
		}
		//draw score
		DrawTextEx(robotoFont, std::to_string(score).c_str(), { GetScreenWidth() / 2.0f - MeasureTextEx(robotoFont, std::to_string(score).c_str(), Lerp(64, 48, easeOutExpo(scoreTextAnim)), 0).x / 2.0f, 80 - MeasureTextEx(robotoFont, std::to_string(score).c_str(), Lerp(64, 48, easeOutExpo(scoreTextAnim)), 0).y / 2.0f }, Lerp(64, 48, easeOutExpo(scoreTextAnim)), 0, WHITE);
		EndDrawing();
	}
	return 0;
}