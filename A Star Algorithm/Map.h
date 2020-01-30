#ifndef _MAP_
#define _MAP_

#include <vector>
#include "Vec2.h"


struct Map
{
	//vector representing the 2D map
	std::vector<std::vector<int>> map;

	int height = 0;
	int width = 0;

	Vec2 start = { 0, 0 };
	Vec2 finish = { 0, 0 };

};


#endif 

