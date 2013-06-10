/*
 * Location.hpp
 *
 *  Created on: May 28, 2013
 *      Author: alseh
 */

#ifndef LOCATION_HPP_
#define LOCATION_HPP_

class Location{
public:
	float x;
	float y;
	float theta;

	Location()
	{
		this->x = 0;
		this->y = 0;
		this->theta = 0;
	}

	Location(float x, float y, float t)
	{
		this->x = x;
		this->y = y;
		this->theta = t;
	}
};


#endif /* LOCATION_HPP_ */
