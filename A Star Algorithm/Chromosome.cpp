#include "Chromosome.h"

void Chromosome::generateGenes(Map _tileMap)
{
	//uniform distribution
	std::random_device rand;//used to make the seed
	std::default_random_engine generator(rand());
	std::uniform_int_distribution<int> distribution(0, 7);

	int randomNum = 0;

	//generate initial population
	for (int j = 0; j < ChromoSize; j++)
	{
		randomNum = distribution(generator);

		genes.push_back(Direction(randomNum));

	}

}

void Chromosome::move(Map _tileMap, bool *_end, int *_endPath)
{
	for (int j = 0; j < ChromoSize; j++)
	{
		//validation for if wall or if corner

		switch (genes[j])
		{
		case Up:
			if (validMove(Up, _tileMap)) 
			{
				endPosition.yPos--;
			}
			break;

		case Down:
			if (validMove(Down, _tileMap))
			{
				endPosition.yPos++;
			}
			break;

		case Right:
			if (validMove(Right, _tileMap))
			{
				endPosition.xPos++;
			}
			break;

		case Left:
			if (validMove(Left, _tileMap))
			{
				endPosition.xPos--;
			}
			break;

		case Up_Right:
			if (validMove(Up_Right, _tileMap))
			{
				endPosition.xPos++;
				endPosition.yPos--;
			}
			break;

		case Up_Left:
			if (validMove(Up_Left, _tileMap))
			{
				endPosition.xPos--;
				endPosition.yPos--;
			}
			break;

		case Down_Right:
			if (validMove(Down_Right, _tileMap))
			{
				endPosition.xPos++;
				endPosition.yPos++;
			}
			break;

		case Down_Left:
			if (validMove(Down_Left, _tileMap))
			{
				endPosition.xPos--;
				endPosition.yPos++;
			}
			break;

		}

		if (endPosition.xPos == _tileMap.finish.xPos && endPosition.yPos == _tileMap.finish.yPos)
		{
			*_end = true;
			//set the index at which the path reached the Finish Node
			*_endPath = j;

			return;
		}
	}
}

bool Chromosome::validMove(Direction _dir, Map _tileMap)
{
	bool valid = false;

	switch (_dir)
	{
	case Up:
		if ((endPosition.yPos != 0) && (_tileMap.map[endPosition.yPos - 1][endPosition.xPos] != 1))
		{
			valid = true;
		}

		break;

	case Down:
		if ((endPosition.yPos != (_tileMap.height - 1)) && (_tileMap.map[endPosition.yPos + 1][endPosition.xPos] != 1))
		{
			valid = true;
		}
		break;

	case Right:
		if ((endPosition.xPos != (_tileMap.width - 1)) && (_tileMap.map[endPosition.yPos][endPosition.xPos + 1] != 1))
		{
			valid = true;
		}
		break;

	case Left:
		if ((endPosition.xPos != 0) && (_tileMap.map[endPosition.yPos][endPosition.xPos - 1] != 1))
		{
			valid = true;
		}
		break;

	case Up_Right:
		if ((endPosition.yPos != 0 && (endPosition.xPos != (_tileMap.width - 1))) && (_tileMap.map[endPosition.yPos - 1][endPosition.xPos + 1] != 1))
		{
			valid = true;
		}
		break;

	case Up_Left:
		if ((endPosition.yPos != 0 && endPosition.xPos != 0) && (_tileMap.map[endPosition.yPos - 1][endPosition.xPos - 1] != 1))
		{
			valid = true;
		}
		break;

	case Down_Right:
		if (((endPosition.yPos != (_tileMap.height - 1)) && (endPosition.xPos != (_tileMap.width - 1))) && (_tileMap.map[endPosition.yPos + 1][endPosition.xPos + 1] != 1))
		{
			valid = true;
		}
		break;

	case Down_Left:
		if (((endPosition.yPos != (_tileMap.height - 1)) && endPosition.xPos != 0) && (_tileMap.map[endPosition.yPos + 1][endPosition.xPos - 1] != 1))
		{
			valid = true;
		}
		break;

	}
	
	if (!valid)
	{
		error++;
	}

	return valid;
}

void Chromosome::calcFitness(Vec2 _finish)
{
	float dX = (endPosition.xPos - _finish.xPos);
	float dY = (endPosition.yPos - _finish.yPos);

	//Euclidean distance and adding an error for everytime it moved invalidly
	fitness = 1.0 / (sqrt((dX*dX + dY * dY)) + error + 1.0);

}

void Chromosome::calcFitnessPerc(double _total)
{
	fitnessPercentage = (fitness / _total) * 100;
}

std::vector<Node> Chromosome::createPath(Map _tileMap, int _endPathIndex)
{
	std::vector<Node> path;
	Node currentNode;
	currentNode.parentPos = _tileMap.start;
	currentNode.position = _tileMap.start;

	for (int i = 0; i <=_endPathIndex; i++)
	{
		if (currentNode.validNode(genes[i], _tileMap))
		{
			switch (genes[i])
			{
			case Up:
				currentNode.position.yPos = currentNode.parentPos.yPos - 1;
				break;
			case Down:
				currentNode.position.yPos = currentNode.parentPos.yPos + 1;
				break;
			case Right:
				currentNode.position.xPos = currentNode.parentPos.xPos + 1;
				break;
			case Left:
				currentNode.position.xPos = currentNode.parentPos.xPos - 1;
				break;
			case Up_Right:
				currentNode.position.yPos = currentNode.parentPos.yPos - 1;
				currentNode.position.xPos = currentNode.parentPos.xPos + 1;
				break;
			case Up_Left:
				currentNode.position.yPos = currentNode.parentPos.yPos - 1;
				currentNode.position.xPos = currentNode.parentPos.xPos - 1;
				break;
			case Down_Right:
				currentNode.position.yPos = currentNode.parentPos.yPos + 1;
				currentNode.position.xPos = currentNode.parentPos.xPos + 1;
				break;
			case Down_Left:
				currentNode.position.yPos = currentNode.parentPos.yPos + 1;
				currentNode.position.xPos = currentNode.parentPos.xPos - 1;
				break;
			}

			path.push_back(currentNode);
			currentNode.parentPos = currentNode.position;

		}
	}

	return path;
}

