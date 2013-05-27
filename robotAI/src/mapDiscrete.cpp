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

	for(unsigned int ix = 0; ix < this->getSizeX(); ++ix)
		for(unsigned int iy = 0; iy < this->getSizeY(); ++iy)
		{
			if(iy > 10 && iy < 50)
				(*map)[ix][iy] = 0.9;
			else
				(*map)[ix][iy] = 0.5;
		}
}
