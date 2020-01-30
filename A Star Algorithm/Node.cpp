#include "Node.h"

void Node::calcHCost(Vec2 _finish)
{
	float dX = (position.xPos - _finish.xPos);
	float dY = (position.yPos - _finish.yPos);
	
	hCost = sqrt((dX*dX + dY*dY)); //Euclidean distance 
}

bool Node::validNode(Direction _dir, Map _tileMap)
{
	bool valid = false;

	switch (_dir)
	{
	case Up:
		if ((position.yPos != 0) && (_tileMap.map[position.yPos - 1][position.xPos] != 1))
		{
			valid = true;
		}

		break;

	case Down:
		if ((position.yPos != (_tileMap.height - 1)) && (_tileMap.map[position.yPos + 1][position.xPos] != 1))
		{
			valid = true;
		}
		break;

	case Right:
		if ((position.xPos != (_tileMap.width - 1)) && (_tileMap.map[position.yPos][position.xPos + 1] != 1))
		{
			valid = true;
		}
		break;

	case Left:
		if ((position.xPos != 0) && (_tileMap.map[position.yPos][position.xPos - 1] != 1))
		{
			valid = true;
		}
		break;

	case Up_Right:
		if ((position.yPos != 0 && (position.xPos != (_tileMap.width - 1))) && (_tileMap.map[position.yPos - 1][position.xPos + 1] != 1))
		{
			valid = true;
		}
		break;

	case Up_Left:
		if ((position.yPos != 0 && position.xPos != 0) && (_tileMap.map[position.yPos - 1][position.xPos - 1] != 1))
		{
			valid = true;
		}
		break;

	case Down_Right:
		if (((position.yPos != (_tileMap.height - 1)) && (position.xPos != (_tileMap.width - 1))) && (_tileMap.map[position.yPos + 1][position.xPos + 1] != 1))
		{
			valid = true;
		}
		break;

	case Down_Left:
		if (((position.yPos != (_tileMap.height - 1)) && position.xPos != 0) && (_tileMap.map[position.yPos + 1][position.xPos - 1] != 1))
		{
			valid = true;
		}
		break;

	}

	return valid;
}

bool Node::availableNode(Direction _dir, Map _tileMap, std::vector<Node> _open, std::vector<Node> _closed)
{
	bool valid = true;
	//validation that the node is not on the closed or open list
	switch (_dir)
	{
	case Up:
		for (int i = 0; i < _closed.size(); i++)
		{
			if ((position.yPos - 1 == _closed[i].position.yPos) && (position.xPos == _closed[i].position.xPos))
			{
				valid = false;
			}
		}
		for (int i = 0; i < _open.size(); i++)
		{
			if ((position.yPos - 1 == _open[i].position.yPos) && (position.xPos == _open[i].position.xPos))
			{
				valid = false;
			}
		}

		break;

	case Down:
		for (int i = 0; i < _closed.size(); i++)
		{
			if ((position.yPos + 1 == _closed[i].position.yPos) && (position.xPos == _closed[i].position.xPos))
			{
				valid = false;
			}
		}
		for (int i = 0; i < _open.size(); i++)
		{
			if ((position.yPos + 1 == _open[i].position.yPos) && (position.xPos == _open[i].position.xPos))
			{
				valid = false;
			}
		}

		break;

	case Right:
		for (int i = 0; i < _closed.size(); i++)
		{
			if ((position.yPos == _closed[i].position.yPos) && (position.xPos + 1 == _closed[i].position.xPos))
			{
				valid = false;
			}
		}
		for (int i = 0; i < _open.size(); i++)
		{
			if ((position.yPos == _open[i].position.yPos) && (position.xPos + 1 == _open[i].position.xPos))
			{
				valid = false;
			}
		}

		break;

	case Left:
		for (int i = 0; i < _closed.size(); i++)
		{
			if ((position.yPos == _closed[i].position.yPos) && (position.xPos - 1 == _closed[i].position.xPos))
			{
				valid = false;
			}
		}
		for (int i = 0; i < _open.size(); i++)
		{
			if ((position.yPos == _open[i].position.yPos) && (position.xPos - 1 == _open[i].position.xPos))
			{
				valid = false;
			}
		}

		break;

	case Up_Right:
		for (int i = 0; i < _closed.size(); i++)
		{
			if ((position.yPos - 1 == _closed[i].position.yPos) && (position.xPos + 1 == _closed[i].position.xPos))
			{
				valid = false;
			}
		}
		for (int i = 0; i < _open.size(); i++)
		{
			if ((position.yPos - 1 == _open[i].position.yPos) && (position.xPos + 1 == _open[i].position.xPos))
			{
				valid = false;
			}
		}

		break;

	case Up_Left:
		for (int i = 0; i < _closed.size(); i++)
		{
			if ((position.yPos - 1 == _closed[i].position.yPos) && (position.xPos - 1 == _closed[i].position.xPos))
			{
				valid = false;
			}
		}
		for (int i = 0; i < _open.size(); i++)
		{
			if ((position.yPos - 1 == _open[i].position.yPos) && (position.xPos - 1 == _open[i].position.xPos))
			{
				valid = false;
			}
		}

		break;

	case Down_Right:
		for (int i = 0; i < _closed.size(); i++)
		{
			if ((position.yPos + 1 == _closed[i].position.yPos) && (position.xPos + 1 == _closed[i].position.xPos))
			{
				valid = false;
			}
		}
		for (int i = 0; i < _open.size(); i++)
		{
			if ((position.yPos + 1 == _open[i].position.yPos) && (position.xPos + 1 == _open[i].position.xPos))
			{
				valid = false;
			}
		}
		break;

	case Down_Left:
		for (int i = 0; i < _closed.size(); i++)
		{
			if ((position.yPos + 1 == _closed[i].position.yPos) && (position.xPos - 1 == _closed[i].position.xPos))
			{
				valid = false;
			}
		}
		for (int i = 0; i < _open.size(); i++)
		{
			if ((position.yPos + 1 == _open[i].position.yPos) && (position.xPos - 1 == _open[i].position.xPos))
			{
				valid = false;
			}
		}

		break;

	}

	return valid;
}

bool Node::isInClosedList(Map _tileMap, std::vector<Node> _closed)
{

	bool valid = true;

	for (int i = 0; i < _closed.size(); i++)
	{
		if (((position.yPos == _closed[i].position.yPos) && (position.xPos == _closed[i].position.xPos)))
		{
			valid = false;
		}
	}

	return valid;
}
