#include <iostream>
#include <iomanip>
#include <clocale>
#include <filesystem>
#include <string>
#include <windows.h>
#include <conio.h>
#include <fstream>

#include "keyboardCommands.h"
#include "keyboardMenuControll.h"

using namespace std;

int startMenuPrintX = 55;
int startMenuPrintY = 3;
const int menuSize = 6;

int infoPrintPosY = startMenuPrintY + menuSize + 1;

// Вивід вмісту директорії
void drawDirectoryContents(const filesystem::path& path, int activeRow)
{
    SetCursorPosition(0, 0);
    cout << "Current path: " << path.string() << endl;
    cout << string(path.string().length() + 14, '-') << endl;

    int col1Width = 40;
    int col2Width = 10;
    int col3Width = 20;

    int index = 0;
    

    const char* controlMenu[menuSize] = {
        "Arrow down - move down\n",
        "Arrow up - move up\n",
        "Enter - enter directory\n",
        "Backspace - move to parent directory\n",
        "Cntl + N - create new directory\n",
        "Cntl + O - open the file\n"
    };

    
    if (path != path.root_directory())
    if (!path.parent_path().empty())
    {
        if (index == activeRow) {
            SetColor(BLACK, WHITE);
        }

        cout << left << setw(col1Width) << ".." << setw(col2Width) << "UP" << endl;
        SetColor(WHITE, BLACK);

        index++;
    }

    for (const filesystem::directory_entry& entry : filesystem::directory_iterator(path))
    {
        if (index == activeRow) {
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

        
         

        SetColor(WHITE, BLACK);
        cout << endl;

        index++;

    }
    for (int i = 0; i < menuSize; ++i) {
        SetCursorPosition(startMenuPrintX, startMenuPrintY + i);
        cout << controlMenu[i];
    }
}

int getDirectoryEntriesCount(const filesystem::path& path)
{
    auto count = distance(filesystem::directory_iterator(path), filesystem::directory_iterator{});
    if (!path.parent_path().empty()) {
        count++; // For the ".." entry
    }
    return count;
}

string readDataFromFile(string filePath) {
    ifstream file(filePath);
    string str, file_contents;

    if (file.is_open()) {
        while (getline(file, str)) {
            file_contents += str;
            file_contents.push_back('\n');
        }
    }

    return file_contents;
}

void copyFileTo(filesystem::path from, filesystem::path to, bool deleteCopyPathFile) {
    if (from.string() != "") {
        if (from.extension() == "") { // directory check cause directories don't have extensions
            filesystem::create_directories(from.filename().string());
            filesystem::copy(from, to / from.filename());
        }
        else {
            filesystem::copy(from, to);
        }
        
    }
    if (deleteCopyPathFile) filesystem::remove_all(from);

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
    string newFolderName;

    filesystem::path copyPath;
    bool deleteCopyPathFile;

    while (true) {
        drawDirectoryContents(path, active);
        // control menu up and down and entity deleting
        int key = _getch();
        if (key == 224) {
            key = _getch();
            if (key == 72) {
                if (active > 0) active--; // up
            }
            else if (key == 80) {
                if (active < elementsCount - 1)  active++; // down
            }

            // delete something
            else if (key == 83) {
                cout << path;
                // don't include ".." entry to delete properly
                int index = 1;

                for (const filesystem::directory_entry& entry : filesystem::directory_iterator(path))
                {
                    // If the current index matches the active selection
                    if (index == active) {
                        if (entry.is_directory()) {

                            // remove
                            filesystem::remove_all(entry.path());
                            // redraw the filesystem
                            system("cls");
                            active = 0;
                            elementsCount = getDirectoryEntriesCount(path);
                        }
                        else {
                            // remove
                            filesystem::remove(entry.path());
                            // redraw the filesystem
                            system("cls");
                            active = 0;
                            elementsCount = getDirectoryEntriesCount(path);
                        }
                        break;
                    }
                    index++;
                }
            }
        }
        else if (key == 13) {
            system("cls");
            int index = 0;

            if (!path.parent_path().empty()) {
                if (active == 0) {
                    path = path.parent_path();
                    elementsCount = getDirectoryEntriesCount(path);
                    continue;
                }
            }
            index++;
                
            for (const filesystem::directory_entry& entry : filesystem::directory_iterator(path))
            {
                // If the current index matches the active selection
                if (index == active) {
                    if (entry.is_directory()) {
                        path = entry.path();
                        active = 0;
                        elementsCount = getDirectoryEntriesCount(path);
                    }
                    break;
                }
                index++;
            }
            
        }
        else if (key == 8) {
            system("cls");
            path = path.parent_path();
            active = 0;
            elementsCount = getDirectoryEntriesCount(path);
        }
        else if (key == 14) {
            // get the folder name
            SetCursorPosition(startMenuPrintX, infoPrintPosY);
            ShowConsoleCursor(true);
            cout << "Enter the name of the folder: "; getline(cin, newFolderName);
            ShowConsoleCursor(false);
            // create a folder
            filesystem::create_directories(path / newFolderName);

            // redraw the filesystem
            system("cls");
            active = 0;
            elementsCount = getDirectoryEntriesCount(path);
        }
        else if (key == 15) {
            SetCursorPosition(startMenuPrintX, infoPrintPosY);
            int index = 1;
            for (const filesystem::directory_entry& entry : filesystem::directory_iterator(path))
            {
                
                if (index == active) {
                    if (!entry.is_directory()) {
                        // read the full path
                        filesystem::path fullFilePath = entry.path();
                        
                        // proceed it
                        ShowConsoleCursor(true);
                        string fileData = readDataFromFile(fullFilePath.string());
                        system("cls");
                        cout << fileData;
                        cin.get();
                        ShowConsoleCursor(false);
                        system("cls");
                        break;
                        
                    }
                }
                
                index++;
            }
        
        }
        else if (key == 3) {
            int index = 1;
            for (const filesystem::directory_entry& entry : filesystem::directory_iterator(path))
            {

                if (index == active) {
                    copyPath = entry.path();
                    break;
                    
                }
                index++;
            }
            deleteCopyPathFile = false;
        }
        else if (key == 22) {
            copyFileTo(copyPath, path, deleteCopyPathFile);
            system("cls");
            elementsCount = getDirectoryEntriesCount(path);
        }
        else if (key == 24) {
            int index = 1;
            for (const filesystem::directory_entry& entry : filesystem::directory_iterator(path))
            {

                if (index == active) {
                    copyPath = entry.path();
                    break;

                }
                index++;
            }
            deleteCopyPathFile = true;
        }
    }

}


