/*
 * MapParticle.cpp
 *
 *  Created on: Jun 6, 2013
 *      Author: alseh
 */

#include <iostream>
#include "MapParticle.hpp"
#include "geometricPlane.hpp"

#define NEW_PARTICLE_FACTOR 1
#define CLEAR_SAFE_FACTOR   0.1
#define REMOVE_FACTOR		 0.1

MapParticle::MapParticle()
{
	particleList = new vector<LocationWWeight>;

	distribution = new DistNormalFloat(0, 10);
	gGenWall = new gaussGenerator(rEngine,*distribution);
}

void MapParticle::updateMap(tyPolygon* safeArea, tyPolygon* wallArea) {
	fillWallArea(wallArea);
	clearSafeArea(safeArea);
}

void MapParticle::fillWallArea(tyPolygon* wallArea) {
	for(tyPolygon::iterator it = wallArea->begin(); it != wallArea->end(); ++it)
	{
		for(int i = 0; i < 10; ++i)
		{
			int x = it->x + (*gGenWall)();
			int y = it->y + (*gGenWall)();
			particleList->push_back(LocationWWeight(x, y, NEW_PARTICLE_FACTOR ));
			//			cout << x << " x " << y << endl;
		}
	}
}

void MapParticle::clearSafeArea(tyPolygon* wallArea) {

	for(vector<LocationWWeight>::iterator it = particleList->begin(); it != particleList->end();)
	{
		if(insidePolygon2D(*it, *wallArea))
		{
			it->weight -= CLEAR_SAFE_FACTOR;
		}

		if(it->weight < REMOVE_FACTOR)
			it = particleList->erase(it);
		else
			++it;
	}
}
