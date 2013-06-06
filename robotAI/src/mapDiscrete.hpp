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
#include "geometricPlane.hpp"

using namespace std;
using namespace boost;


typedef multi_array<float, 2> tyMapArray;


class MapDiscrete : MapGeneric{
	unsigned int mapSizeX;
	unsigned int mapSizeY;

	int mapSizeXmm;
	int mapSizeYmm;

	tyMapArray *map;

public:
	vector<int> *getIntersections(tyPolygon *poly, int xCoordMin, int xCoordMax, int yCoord);
	void fillMapPolygon(tyPolygon * poly, float factor);

	void convertRealToMap(int realXmm, int realYmm, int *x, int *y);

public:
	MapDiscrete(unsigned int mapSizeX, unsigned int mapSizeY, int mapSizeXmm, int mapSizeYmm);
	virtual ~MapDiscrete(){}

	unsigned int getSizeX(){ return mapSizeX; }
	unsigned int getSizeY(){ return mapSizeY; }

	float getValueXY(int x, int y);

	void setValueXY(int x, int y, float val);

	virtual void updateMap(tyPolygon * safeArea, tyPolygon * wallArea);

	int getMapSizeXmm() const {
		return mapSizeXmm;
	}

	int getMapSizeYmm() const {
		return mapSizeYmm;
	}
};


#endif /* MAPDISCRETE_HPP_ */
