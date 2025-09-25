#include <iostream>
#include <iomanip>
#include <clocale>
#include <filesystem>
#include <string>
#include <windows.h>
#include <conio.h>

using namespace std;
// Позиціювання курсору в консолі
void SetCursorPosition(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}
// Показати/сховати курсор
void ShowConsoleCursor(bool showFlag)
{
    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO     cursorInfo;
    GetConsoleCursorInfo(out, &cursorInfo);
    cursorInfo.bVisible = showFlag; // set the cursor visibility
    SetConsoleCursorInfo(out, &cursorInfo);
}
// Задання кольорів тексту та фону
void SetColor(int text, int background) {
    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hStdOut, (WORD)((background << 4) | text));
}
// Кольори тексту та фону
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

// Вивід вмісту директорії
void drawDirectoryContents(const filesystem::path& path, int active)
{
    SetCursorPosition(0, 0);
    cout << "Current path: " << path.string() << endl;
    cout << string(path.string().length() + 14, '-') << endl;

    int col1Width = 40;
    int col2Width = 10;

    int idx = 0;

    if (!path.parent_path().empty())
    {
        if (idx++ == active) {
            SetColor(BLACK, WHITE);
        }
        cout << left << setw(col1Width) << ".." << setw(col2Width) << "UP" << endl;
        SetColor(WHITE, BLACK);
    }

    for (const filesystem::directory_entry& entry : filesystem::directory_iterator(path))
    {
        if (idx++ == active) {
            SetColor(BLACK, WHITE);
        }
        else {
            SetColor(WHITE, BLACK);
        }
        // Ім'я файлу
        cout << left << setw(col1Width) << entry.path().filename().string();
        // Файл чи директорія
        if (entry.is_directory()) {
            cout << "<DIR>";
        }
        else {
            cout << setw(col2Width) << filesystem::file_size(entry);
        }
        cout << endl;
        SetColor(WHITE, BLACK);

    }
}

// Підрахунок кількості елементів у директорії
int getDirectoryEntriesCount(const filesystem::path& path)
{
    int count = distance(filesystem::directory_iterator(path), filesystem::directory_iterator{});
    if (!path.parent_path().empty()) {
        count++; // For the ".." entry
    }
    return count;
}

int main()
{
    setlocale(LC_ALL, "ukr");
    ShowConsoleCursor(false);

    // Поточна директорія
    filesystem::path path = filesystem::current_path();
    // Кількість елементів у директорії
    int elementsCount = getDirectoryEntriesCount(path);

    int active = 0;
    while (true) {
        // Вивід вмісту директорії
        drawDirectoryContents(path, active);

        // Обробка натиснутих клавіш
        int key = _getch();
        if (key == 224) { // Special keys (arrows, function keys, etc.)
            key = _getch();
            if (key == 72) { // Up arrow
                if (active > 0) {
                    active--;
                }
            }
            else if (key == 80) { // Down arrow
                if (active < elementsCount - 1) {
                    active++;
                }

            }
        }
        else if (key == 13) { // Enter key
            system("cls");
            int idx = 0;
            // If ".." is selected
            if (!path.parent_path().empty()) {
                if (active == 0) {
                    path = path.parent_path();
                    active = 0;
                    elementsCount = getDirectoryEntriesCount(path);
                    continue;
                }
                idx++;
            }
            // Iterate through directory entries
            for (const filesystem::directory_entry& entry : filesystem::directory_iterator(path))
            {
                // If the current index matches the active selection
                if (idx == active) {
                    if (entry.is_directory()) {
                        path = entry.path();
                        active = 0;
                        elementsCount = getDirectoryEntriesCount(path);
                    }
                    break;
                }
                idx++;
            }
        }
        else if (key == 8) { // Backspace key
            system("cls");
            // If ".." is available
            if (!path.parent_path().empty()) {
                path = path.parent_path();
                active = 0;
                elementsCount = getDirectoryEntriesCount(path);
            }
        }
    }



    /*cout << "Current path: " << path << endl;

    filesystem::path parentPath = path.parent_path();

    cout << "Parent path: " << parentPath << endl;

    parentPath = parentPath.parent_path();

    cout << "Parent path: " << parentPath << endl;*/


    // Комбінація шляхів
    // Диск C:
    /*filesystem::path path = "C:\\";
    path /= "Users";
    path /= "kvvkv";*/

    /*filesystem::path path = "C:\\";
    path.append("Users");
    path.append("kvvkv");*/

    //cout << "Path: " << path << endl;

    /*path += "Users";
    path += "kvvkv";

    cout << "Path: " << path << endl;*/



    // Перегляд файлів у вказаній директорії

    //for (const filesystem::directory_entry& entry : filesystem::directory_iterator(path))
    //{
    // // Повний шлях
    // cout << entry.path() << endl;
    // // Ім'я файлу
    // cout << entry.path().filename() << endl;
    // // Розширення файлу
    // cout << entry.path().extension() << endl;
    // // Файл чи директорія
    // cout << (entry.is_directory() ? "DIR": "FILE") << endl;
    // // Розмір файлу
    // cout << filesystem::file_size(entry) << endl;


    // auto subpath = path/=entry.path();

    //}



    /*
    1. Простий файловий менеджер

    Вивід вмісту поточної директорії

    Зверху вниз - файли та директорії (назва, розмір, тип)

    Перехід у вказану директорію
    Повернення у батьківську директорію

    Керування клавішами:
    - Up/Down - вибір файлу/директорії
    - Enter - перехід у директорію
    - Backspace - повернення у батьківську директорію


    2. Додатково
    Видалення файлів/директорій
    Створення директорій

    3. Додатково
    Перегляд текстових файлів (з розширенням txt, cpp, h, log)

    */



}


