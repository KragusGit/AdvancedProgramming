#include "Grid.h"

const int default_num_exits = 5;
const int VAL_IF_EXIT = 2;
const char OUTPUT_IF_EXIT = 'E';
const char OUTPUT_IF_CENTER = 'S';

class Maze :private Grid
{
private:
	const int center = Size / 2;
	int numberOfExitsRequired;
	int numberOfExitsPlaced;
	vector<Node*> GetValidWalls(Node* node);
	bool CanBuildPath(Node* node, int direction);
	int GetRandomPossibleDirectionToBuild(Node* node);
	bool HasWalkableNeighbour(Node* node);
	bool IsExit(Node* node);
	bool IsCenter(Node* node);
	void SetAsExit(Node* node);
	void GenerateExits();


public:
	vector<Node*> Exits;
	Maze();
	Maze(int size, int numberOfExits);
	int	GetSize();
	Node* GetNode(int row, int col);
	virtual void Print();
	virtual char GetAppropriateNodeOutput(Node* node);
	void GenerateMaze();
	vector<vector<int>> GetPath(Node* start, Node* destination);
};
