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
#define CLEAR_SAFE_FACTOR   0.4
#define REMOVE_FACTOR		 0.4

MapParticle::MapParticle()
{
	particleList = new list<LocationWWeight>;

	distribution = new DistNormalFloat(0, 20);
	gGenWall = new gaussGenerator(rEngine,*distribution);
}

void MapParticle::updateMap(tyPolygon* safeArea, tyPolygon* wallArea) {
	fillWallArea(wallArea);
	clearSafeArea(safeArea);
}

void MapParticle::fillWallArea(tyPolygon* wallArea) {
	for(tyPolygon::iterator it = wallArea->begin(); it != wallArea->end(); ++it)
	{
		int n_proximity = 0;

		for(list<LocationWWeight>::iterator it_particle = particleList->begin(); it_particle != particleList->end();++it_particle)
		{
			if(euclidDistance(*it_particle, *it) < 20)
				n_proximity++;
		}
		if(n_proximity > 10)
			continue;

		// create particles gaussian distributed
		for(int i = 0; i < 10; ++i)
		{
			int x = it->x + (*gGenWall)();
			int y = it->y + (*gGenWall)();
			particleList->push_back(LocationWWeight(x, y, NEW_PARTICLE_FACTOR ));
		}
	}
}

void MapParticle::clearSafeArea(tyPolygon* wallArea) {

	for(list<LocationWWeight>::iterator it = particleList->begin(); it != particleList->end();)
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

float MapParticle::computeCollisionFactor(tyPolygon* area) {
	float colFactor = 0;

	for(list<LocationWWeight>::iterator it = particleList->begin(); it != particleList->end(); it++)
		if(insidePolygon2D(*it, *area))
		{
			colFactor += it->weight;
		}

	return colFactor;
}






