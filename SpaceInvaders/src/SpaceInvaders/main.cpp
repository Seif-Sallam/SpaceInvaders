#include "raylib.h"

#include <stdio.h>                  // Required for: fopen(), fclose(), fputc(), fwrite(), printf(), fprintf(), funopen()
#include <time.h>                   // Required for: time_t, tm, time(), localtime(), strftime()

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

int main(void)
{
	const int screenWidth = 1200;
	const int screenHeight = 800;

	SetTraceLogCallback(Log);
	SetTraceLogLevel(LOG_ALL);

	InitWindow(screenWidth, screenHeight, "Space Invaders");

	SetTargetFPS(60);

	while (!WindowShouldClose())
	{
		BeginDrawing();
		ClearBackground(RAYWHITE);

		EndDrawing();
	}

	CloseWindow();

	return 0;
}