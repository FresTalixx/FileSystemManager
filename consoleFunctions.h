#include <iostream>
#include <iomanip>
#include <clocale>
#include <filesystem>
#include <string>
#include <windows.h>
#include <conio.h>

using namespace std;
// ������������ ������� � ������
void SetCursorPosition(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}
// ��������/������� ������
void ShowConsoleCursor(bool showFlag)
{
    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO     cursorInfo;
    GetConsoleCursorInfo(out, &cursorInfo);
    cursorInfo.bVisible = showFlag; // set the cursor visibility
    SetConsoleCursorInfo(out, &cursorInfo);
}
// ������� ������� ������ �� ����
void SetColor(int text, int background) {
    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hStdOut, (WORD)((background << 4) | text));
}
// ������� ������ �� ����
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

// ���� ����� ��������
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
        // ��'� �����
        cout << left << setw(col1Width) << entry.path().filename().string();
        // ���� �� ���������
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

// ϳ�������� ������� �������� � ��������
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

    // ������� ���������
    filesystem::path path = filesystem::current_path();
    // ʳ������ �������� � ��������
    int elementsCount = getDirectoryEntriesCount(path);

    int active = 0;
    while (true) {
        // ���� ����� ��������
        drawDirectoryContents(path, active);

        // ������� ���������� �����
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


    // ��������� ������
    // ���� C:
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



    // �������� ����� � ������� ��������

    //for (const filesystem::directory_entry& entry : filesystem::directory_iterator(path))
    //{
    // // ������ ����
    // cout << entry.path() << endl;
    // // ��'� �����
    // cout << entry.path().filename() << endl;
    // // ���������� �����
    // cout << entry.path().extension() << endl;
    // // ���� �� ���������
    // cout << (entry.is_directory() ? "DIR": "FILE") << endl;
    // // ����� �����
    // cout << filesystem::file_size(entry) << endl;


    // auto subpath = path/=entry.path();

    //}



    /*
    1. ������� �������� ��������

    ���� ����� ������� ��������

    ������ ���� - ����� �� �������� (�����, �����, ���)

    ������� � ������� ���������
    ���������� � ���������� ���������

    ��������� ��������:
    - Up/Down - ���� �����/��������
    - Enter - ������� � ���������
    - Backspace - ���������� � ���������� ���������


    2. ���������
    ��������� �����/���������
    ��������� ���������

    3. ���������
    �������� ��������� ����� (� ����������� txt, cpp, h, log)

    */



}


