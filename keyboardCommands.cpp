#include "windows.h"
#include "iostream"
#include "keyboardCommands.h"

using namespace std;

void SetCursorPosition(int x, int y) {
	COORD coord;
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void GetConsoleSize(int& width, int& height) {
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
	width = csbi.srWindow.Right - csbi.srWindow.Left + 1;
	height = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
}

void ShowConsoleCursor(bool showFlag)
{
	HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO     cursorInfo;
	GetConsoleCursorInfo(out, &cursorInfo);
	cursorInfo.bVisible = showFlag; // set the cursor visibility
	SetConsoleCursorInfo(out, &cursorInfo);
}

void SetColor(int text, int background) {
	HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hStdOut, (WORD)((background << 4) | text));
}

void Draw(int x, int y, string text) {
	SetColor(5, 8);
	SetCursorPosition(x, y);
}

void drawConsoleFrame(int width, int height, int textColor, int backgroundColor) {
	SetColor(textColor, backgroundColor);
	int y = 0;
	for (int i = 0; i < width; ++i) cout << '*';
	for (int y = 1; y < height - 1; ++y) {
		SetCursorPosition(0, y);
		cout << '*';
		SetCursorPosition(width - 1, y);
		cout << '*';
	}
	SetCursorPosition(0, height - 1);
	for (int i = 0; i < width; ++i) cout << '*';

}

