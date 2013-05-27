/*
 * locationWWeight.hpp
 *
 *  Created on: May 26, 2013
 *      Author: alseh
 */

#ifndef LOCATIONWWEIGHT_HPP_
#define LOCATIONWWEIGHT_HPP_

class LocationWWeight{
public:
	int x;
	int y;
	int z;
	float weight;

public:
	LocationWWeight(int x, int y, float w)
	{
		this->x = x;
		this->y = y;
		this->z = 0;
		this->weight = w;
	}
};


#endif /* LOCATIONWEIGHT_HPP_ */
