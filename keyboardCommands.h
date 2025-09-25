#pragma once

#include "windows.h"
#include "iostream"


using namespace std;

void SetCursorPosition(int x, int y);

void GetConsoleSize(int& width, int& height);

void ShowConsoleCursor(bool showFlag);

void SetColor(int text, int background);

void Draw(int x, int y, string text);

void drawConsoleFrame(int width, int height, int textColor, int backgroundColor);

enum Colors {
	BLACK = 0,
	DARK_BLUE = 1,
	DARK_GREEN = 2,
	DARK_CYAN = 3,
	DARK_RED = 4,
	DARK_MAGENTA = 5,
	DARK_YELLOW = 6,
	GREY = 7,
	DARK_GREY = 8,
	BLUE = 9,
	GREEN = 10,
	CYAN = 11,
	RED = 12,
	MAGENTA = 13,
	YELLOW = 14,
	WHITE = 15
};
