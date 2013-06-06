/*
 * MapGeneric.hpp
 *
 *  Created on: May 26, 2013
 *      Author: alseh
 */

#ifndef MAPGENERIC_HPP_
#define MAPGENERIC_HPP_

#include "geometricPlane.hpp"

class MapGeneric{

public:
	MapGeneric(){}
	virtual ~MapGeneric(){}

	virtual void updateMap(tyPolygon * safeArea, tyPolygon * wallArea) = 0;
};


#endif /* MAPGENERIC_HPP_ */
