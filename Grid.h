#include <iostream>
#include <vector>
#include "Node.h"
#include "Astar.h"
using namespace std;

const int default_size = 20;
const char OUTPUT_IF_WALKABLE = ' ';
const char OUTPUT_IF_NOT_WALKABLE = 'X';
const int VAL_IF_WALKABLE = 0;
const int VAL_IF_WALL = 1;

class Grid
{
private:
	Node** Root;

protected:
	int Size = 0;

public:

	Grid();
	Grid(int size);
	~Grid();
	int GetSize();
	Node* GetNode(int row, int col);
	virtual char GetAppropriateNodeOutput(Node* node);
	virtual void Print();
	bool IsWalkable(Node* node);
	void SetWalkable(Node* node, bool isWalkable);
	vector<vector<int>>  FindPath(Node* start, Node* destination);
};

