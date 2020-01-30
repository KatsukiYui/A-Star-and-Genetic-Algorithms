#include <SDL.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <ctime>
#include "Chromosome.h"

#define TILESIZE 50 //tile size used to draw the map

//opens the chosen file and reads the map onto a vector in the Map struct
bool initMap(Map *_tileMap);

//draws the map using SDL rectangles
void drawMap(Map _tileMap, SDL_Renderer *_renderer);

//finds the shortest path from start to finish node using A* Algorithm
std::vector <Node> findPathAStar(Map _tileMap);

//finds a path from start to finish node using Genetic Algorithm
std::vector <Node> findPathGA(Map _tileMap);

//draws the found path using SDL_RenderDrawLine
void drawPath(std::vector<Node> _path, Map _tileMap, SDL_Renderer *_renderer, bool *_end, bool _aStar);


int main(int argc, char *argv[])
{
	//initialise SDL
	SDL_Init(SDL_INIT_VIDEO);

	SDL_Window *window;
	SDL_Renderer *renderer;


	//used to store the 2D vector
	Map tileMap;

	//event polling
	SDL_Event event;
	SDL_PollEvent(&event);
	bool end = false;
	bool exit = false;
	std::string algorithm = "";
	std::vector<Node> path;
	bool aStar = false;
	int choice = 0;


	do
	{
		//choose and initialise the map
		while (!initMap(&tileMap)) {};

		while (algorithm != "1" && algorithm != "2" && algorithm != "3")
		{
			std::cout << std::endl << "Choose An Algorithm (Enter a Number):" << std::endl << "1: A* Algorithm" << std::endl <<
				"2: Genetic Algorithm" << std::endl << "3: Exit" << std::endl;
			std::getline(std::cin, algorithm);
		}

		std::stringstream alg(algorithm);

		alg >> choice;

		switch (choice)
		{
		case 1:
			algorithm = "A* Algorithm";
			path = findPathAStar(tileMap);
			aStar = true;
			break;
		case 2:
			algorithm = "Genetic Algorithm";
			path = findPathGA(tileMap);
			aStar = false;
			break;
		case 3:
			exit = true;
			break;
		}

		window = SDL_CreateWindow(algorithm.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 800, 0);
		renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

		
		do 
		{
			SDL_PollEvent(&event);

			//draw the map usin sdl squares
			drawMap(tileMap, renderer);
			//draw the path using sdl lines
			drawPath(path, tileMap, renderer, &end, aStar);


		} while (event.type != SDL_QUIT && !end && !exit);

		//clear memory.. reset variables
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		algorithm = "";
		end = false;
		aStar = false;
		choice = 0;
		tileMap.map.clear();
		path.clear();
		algorithm.clear();

	} while ((!exit));

	SDL_Quit();

	return 0;
}

bool initMap(Map *_tileMap)
{
	std::string fileName = "";

	std::cout << "Enter the Name of the Map File" << std::endl;
	std::getline(std::cin, fileName);
	fileName.append(".txt");

	//Open the file for reading
	std::ifstream mapFile(fileName.c_str());
	
	if (mapFile.is_open()) 
	{

		std::string currentLine;

		//Read from file
		while (std::getline(mapFile, currentLine))
		{

			std::stringstream stringMap(currentLine);

			if (!(stringMap >>(*_tileMap).width))
			{
				std::cout << "error reading width" << std::endl;
			}

			if (!(stringMap >> (*_tileMap).height))
			{
				std::cout << "error reading height" << std::endl;
			}

			int currentTile = 5; //used to extract tiles one by one

			std::vector<int> temp;

			for (int i = 0; i < (*_tileMap).height; i++)
			{

				for (int j = 0; j < (*_tileMap).width; j++)
				{

					if ((stringMap >> currentTile))
					{
						temp.push_back(currentTile);

						switch (currentTile)
						{
						case 0:
							break;

						case 1:
							break;

						case 2:
							(*_tileMap).start = { j , i };
							break;

						case 3:
							(*_tileMap).finish = { j , i };
							break;

						default:
							std::cout << "error: unidentified tile" << std::endl;
							break;
						}

					}
					else
					{
						std::cout << "error reading map tiles" << std::endl;

					}
				}

				(*_tileMap).map.push_back(temp);
				temp.clear();
			}

			/*
			//print vector for testing
			for (int i = 0; i < (*_tileMap).height; i++)
			{

				for (int j = 0; j < (*_tileMap).width; j++)
				{
					std::cout << (*_tileMap).map[i][j];
				}
				std::cout << std::endl;
			}
			
			*/

			mapFile.close();
			fileName = "";
			return true;

		}

	}
	else
	{
		std::cout << "File couldn't be opened. Please enter a valid file name" << std::endl;
		fileName = "";
		mapFile.close();
		return false;
	}
}

void drawMap(Map _tileMap, SDL_Renderer *_renderer)
{

	SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 255);
	SDL_RenderClear(_renderer);

	SDL_Rect tile;
	tile.w = TILESIZE;
	tile.h = TILESIZE;


	for (int i = 0; i < _tileMap.height; i++)
	{

		for (int j = 0; j < _tileMap.width; j++)
		{
			//outline the map
			tile.x = j * TILESIZE;
			tile.y = i * TILESIZE;
			SDL_SetRenderDrawColor(_renderer, 255, 255, 255, 100);
			SDL_RenderDrawRect(_renderer, &tile);

			switch (_tileMap.map[i][j])
			{
			case 0:
				break;

			//draw the walls
			case 1:
				tile.x = j * TILESIZE;
				tile.y = i * TILESIZE;
				SDL_SetRenderDrawColor(_renderer, 101, 67, 33, 255);
				SDL_RenderDrawRect(_renderer, &tile);
				SDL_RenderFillRect(_renderer, &tile);
				break;

			//draw the Start Position
			case 2:
				tile.x = j * TILESIZE;
				tile.y = i * TILESIZE;
				SDL_SetRenderDrawColor(_renderer, 50, 50, 200, 255);
				SDL_RenderDrawRect(_renderer, &tile);
				SDL_RenderFillRect(_renderer, &tile);
				break;

			//draw the Finish Position
			case 3:
				tile.x = j * TILESIZE;
				tile.y = i * TILESIZE;
				SDL_SetRenderDrawColor(_renderer, 200, 150, 0, 255);
				SDL_RenderDrawRect(_renderer, &tile);
				SDL_RenderFillRect(_renderer, &tile);
				break;

			}


		}
	}

	SDL_RenderPresent(_renderer);

}

//A*
std::vector <Node> findPathAStar(Map _tileMap)
{
	//clock_t t;

	std::vector<Node> closedList;
	std::vector<Node> openList;
	
	Node temp;
	Node currentNode;

	//t = clock();

	currentNode.position = _tileMap.start;

	do  
	{
		//push the current node onto closed list
		if ((currentNode.isInClosedList(_tileMap, closedList)))
		{
			closedList.push_back(currentNode);
		}

		//up
		if ((currentNode.validNode(Up, _tileMap)))
		{
			if (currentNode.availableNode(Up, _tileMap, openList, closedList))
			{
				temp.position.yPos = currentNode.position.yPos - 1;
				temp.position.xPos = currentNode.position.xPos;
				temp.parentPos = currentNode.position;

				temp.gCost = currentNode.gCost + 1;

				//add to Open list
				openList.push_back(temp);
			}

		}

		//down
		if ((currentNode.validNode(Down, _tileMap)))
		{
			if (currentNode.availableNode(Down, _tileMap, openList, closedList))
			{
				temp.position.yPos = currentNode.position.yPos + 1;
				temp.position.xPos = currentNode.position.xPos;
				temp.parentPos = currentNode.position;

				temp.gCost = currentNode.gCost + 1;

				//add to Open list
				openList.push_back(temp);
			}
		}

		//right
		if ((currentNode.validNode(Right, _tileMap)))
		{
			if (currentNode.availableNode(Right, _tileMap, openList, closedList))
			{
				temp.position.yPos = currentNode.position.yPos;
				temp.position.xPos = currentNode.position.xPos + 1;
				temp.parentPos = currentNode.position;

				temp.gCost = currentNode.gCost + 1;

				//add to Open list
				openList.push_back(temp);
			}
		}

		//left
		if ((currentNode.validNode(Left, _tileMap)))
		{
			if (currentNode.availableNode(Left, _tileMap, openList, closedList))
			{
				temp.position.yPos = currentNode.position.yPos;
				temp.position.xPos = currentNode.position.xPos - 1;
				temp.parentPos = currentNode.position;

				temp.gCost = currentNode.gCost + 1;

				//add to Open list
				openList.push_back(temp);
			}
		}

		//Up-Right
		if ((currentNode.validNode(Up_Right, _tileMap)))
		{
			if (currentNode.availableNode(Up_Right, _tileMap, openList, closedList))
			{
				temp.position.yPos = currentNode.position.yPos - 1;
				temp.position.xPos = currentNode.position.xPos + 1;
				temp.parentPos = currentNode.position;

				temp.gCost = currentNode.gCost + 1.4;

				//add to Open list
				openList.push_back(temp);
			}
		}

		//Up-Left
		if ((currentNode.validNode(Up_Left, _tileMap)))
		{
			if (currentNode.availableNode(Up_Left, _tileMap, openList, closedList))
			{
				temp.position.yPos = currentNode.position.yPos - 1;
				temp.position.xPos = currentNode.position.xPos - 1;
				temp.parentPos = currentNode.position;

				temp.gCost = currentNode.gCost + 1.4;

				//add to Open list
				openList.push_back(temp);
			}
		}

		//Down-Right
		if ((currentNode.validNode(Down_Right, _tileMap)))
		{
			if (currentNode.availableNode(Down_Right, _tileMap, openList, closedList))
			{
				temp.position.yPos = currentNode.position.yPos + 1;
				temp.position.xPos = currentNode.position.xPos + 1;
				temp.parentPos = currentNode.position;

				temp.gCost = currentNode.gCost + 1.4;

				//add to Open list
				openList.push_back(temp);
			}
		}

		//Down-Left
		if ((currentNode.validNode(Down_Left, _tileMap)))
		{
			if (currentNode.availableNode(Down_Left, _tileMap, openList, closedList))
			{
				temp.position.yPos = currentNode.position.yPos + 1;
				temp.position.xPos = currentNode.position.xPos - 1;
				temp.parentPos = currentNode.position;

				temp.gCost = currentNode.gCost + 1.4;

				//add to Open list
				openList.push_back(temp);
			}
		}

		double currentLowest = 100000000;
		int lowestIndex = 0;

		//calculate F cost for openlist nodes and compare
		for (int i = 0; i < openList.size(); i++)
		{
			openList[i].calcHCost(_tileMap.finish);
			openList[i].fCost = openList[i].hCost + openList[i].gCost;

			if (openList[i].fCost < currentLowest)
			{
				currentLowest = openList[i].fCost;
				lowestIndex = i;
			}
		}


		//choose the node with lease Fcost
		temp = currentNode;
		currentNode.parentPos = openList[lowestIndex].parentPos;
		currentNode.position = openList[lowestIndex].position;
		currentNode.gCost = openList[lowestIndex].gCost;

		//erase prev current from openlist... if it is 
		for (int i = 0; i < openList.size(); i++)
		{
			//openList.erase(openList.begin() + lowestIndex);
			if ((openList[i].position.xPos == temp.position.xPos) && (openList[i].position.yPos == temp.position.yPos))
			{
				openList.erase(openList.begin() + i);
			}
		}


	} while (!openList.empty() && !((currentNode.position.xPos == _tileMap.finish.xPos) && (currentNode.position.yPos == _tileMap.finish.yPos)));

	if (openList.empty())
	{
		std::cout << "error finding the finish node" << std::endl;
	}

	//make the shortest path by following parent pointers
	std::vector <Node> shortestPath;

	//currentNode is the Finish Node
	while (!((_tileMap.start.xPos == currentNode.position.xPos) && (_tileMap.start.yPos == currentNode.position.yPos)))
	{
		for (int i = 0; i < closedList.size(); i++)
		{
			if ((closedList[i].position.xPos == currentNode.parentPos.xPos) && (closedList[i].position.yPos == currentNode.parentPos.yPos))
			{
				shortestPath.push_back(currentNode);

				currentNode = closedList[i];
			}

		}
	}

	//clear the vectors
	closedList.clear();
	openList.clear();

	//time calculation
	//t = clock() - t;
	//std::cout << "It took " << ((float)t) / CLOCKS_PER_SEC << " seconds to find the path." << std::endl;

	return shortestPath;
}

//GA
std::vector<Node> findPathGA(Map _tileMap)
{
	//clock_t t;

	srand(time(NULL));

	Chromosome population[NumOfChromosomes];

	double totalFitness = 0;

	//t = clock();

	for (int i = 0; i < NumOfChromosomes; i++)
	{
		//generate initial polulation
		population[i].generateGenes(_tileMap);
		population[i].endPosition = _tileMap.start;
	}

	bool end = false;
	int pathIndex;
	int endPathIndex;

	do {

		int i = 0;

		while (i < NumOfChromosomes && !end)
		{
			//move and calculate the end position of each chromosome
			population[i].move(_tileMap, &end, &endPathIndex);

			if (end)
			{
				pathIndex = i;
				
			}

			//calculate the fitness for every chromosome
			population[i].calcFitness(_tileMap.finish);

			totalFitness += population[i].fitness;

			i++;
		}

		//if a chromosome hasn't reached the end
		if (!end)
		{

			//Breed
			//calculate the percentage of fitness for each chromosome
			for (int i = 0; i < NumOfChromosomes; i++)
			{
				population[i].calcFitnessPerc(totalFitness);
			}

			int parents[NumOfChromosomes];
			double totalPerc = 0; //total of percentages so far
			float randNum = 0;

			//randomly select parents
			for (int j = 0; j < NumOfChromosomes; j++)
			{
				randNum = rand() % 100;

				for (int i = 0; i < NumOfChromosomes; i++)
				{

					if (randNum >= totalPerc && randNum < (population[i].fitnessPercentage + totalPerc))
					{
						parents[j] = i;

					}

					totalPerc += population[i].fitnessPercentage;
				}

				totalPerc = 0;
			}

			//breeding parents according to crossover Rate
			Chromosome offsprings[NumOfChromosomes];
			for (int i = 0; i < NumOfChromosomes; i++)
			{
				//initialise genes for offsprings
				offsprings[i].generateGenes(_tileMap);
			}

			int crossover;

			for (int k = 0; k < NumOfChromosomes; k = k + 2)
			{
				crossover = rand() % 100;

				if (crossover <= crossoverRate)
				{
					//copy first half
					for (int p = 0; p < ChromoSize / 2; p++)
					{
						offsprings[k].genes[p] = population[parents[k]].genes[p];
						offsprings[k + 1].genes[p] = population[parents[k + 1]].genes[p];
					}
					//copy second half
					for (int q = ChromoSize / 2; q < ChromoSize; q++)
					{
						offsprings[k].genes[q] = population[parents[k + 1]].genes[q];
						offsprings[k + 1].genes[q] = population[parents[k]].genes[q];
					}
				}

				else
				{
					//copy parent chromosomes
					for (int p = 0; p < ChromoSize; p++)
					{
						offsprings[k].genes[p] = population[parents[k]].genes[p];
						offsprings[k + 1].genes[p] = population[parents[k + 1]].genes[p];
					}
				}
			}


			//Mutate
			float mutation = 0.0;
			Direction randDir = Up;

			for (int i = 0; i < NumOfChromosomes; i++)
			{
				for (int j = 0; j < ChromoSize; j++)
				{
					mutation = ((double)rand() / (RAND_MAX));

					if (mutation <= mutationRate)
					{
						randDir = Direction(rand() % 8);
						offsprings[i].genes[j] = randDir;
					}
				}
			}

			//kill parents
			for (int i = 0; i < NumOfChromosomes; i++)
			{
				population[i].fitness = 0;
				population[i].fitnessPercentage = 0;
				population[i].error = 0;
				population[i].endPosition = _tileMap.start;

				for (int j = 0; j < ChromoSize; j++)
				{
					population[i].genes[j] = offsprings[i].genes[j];
					
				}

				offsprings[i].genes.clear();
			}
		}

	} while (!end);

	//time calculation
	//t = clock() - t;
	//std::cout << "It took " << ((float)t) / CLOCKS_PER_SEC << " seconds to find the path." << std::endl;

	//create and return a path from the directions
	return  population[pathIndex].createPath(_tileMap, endPathIndex);
}


void drawPath(std::vector<Node> _path, Map _tileMap, SDL_Renderer * _renderer, bool *_end, bool _aStar)
{
	//event polling
	SDL_Event event;
	SDL_PollEvent(&event);

	SDL_SetRenderDrawColor(_renderer, 255, 50, 200, 255);
	if (_aStar)
	{
		int i = (_path.size() - 1);

		while (i >= 0 && (event.type != SDL_QUIT))
		{
			SDL_PollEvent(&event);


			int x1 = (_path[i].parentPos.xPos + 1) * (TILESIZE)-(TILESIZE / 2);
			int y1 = (_path[i].parentPos.yPos + 1) * (TILESIZE)-(TILESIZE / 2);

			int x2 = (_path[i].position.xPos + 1) * (TILESIZE)-(TILESIZE / 2);
			int y2 = (_path[i].position.yPos + 1) * (TILESIZE)-(TILESIZE / 2);

			SDL_RenderDrawLine(_renderer, x1, y1, x2, y2);
			SDL_RenderPresent(_renderer);
			SDL_Delay(500);
			i--;

		}

	}
	else
	{
		int i = 0;

		while (i < (_path.size()) && (event.type != SDL_QUIT))
		{
			SDL_PollEvent(&event);


			int x1 = (_path[i].parentPos.xPos + 1) * (TILESIZE)-(TILESIZE / 2);
			int y1 = (_path[i].parentPos.yPos + 1) * (TILESIZE)-(TILESIZE / 2);

			int x2 = (_path[i].position.xPos + 1) * (TILESIZE)-(TILESIZE / 2);
			int y2 = (_path[i].position.yPos + 1) * (TILESIZE)-(TILESIZE / 2);

			SDL_RenderDrawLine(_renderer, x1, y1, x2, y2);
			SDL_RenderPresent(_renderer);
			SDL_Delay(500);
			i++;

		}
	}

	if (event.type == SDL_QUIT)
	{
		*_end = true;
	}

}


