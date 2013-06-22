/*
 * MapParticle.cpp
 *
 *  Created on: Jun 6, 2013
 *      Author: alseh
 */

#include <iostream>
#include "MapParticle.hpp"
#include "geometricPlane.hpp"
#include "protobuf/robotdata.pb.h"

#define NEW_PARTICLE_FACTOR 1
#define CLEAR_SAFE_FACTOR   1
#define REMOVE_FACTOR		 0.4

MapParticle::MapParticle()
{
	safeParticleList = new list<LocationWWeight>;
	wallParticleList = new list<LocationWWeight>;

	distribution = new DistNormalFloat(0, 20);
	gGenWall = new gaussGenerator(rEngine, *distribution);
}

void MapParticle::updateMap(tyPolygon* safeArea, tyPolygon* wallArea)
{
	if (wallArea)
		fillWallArea(wallArea);
	if (safeArea)
		clearSafeArea(safeArea);
}

void MapParticle::updateMap(tySensor* sensor)
{
	if(sensor)
	{
		fillWallArea(sensor);
		fillSafeArea(sensor);
	}
}

void MapParticle::fillWallArea(tyPolygon* wallArea)
{
	for (tyPolygon::iterator it = wallArea->begin() + 1;
			it != wallArea->end() - 1; ++it)
	{
		int n_proximity = 0;

		for (list<LocationWWeight>::iterator it_particle =
				safeParticleList->begin(); it_particle != safeParticleList->end();
				++it_particle)
		{
			if (euclidDistance(*it_particle, *it) < 20)
				n_proximity++;
		}
		if (n_proximity > 10)
			continue;

		// create particles gaussian distributed
		for (int i = 0; i < 10; ++i)
		{
			int x = it->x + (*gGenWall)();
			int y = it->y + (*gGenWall)();
			safeParticleList->push_back(LocationWWeight(x, y, NEW_PARTICLE_FACTOR));
		}
	}
}

void MapParticle::clearSafeArea(tyPolygon* wallArea)
{
	for (list<LocationWWeight>::iterator it = wallParticleList->begin();
			it != wallParticleList->end();)
	{
		if (insidePolygon2D(*it, *wallArea))
			it = wallParticleList->erase(it);
		else
			++it;
	}
}

void MapParticle::fillWallArea(tySensor* sensor)
{
	DistNormalFloat *distributionDistance = new DistNormalFloat(0, sensor->distanceMM);
	gaussGenerator *gaussGeneratorDistance = new gaussGenerator(rEngine, *distributionDistance);

	DistNormalFloat *distributionTheta = new DistNormalFloat(0, sensor->angleSpan);
	gaussGenerator *gaussGeneratorTheta = new gaussGenerator(rEngine, *distributionTheta);

	//	for(int i = 0; )
}

void MapParticle::fillSafeArea(tySensor* sensor)
{
	DistNormalFloat *distributionDistance = new DistNormalFloat(0, sensor->distanceMM/2);
	gaussGenerator *gaussGeneratorDistance = new gaussGenerator(rEngine, *distributionDistance);

	DistNormalFloat *distributionTheta = new DistNormalFloat(0, sensor->angleSpan/3);
	gaussGenerator *gaussGeneratorTheta = new gaussGenerator(rEngine, *distributionTheta);

	safeParticleGuard.lock();
	for(int i = 0; i < (int)sensor->distanceMM*sensor->angleSpan/20; i++)
	{
		float theta;
		float length;

		do{
			length = fabs((*gaussGeneratorDistance)());
		}while(length > sensor->distanceMM);

		do{
			theta = (*gaussGeneratorTheta)();
		}while(theta > sensor->angleSpan/2 || theta < - sensor->angleSpan/2);

		float x = sensor->offsetXmapMM + length * cos(sensor->angleCenterRadMap + theta);
		float y = sensor->offsetYmapMM + length * sin(sensor->angleCenterRadMap + theta);

		safeParticleList->push_back(LocationWWeight(x, y, (float)(4000 - sensor->distanceMM) / 100));
	}
	safeParticleGuard.unlock();

	wallParticleGuard.lock();
	clearSafeArea(sensor->polySafe);

	for(int i = 0; i < (int)sensor->distanceMM*sensor->angleSpan/100; i++)
	{
		float theta;
		float length;

		do{
			length = fabs((*gaussGeneratorDistance)());
		}while(length < sensor->distanceMM || length > sensor->distanceMM + 50);

		do{
			theta = (*gaussGeneratorTheta)();
		}while(theta > sensor->angleSpan/2 || theta < - sensor->angleSpan/2);

		float x = sensor->offsetXmapMM + length * cos(sensor->angleCenterRadMap + theta);
		float y = sensor->offsetYmapMM + length * sin(sensor->angleCenterRadMap + theta);

		wallParticleList->push_back(LocationWWeight(x, y, (float)(4000 - sensor->distanceMM) / 100));
	}
	wallParticleGuard.unlock();
}

float MapParticle::computeCollisionFactor(tyPolygon* area)
{
	float colFactor = 0;

	wallParticleGuard.lock();
	for (list<LocationWWeight>::iterator it = wallParticleList->begin();
			it != wallParticleList->end(); it++)
		if (insidePolygon2D(*it, *area))
		{
			colFactor += it->weight;
		}
	wallParticleGuard.unlock();

	return colFactor;
}

void MapParticle::clearMap(void)
{
	safeParticleList->clear();
	wallParticleList->clear();
}

google::protobuf::Message* MapParticle::sendFullMap()
{
	robotdata::FullMap *fullMap = new robotdata::FullMap();

	//add wall particles
	for (list<LocationWWeight>::iterator it = wallParticleList->begin();
			it != wallParticleList->end();
			it++)
	{
		robotdata::FullMap_MapParticle *helper = fullMap->add_wallparticles();
		helper->set_posx(it->x);
		helper->set_posy(it->y);
		helper->set_weight(it->weight);
	}

	//add safe particles
	for (list<LocationWWeight>::iterator it = safeParticleList->begin();
			it != safeParticleList->end();
			it++)
	{
		robotdata::FullMap_MapParticle *helper = fullMap->add_safeparticles();
		helper->set_posx(it->x);
		helper->set_posy(it->y);
		helper->set_weight(it->weight);
	}

	return fullMap;
}















