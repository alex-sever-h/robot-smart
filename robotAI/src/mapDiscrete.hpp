/*
 * mapDiscrete.hpp
 *
 *  Created on: May 26, 2013
 *      Author: alseh
 */

#ifndef MAPDISCRETE_HPP_
#define MAPDISCRETE_HPP_

#include <boost/multi_array.hpp>
#include "locationWWeight.hpp"
#include "MapGeneric.hpp"

using namespace std;
using namespace boost;



typedef multi_array<double, 2> tyMapArray;
//typedef tyMapArray::index index;


class MapDiscrete : MapGeneric{
	unsigned int mapSizeX;
	unsigned int mapSizeY;
	tyMapArray *map;

public:
	MapDiscrete(unsigned int mapSizeX, unsigned int mapSizeY);
	virtual ~MapDiscrete(){}
};


#endif /* MAPDISCRETE_HPP_ */
