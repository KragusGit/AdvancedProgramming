#include "Maze.h"

using namespace std;


Maze::Maze() :Grid(default_size)
{
	numberOfExitsRequired = default_num_exits;
	Maze(default_size, numberOfExitsRequired);
}


int Maze::GetSize()
{
	return Size;
}

Node* Maze::GetNode(int row, int col)
{
	return (Grid::GetNode(row, col));
}

Maze::Maze(int size, int numberOfExits):Grid(size)
{
	numberOfExitsRequired = numberOfExits;
	GenerateMaze();
}

void Maze::Print()
{
	Grid::Print();
}

char Maze::GetAppropriateNodeOutput(Node* node)
{
	return IsCenter(GetNode(node->row, node->col)) ? OUTPUT_IF_CENTER :
		IsExit(GetNode(node->row, node->col)) ? OUTPUT_IF_EXIT : Grid::GetAppropriateNodeOutput(node);
}

vector<Node*> Maze::GetValidWalls(Node* node)
{
	vector<Node*> validWalls;
	if (node->row == 0 || node->row == Size - 1 || node->col == 0 || node->col == Size - 1)
		return validWalls;

	if (node->row - 2 >= 0 && !IsWalkable(GetNode(node->row - 2, node->col))) validWalls.push_back(GetNode(node->row - 2, node->col));
	if (node->row + 2 <= Size - 1 && !IsWalkable(GetNode(node->row + 2, node->col))) validWalls.push_back(GetNode(node->row + 2, node->col));
	if (node->col - 2 >= 0 && !IsWalkable(GetNode(node->row, node->col - 2))) validWalls.push_back(GetNode(node->row, node->col - 2));
	if (node->col + 2 <= Size - 1 && !IsWalkable(GetNode(node->row, node->col + 2))) validWalls.push_back(GetNode(node->row, node->col + 2));
	return validWalls;
}

bool Maze::CanBuildPath(Node* node, int direction)
{
	switch (direction)
	{
	case 0:return	node->row > 1 ? (!IsWalkable(GetNode(node->row - 1, node->col))) && (node->row - 2 < 0 || IsWalkable(GetNode(node->row - 2, node->col))) : false;
	case 1:return	node->col < Size - 2 ? !IsWalkable(GetNode(node->row, node->col + 1)) && (node->col + 2 >= Size || IsWalkable(GetNode(node->row, node->col + 2))) : false;
	case 2:return	node->row < Size - 2 ? !IsWalkable(GetNode(node->row + 1, node->col)) && (node->row + 2 >= Size || IsWalkable(GetNode(node->row + 2, node->col))) : false;
	case 3:return	node->col > 1 ? !IsWalkable(GetNode(node->row, node->col - 1)) && (node->col - 2 < 0 || IsWalkable(GetNode(node->row, node->col - 2))) : false;
	default:return false;
	}
}

int Maze::GetRandomPossibleDirectionToBuild(Node* node)
{
	vector <int> validDir;
	for (int i = 0; i < 4; i++)
		if (CanBuildPath(node, i))  validDir.push_back(i);
	return validDir.size() > 0 ? validDir[rand() % validDir.size()] : -1;
}

void Maze::GenerateMaze()
{
	vector<Node*> walls, temp; Node* curNode;
	int center = Size / 2;
	SetWalkable(GetNode(center, center), true);
	walls.push_back(GetNode(center + 2, center));
	walls.push_back(GetNode(center, center + 2));
	walls.push_back(GetNode(center - 2, center));
	walls.push_back(GetNode(center, center - 2));

	while (walls.size() != 0)
	{
		int wallIndex = rand() % walls.size();
		curNode = walls[wallIndex];
		walls.erase(walls.begin() + wallIndex);

		if (IsWalkable(curNode))
			continue;

		SetWalkable(curNode, (curNode->row != 0 && curNode->col != 0 && curNode->row != Size - 1 && curNode->col != Size - 1));

		int buildDir = GetRandomPossibleDirectionToBuild(curNode);

		if (buildDir == -1)
			continue;

		SetWalkable(GetNode(curNode->row + (buildDir == 0 ? -1 : buildDir == 2 ? 1 : 0),
			curNode->col + (buildDir == 1 ? 1 : buildDir == 3 ? -1 : 0)), true);

		temp = GetValidWalls(curNode);
		walls.insert(walls.end(), temp.begin(), temp.end());
	}

	for (int i = -1; i <= 1; i++)
		for (int j = -1; j <= 1; j++)
			SetWalkable(GetNode(center + i, center + j), true);

	GenerateExits();
}

vector<vector<int>> Maze::GetPath(Node* start, Node* destination)
{
	if (start != destination)
		return FindPath(start, destination);
	else
		return vector<vector<int>> {0};
}

bool Maze::HasWalkableNeighbour(Node* node)
{
	return	node->row >= 0 && node->row < Size - 1 && IsWalkable(GetNode(node->row + 1, node->col))
		|| node->row > 0 && node->row <= Size - 1 && IsWalkable(GetNode(node->row - 1, node->col))
		|| node->col >= 0 && node->col < Size - 1 && IsWalkable(GetNode(node->row, node->col + 1))
		|| node->col > 0 && node->col <= Size - 1 && IsWalkable(GetNode(node->row, node->col - 1));
}


bool Maze::IsExit(Node* node)
{
	return	node->val == VAL_IF_EXIT;
}

bool Maze::IsCenter(Node* node)
{
	return	node->col == (Size / 2) && node->row == (Size / 2);
}

void Maze::SetAsExit(Node* node)
{
	node->val = VAL_IF_EXIT;
}


void Maze::GenerateExits()
{
	vector<Node*> validExits;
	for (int i = 0; i < Size; i++)
	{
		for (int j = 0; j < Size; j++)
		{
			if (i == 0 || j == 0 || i == Size - 1 || j == Size - 1)
			{
				if (HasWalkableNeighbour(GetNode(i, j)))
					validExits.push_back(GetNode(i, j));
			}
		}
	}

	int randNum;
	for (int i = 0; i < numberOfExitsRequired; i++)
	{
		randNum = rand() % validExits.size();
		GetNode(validExits[randNum]->row, validExits[randNum]->col)->val = VAL_IF_EXIT;
		Exits.push_back(validExits[randNum]);
		validExits.erase(validExits.begin() + randNum);
		numberOfExitsPlaced++;
	}
}

