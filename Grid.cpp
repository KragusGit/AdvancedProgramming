#include "Grid.h"

Grid::Grid()
{
	Grid::Grid(default_size);
}

Grid::Grid(int size)
{
	Size = size;
	Root = new Node * [size];
	for (int i = 0; i < Size; i++)
	{
		Root[i] = new Node[Size];
		for (int j = 0; j < Size; j++)
		{
			Root[i][j].row = i;
			Root[i][j].col = j;
		}
	}
}

Grid::~Grid()
{

	for (int i = 0; i < Size; i++)	 delete[] Root[i];
	delete[] Root;
}


int Grid::GetSize()
{
	return Size;
}

Node* Grid::GetNode(int row, int col)
{
	return &Root[row][col];
}

char Grid::GetAppropriateNodeOutput(Node* node)
{
	return node->val == VAL_IF_WALKABLE ? OUTPUT_IF_WALKABLE : node->val == VAL_IF_WALL ? OUTPUT_IF_NOT_WALKABLE : '9';
}

void Grid::Print()
{
	for (int i = 0; i < Size; ++i) {
		for (int j = 0; j < Size; ++j)
			cout << GetAppropriateNodeOutput(GetNode(i, j)) << " ";
		cout << endl;
	}
}

bool Grid::IsWalkable(Node* node)
{
	return node->val == VAL_IF_WALKABLE;
}

void Grid::SetWalkable(Node* node, bool isWalkable)
{
	node->val = isWalkable ? VAL_IF_WALKABLE : VAL_IF_WALL;
}


vector<vector<int>> Grid::FindPath(Node* start, Node* destination)
{
	Astar pathFinder;
	return	pathFinder.FindPath(start, destination, Root, Size);
}


