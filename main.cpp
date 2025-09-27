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
using namespace std::chrono_literals;

int startMenuPrintX = 55;
int startMenuPrintY = 3;
const int menuSize = 10;

int infoPrintPosY = startMenuPrintY + menuSize + 1;

// print all of the disks
void listAllTheDisks(string disks[], int& count) {
    DWORD drives = GetLogicalDrives();
    count = 0;

    for (char letter = 'A'; letter <= 'Z'; ++letter) {
        if (drives & (1 << (letter - 'A'))) {
            string root = string(1, letter) + ":\\";
            try {
                if (filesystem::exists(root)) {
                    /*cout << root << "\n";*/
                    disks[count++] = root;
                }
            }
            catch (const filesystem::filesystem_error& e) {
                cerr << "Error accessing " << root << ": " << e.what() << "\n";
            }
        }
    }

}


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
    bool isRoot = false;
    

    const char* controlMenu[menuSize] = {
        "Arrow down - move down\n",
        "Arrow up - move up\n",
        "Enter - enter directory\n",
        "Backspace - move to parent directory\n",
        "Cntl + N - create new directory\n",
        "Cntl + O - open the file\n",
        "Cntl + X - cut the file\n",
        "Cntl + C - copy the file\n",
        "Cntl + V - paste the file\n",
        "Cntl + L - Look for file\n"
    };


    if (!path.empty())
    {
        
        isRoot = false;
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
        //filesystem::file_time_type lastWriteTime = filesystem::last_write_time(path);
        //cout << lastWriteTime;
    
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
        if (filesystem::is_directory(from)) { // directory check cause directories don't have extensions
            filesystem::create_directories(to / from.filename().string());
            filesystem::copy(from, to / from.filename(), filesystem::copy_options::overwrite_existing);
        }
        else {
            filesystem::copy(from, to);
        }
        
    }
    if (deleteCopyPathFile) filesystem::remove_all(from);

}

void findFileByName(string targetFile, filesystem::path directory, int startPosX, int startPosY) {
    int y = startPosY;
    int filesFound = 0;
    for (const auto& entry : filesystem::recursive_directory_iterator(directory)) {
        if (entry.is_regular_file() && entry.path().filename() == targetFile) {
            SetCursorPosition(startMenuPrintX, y);
            ++y;
            cout << "Found: " << entry.path() << endl;
            ++filesFound;
        }
    }

    if (filesFound == 0) {
        SetCursorPosition(startMenuPrintX, y);
        cout << "No files has been found with matching the name \"" << targetFile << "\" \n";
    }
}

void findByFileExtension(string targetExtension, filesystem::path directory, int startPosX, int startPosY) {
    int y = startPosY;
    int filesFound = 0;
    for (const auto& entry : filesystem::recursive_directory_iterator(directory)) {
        if (entry.is_regular_file() && entry.path().extension() == targetExtension) {
            SetCursorPosition(startMenuPrintX, y);
            ++y;
            cout << "Found: " << entry.path() << endl;
            ++filesFound;
        }
    }

    if (filesFound == 0) {
        SetCursorPosition(startMenuPrintX, y);
        cout << "No files has been found with matching the name \"" << targetExtension << "\" \n";
    }
}

int findMatchingWordsFile(const string& filepath, string word) {
    string str;
    int lineNumber = 0;
    int wordsCount = 0;

    // lowercase the search word
    transform(word.begin(), word.end(), word.begin(), ::tolower);
    ifstream file(filepath);
    if (file.is_open()) {
        while (getline(file, str)) {
            lineNumber++;

            // make a lowercase copy of the line
            string lowerStr = str;
            transform(lowerStr.begin(), lowerStr.end(), lowerStr.begin(), ::tolower);

            if (lowerStr.find(word) != string::npos) {
                cout << "'" << word << "' found at line: " << lineNumber << endl;
                wordsCount++;
            }
        }
    }
    return wordsCount;
}

void findAndReplaceWordsFile(const string& filepath, string wordToReplace, string wordToReplaceWith) {
    string str;
    int lineNumber = 0;
    string fileContent;

    // lowercase the search word
    transform(wordToReplace.begin(), wordToReplace.end(), wordToReplace.begin(), ::tolower);
    ifstream file(filepath);
    if (file.is_open()) {
        while (getline(file, str)) {
            lineNumber++;

            // make a lowercase copy of the line
            string lowerStr = str;
            transform(lowerStr.begin(), lowerStr.end(), lowerStr.begin(), ::tolower);

            int wordLength = wordToReplace.length();
            int pos = lowerStr.find(wordToReplace);
            if (pos != string::npos) {
                str.replace(pos, wordLength, wordToReplaceWith);
            }
            fileContent += str += '\n';
        }
        file.close();

        ofstream file(filepath);
        if (file.is_open()) {
            file << fileContent;
        }
        file.close();
    }
}

void printFileMenu(const char* controlMenuFile[], const int fileMenuSize) {
    for (int i = 0; i < fileMenuSize; ++i) {
        SetCursorPosition(startMenuPrintX, startMenuPrintY + i);
        cout << controlMenuFile[i] << endl;
    }
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
    bool deleteCopyPathFile = false;

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
            
            int index = 0;
            system("cls");
            if (active == 0) {
                if (path != path.root_path()) {
                    system("cls");
                    path = path.parent_path();
                    elementsCount = getDirectoryEntriesCount(path);
                    continue;
                }
                else {
                    path = "";
                    system("cls");
                    string disks[26];
                    int count;
                    listAllTheDisks(disks, count);

                    const char* cDisks[26];
                    for (int i = 0; i < count; i++) {
                        cDisks[i] = disks[i].c_str();
                    }
                    cout << "Current path: This PC" << endl;
                    cout << string(path.string().length() + 14, '-') << endl;

                    int choice = menuControl(cDisks, count, 0, 2);

                    path = disks[choice - 1];
                    system("cls");
                    path = path.parent_path();
                    elementsCount = getDirectoryEntriesCount(path);
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
                const int fileMenuSize = 2;
                const char* controlMenuFile[fileMenuSize] = {
                    "Cntl + F - find the word repetitions in the file",
                    "Cntl + R - replace the particular word in the file"
                };
                if (index == active) {
                    if (!entry.is_directory()) {
                        // read the full path
                        filesystem::path fullFilePath = entry.path();
                        
                        // proceed it
                        ShowConsoleCursor(true);
                        string fileData = readDataFromFile(fullFilePath.string());
                        system("cls");
                        cout << fileData;
                        printFileMenu(controlMenuFile, fileMenuSize);
                        while (key != 13) {
                            key = _getch();
                            if (key == 6) {
                                string word;
                                SetCursorPosition(startMenuPrintX, startMenuPrintY + fileMenuSize + 2);
                                cout << "Enter the word to find: "; getline(cin, word);
                                int wordRepetitions = findMatchingWordsFile(entry.path().string(), word);
                                cin.get();


                                system("cls");
                                cout << fileData;
                                printFileMenu(controlMenuFile, fileMenuSize);
                            }
                            else if (key == 18) {
                                string word, wordToReplaceWith;
                                SetCursorPosition(startMenuPrintX, startMenuPrintY + fileMenuSize + 2);
                                cout << "Enter the word to replace: "; getline(cin, word);
                                SetCursorPosition(startMenuPrintX, startMenuPrintY + fileMenuSize + 3);
                                cout << "Enter the word to replace with: "; getline(cin, wordToReplaceWith);
                                findAndReplaceWordsFile(entry.path().string(), word, wordToReplaceWith);

                                //reload file
                                ifstream file(entry.path());
                                string fileData;
                                if (file.is_open()) {
                                    string line;
                                    while (getline(file, line)) {
                                        fileData += line + "\n";
                                    }
                                    file.close();
                                }

                                system("cls");
                                cout << fileData;
                                printFileMenu(controlMenuFile, fileMenuSize);
                            }
                        }
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
        else if (key == 12) {
            SetCursorPosition(startMenuPrintX, infoPrintPosY);
            cout << "Choose what do you want to search: ";
            const int searchMenuSize = 2;
            const char* searchMenu[searchMenuSize] = {
                "Search by mask",
                "Search by name",
            };

            int choice = menuControl(searchMenu, searchMenuSize, startMenuPrintX, infoPrintPosY + 1);
            
            if (choice == 1) {
                string targetExtension;
                SetCursorPosition(startMenuPrintX, infoPrintPosY + 3);
                ShowConsoleCursor(true);
                cout << "Enter the extension of a file with a dot: "; getline(cin, targetExtension);
                findByFileExtension(targetExtension, path, startMenuPrintX, infoPrintPosY + 5);
                cin.get();
                ShowConsoleCursor(false);
                system("cls");
            }
            else if (choice == 2) {
                string targetFile;
                SetCursorPosition(startMenuPrintX, infoPrintPosY + 3);
                ShowConsoleCursor(true);
                cout << "Enter the name of the file to search with an extention: "; getline(cin, targetFile);
                findFileByName(targetFile, path, startMenuPrintX, infoPrintPosY + 5);
                cin.get();
                ShowConsoleCursor(false);
                system("cls");
            }
            
            
        }
    }

}


