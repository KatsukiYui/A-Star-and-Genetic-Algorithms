#ifndef _CHROMO_
#define _CHROMO_

#include <random>
#include <cmath>
#include "Vec2.h"
#include "Node.h"

#define ChromoSize 80
#define NumOfChromosomes 8
#define crossoverRate 45
#define mutationRate 0.01

struct Chromosome
{

	std::vector<Direction> genes;

	Vec2 endPosition = { 0,0 };

	double fitness = 0;

	float fitnessPercentage = 0;

	//incremented every time an invalid move is made
	int error = 0;
	
	//generates initial population
	void generateGenes(Map _tileMap);

	//calculates the end position of the chromosome after it follows the directions
	void move(Map _tileMap, bool *_end, int *_endPath);

	//validation for movement. Returns fault if the chromosome hits
	//a wall of goes out of map bounds
	bool validMove(Direction _dir, Map _tileMap);

	//calculates fitness using Euclidean distance and an error
	void calcFitness(Vec2 _finish);

	//total fitness/fitness * 100 
	void calcFitnessPerc(double _total);

	//create and return a path from the directions
	std::vector<Node> createPath(Map _tileMap, int _endPathIndex);
};


#endif
