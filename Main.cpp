#include "Maze.h"
#include<fstream>
#include "Main.h"
#include<chrono>
#include<thread>
#include <string>
#include<filesystem>

using namespace std;
namespace fileSys = std::filesystem;

const string FOLDER_NAME = "SavedMazes";
const string MAIN_MENU_TITLE = "Please Choose one from these options:";
const string MAIN_MENU_OPTION1 = "1. Generate a new Maze.";
const string MAIN_MENU_OPTION2 = "2. Load existing Maze.";
const string MAIN_MENU_EXIT_HINT = "3. EXIT. ";
const string MAIN_MENU_INPUT_HINT = "To select an option, type respective numbers : ";
const string MAZE_GENERATION_TITLE = "Please answer following questions : ";
const string MAZE_GENERATION_SIZE_INPUT_HINT = "Enter size of maze(Max - 50, MIN - 5) : ";
const string MAZE_GENERATION_EXIT_INPUT_HINT = "Enter number of exits to create(Max-%d) : ";
const string GENERIC_ERROR_MESSAGE = "Wrong Input! Please try again. ";
const string MAZE_LOAD_MENU_TITLE = "These are the list of files you can open : ";
const string SAVE_FILE_HINT = "Do You Want To Save This Maze ? (y/n) : ";
const int MAX_MAZE_SIZE = 50;

Maze* maze;
vector<vector<int>> Path;
string fileString;
int numberOfExits;
int maze_size;
fstream fs;
int** pathMap;

void PrintMaze(Maze* maze)
{
	char charToPrint;
	for (int i = 0; i < maze->GetSize(); ++i) {
		for (int j = 0; j < maze->GetSize(); ++j) {

			if (((i == maze->GetSize() / 2 && j == maze->GetSize() / 2)))
				charToPrint = 'S';
			else if (pathMap[i][j] == 1)
				charToPrint = 'o';
			else
				charToPrint = maze->GetAppropriateNodeOutput(maze->GetNode(i, j));

			cout << charToPrint << " ";
			fileString += charToPrint;
			fileString += " ";
		}
		cout << endl;
		fileString += "\n";
	}
	fileString += "\n";
}


void GenerateMaze()
{
	maze = new Maze(maze_size, numberOfExits);
}

void ShowMazeLoadMenu()
{
	int input, counter = 0;
	vector<string> fileNames;

	cout << MAZE_LOAD_MENU_TITLE << endl;
	for (const auto& entry : fileSys::directory_iterator(FOLDER_NAME)) {
		fileNames.push_back(entry.path().string());
		cout << ++counter << ". " << entry.path().string() << endl;
	}

	if (counter == 0)
	{
		cout << "No Saved Mazes Present.";
		return;
	}

	while (true)
	{
		cout << "Choice :";
		cin >> input;
		cout << endl;

		if (cin.fail() || input > counter || input < 1) {
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << GENERIC_ERROR_MESSAGE << endl;
		}
		else {
			PrintFromFile(fileNames[(input - 1)]);
			break;
		}
	}
}

void PrintFromFile(string path)
{
	fileString.clear();
	fs.open(path, ios::in);

	if (fs.is_open())
	{
		while (getline(fs, fileString))
		{
			cout << fileString << endl;
		}
		fs.close();
	}
	else
		cout << "Could not print " << path << endl;
}

void EXIT()
{
	exit(0);
}

void ShowMazeGenerationMenu()
{
	int input;
	cout << MAZE_GENERATION_TITLE << endl;
	while (true)
	{
		cout << MAZE_GENERATION_SIZE_INPUT_HINT;
		cin >> input;
		cout << endl;

		if (cin.fail() || input > MAX_MAZE_SIZE || input < 5) {
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << GENERIC_ERROR_MESSAGE << endl;
		}
		else {
			maze_size = input;
			break;
		}
	}

	while (true)
	{
		cout << MAZE_GENERATION_EXIT_INPUT_HINT << "Maximum allowed( " << maze_size / 2 << " ) : ";
		cin >> input;
		cout << endl;

		if (cin.fail() || input > maze_size / 2 || input < 0) {
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << GENERIC_ERROR_MESSAGE << endl;
		}
		else
		{
			numberOfExits = input;
			break;
		}
	}

	GenerateMaze();

	if (maze->Exits.size() > 0)
	{
		pathMap = new int* [maze->GetSize()];
		for (int i = 0; i < maze->GetSize(); i++)
		{
			pathMap[i] = new int[maze->GetSize()];
			for (int j = 0; j < maze->GetSize(); j++)
				pathMap[i][j] = 0;
		}
		for (int i = 0; i < maze->Exits.size(); i++)
		{
			Path = maze->GetPath(maze->Exits[i], maze->GetNode(maze->GetSize() / 2, maze->GetSize() / 2));
			for (auto point : Path)
				pathMap[point[0]][point[1]] = 1;
		}
	}

	PrintMaze(maze);
	ShowSaveOption();
}

void ShowSaveOption()
{
	char input;
	while (true)
	{
		cout << SAVE_FILE_HINT;
		cin >> input;
		cout << endl;
		if (input == 'y' || input == 'Y' || input == 'n' || input == 'N')
			break;
		else
			cout << GENERIC_ERROR_MESSAGE << endl;
	}
	switch (input)
	{
	case 'Y':
	case 'y':WriteToFile(); break;
	}
}

void WriteToFile()
{
	string fileName;
	int fileNumber = 1;
	if (!fileSys::exists(FOLDER_NAME) || !fileSys::is_directory(FOLDER_NAME))
		fileSys::create_directory(FOLDER_NAME);

	while (true)
	{
		fileName = "Maze_" + std::to_string(fileNumber) + ".txt";
		if (fileSys::exists(FOLDER_NAME + "\\" + fileName))
			fileNumber++;
		else
			break;
	}

	fs.open(FOLDER_NAME + "\\" + fileName, fstream::app);
	if (fs.is_open())
	{
		fs << fileString;
		fs.close();
		cout << "Maze saved in " << FOLDER_NAME + "/" + fileName << endl;
	}
	else
		cout << "Maze could not be saved!" << endl;
}


void ShowMainMenu()
{
	int choice;

	while (true)
	{
		cout << MAIN_MENU_TITLE << endl;
		cout << MAIN_MENU_OPTION1 << endl;
		cout << MAIN_MENU_OPTION2 << endl;
		cout << MAIN_MENU_EXIT_HINT << endl;
		cout << MAIN_MENU_INPUT_HINT;
		cin >> choice;
		cout << endl;
		if (cin.fail() || choice > 3 || choice < 1) {
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << GENERIC_ERROR_MESSAGE << endl;
		}
		else
			break;
	}

	switch (choice)
	{
	case 1: ShowMazeGenerationMenu(); break;
	case 2: ShowMazeLoadMenu(); break;
	case 3: EXIT(); break;
	}

}

void main()
{
	while (true) {
		srand(time(NULL));
		ShowMainMenu();
		cout << endl;
	}
}