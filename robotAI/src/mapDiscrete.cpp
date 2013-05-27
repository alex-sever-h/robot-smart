/*
 * mapDiscrete.cpp
 *
 *  Created on: May 26, 2013
 *      Author: alseh
 */

#include "mapDiscrete.hpp"



MapDiscrete::MapDiscrete(unsigned int mapSizeX, unsigned int mapSizeY)
{
	this->mapSizeX = mapSizeX;
	this->mapSizeY = mapSizeY;

	map = new tyMapArray(boost::extents[mapSizeX][mapSizeY]);
}
