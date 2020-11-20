#include "Node.h"
#include <algorithm>
#include <map>
#include <vector>

using namespace std;

class PathNode
{
public:
	PathNode();
	PathNode(int row, int col, PathNode* parent, bool canWalk);
	int row, col;
	bool isWalkable;
	float gScore, hScore;
	PathNode* Parent;
	float fScore();
};

class Astar
{
private:
	int Size;
	float calculateH(int row, int col, int destRow, int destCol);
	bool IsValid(int row, int col);
	bool IsWalkable(PathNode* node, PathNode** root);

	bool IsDestination(PathNode* node, Node* Destination);

	vector<PathNode*> GetNeighbours(PathNode* node, PathNode** root);

public:
	
	vector<vector<int>>  FindPath(Node* start, Node* destination, Node** root, int size);
};


