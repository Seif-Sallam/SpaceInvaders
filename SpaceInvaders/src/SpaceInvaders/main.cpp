#include "raylib.h"

#include <vector>

#include <stdio.h>                  // Required for: fopen(), fclose(), fputc(), fwrite(), printf(), fprintf(), funopen()
#include <time.h>                   // Required for: time_t, tm, time(), localtime(), strftime()


#define SCREEN_WIDTH 1200
#define SCREEN_HEIGHT 800

void Log(int msgType, const char *text, va_list args)
{
	char timeStr[64] = { 0 };
	time_t now = time(NULL);
	struct tm *tm_info = localtime(&now);

	strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", tm_info);
	printf("[%s] ", timeStr);

	switch (msgType)
	{
		case LOG_INFO: printf("[INFO] : "); break;
		case LOG_ERROR: printf("[ERROR]: "); break;
		case LOG_WARNING: printf("[WARN] : "); break;
		case LOG_DEBUG: printf("[DEBUG]: "); break;
		default: break;
	}

	vprintf(text, args);
	printf("\n");
}



struct Bullet {
	int x, y;
	bool isDead;

	constexpr static const int WIDTH = 2;
	constexpr static const int HEIGHT = 3;
	constexpr static const int SPEED = 350; // in pixels
};

struct Ship {
	int x, y; // x and y positions
	std::vector<Bullet> bullets;
	bool isDead;

	constexpr static const int WIDTH = 50;
	constexpr static const int HEIGHT = 20;
	constexpr static const int SPEED = 150; // in pixels
};

void MoveBullet(Bullet& self, const float deltaTime);
void MoveShip(Ship& self, const float deltaTime);
void DrawBullet(const Bullet& self);
void DrawShip(const Ship& self);

void ShootBullet(Ship& self);

int main(void)
{
	SetTraceLogCallback(Log);
	SetTraceLogLevel(LOG_ALL);

	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Space Invaders");

	SetTargetFPS(60);

	Ship ship{};
	ship.x = SCREEN_WIDTH / 2;
	ship.y = SCREEN_HEIGHT - 60;

	float deltaTime{};

	while (!WindowShouldClose())
	{
		deltaTime = GetFrameTime();

		MoveShip(ship, deltaTime);
		for (auto& bullet : ship.bullets)
			MoveBullet(bullet, deltaTime);

		auto itr = std::remove_if(ship.bullets.begin(), ship.bullets.end(), [](auto& a) {
			return a.isDead;
		});

		if (itr != std::end(ship.bullets))
			ship.bullets.erase(itr);

		BeginDrawing();
		{
			ClearBackground(BLACK);

			for (auto& bullet : ship.bullets)
				DrawBullet(bullet);
			DrawShip(ship);
		}
		EndDrawing();
	}

	CloseWindow();

	return 0;
}

void MoveBullet(Bullet& self, const float deltaTime)
{
	if (self.isDead)
		return;

	if (self.y <= -Bullet::HEIGHT)
		self.isDead = true;

	self.y -= int(Bullet::SPEED * deltaTime);
}

void MoveShip(Ship& self, const float deltaTime)
{
	if (self.isDead)
		return;

	// Keys
	int dir{};
	if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT))
		dir = -1;
	else if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT))
		dir = +1;

	if (IsKeyDown(KEY_SPACE))
		ShootBullet(self);

	self.x += int(dir * Ship::SPEED * deltaTime);
}

void DrawBullet(const Bullet& self)
{
	DrawRectangle(self.x, self.y, self.WIDTH, self.HEIGHT, Color{255, 0, 0, 255});
}

void DrawShip(const Ship& self)
{
	DrawRectangle(self.x, self.y, self.WIDTH, self.HEIGHT, Color{0, 255, 255, 255});
}

void ShootBullet(Ship& self)
{
	Bullet bullet{};
	bullet.x = self.x;
	bullet.y = self.y;

	self.bullets.push_back(bullet);
}