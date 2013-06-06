/*
 * mapVector.hpp
 *
 *  Created on: May 26, 2013
 *      Author: alseh
 */

#ifndef MAPVECTORIAL_HPP_
#define MAPVECTORIAL_HPP_

#include <vector>
#include "locationWWeight.hpp"

using namespace std;


class MapVectorial : MapGeneric{
	vector<vector<LocationWWeight>> polygon;

public:
	MapVectorial();
	virtual ~MapVectorial();

	virtual void updateMap(tyPolygon * safeArea, tyPolygon * wallArea);
};


#endif /* MAPVECTOR_HPP_ */
