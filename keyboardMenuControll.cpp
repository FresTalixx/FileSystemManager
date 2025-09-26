#include "keyboardCommands.h"
#include <conio.h>

using namespace std;

int menuControl(const char* menu[], int menuSize, int startLineX, int startLineY) {

	SetCursorPosition(startLineX, startLineY);
	int y = startLineY;

	SetColor(Colors::WHITE, Colors::BLACK);

	for (int i = 0; i < menuSize; ++i) {
		SetCursorPosition(startLineX, y + i);
		cout << menu[i] << "\n";
	}

	SetCursorPosition(startLineX, startLineY);
	SetColor(Colors::BLACK, Colors::WHITE);
	cout << menu[y - startLineY];
	ShowConsoleCursor(false);

	while (1) {
		int key = _getch();
		if (key == 224) {
			key = _getch();

			switch (key) {



			case 72:
				SetCursorPosition(startLineX, y);
				SetColor(Colors::WHITE, Colors::BLACK);
				cout << menu[y - startLineY];
				--y + startLineY;
				if (y < startLineY)
					y = startLineY + menuSize - 1;
				SetCursorPosition(startLineX, y);
				SetColor(Colors::BLACK, Colors::WHITE);
				cout << menu[y - startLineY];
				SetColor(Colors::WHITE, Colors::BLACK);
				break;

			case 80:
				SetCursorPosition(startLineX, y);
				SetColor(Colors::WHITE, Colors::BLACK);
				cout << menu[y - startLineY];
				++y + startLineY;
				if (y > startLineY + menuSize - 1)
					y = startLineY;
				SetCursorPosition(startLineX, y);
				SetColor(Colors::BLACK, Colors::WHITE);
				cout << menu[y - startLineY];
				SetColor(Colors::WHITE, Colors::BLACK);
				break;

			}

		}
		else if (key == 13) {
			SetColor(WHITE, BLACK);
			return y - startLineY + 1;

		}
	}



}