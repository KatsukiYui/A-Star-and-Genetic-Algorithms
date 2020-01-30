#ifndef _NODE_
#define _NODE_

#include "Vec2.h"
#include "Map.h"
#include <cmath>

enum Direction { Up, Down, Right, Left, Up_Right, Up_Left, Down_Right, Down_Left };

struct Node
{
	Vec2 position = {0, 0};
	Vec2 parentPos = {0, 0};

	double fCost = 0;
	double gCost = 0;
	double hCost = 0;

	void calcHCost(Vec2 _finish);

	//node is not a wall or outside the map
	bool validNode(Direction _dir, Map _tileMap);

	//checks if the node is in the open or closed list
	bool availableNode(Direction _dir, Map _tileMap, std::vector<Node> _open, std::vector<Node> _closed);

	//checks if the node is in the closed list
	bool isInClosedList(Map _tileMap, std::vector<Node> _closed);
};

#endif