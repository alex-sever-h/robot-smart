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

#define NEW_PARTICLE_FACTOR 0.1f
#define CLEAR_SAFE_FACTOR   1
#define REMOVE_FACTOR		 0.4

#define MINIMUM_OBSTACLE_SIZE 20
#define WALL_PARTICLE_GENERATION_NUMBER 20
#define SAFE_PARTICLE_GENERATION_NUMBER 30

MapParticle::MapParticle()
{
	safeParticleList = new list<LocationWWeight>;
	wallParticleList = new list<LocationWWeight>;

	distribution = new DistNormalFloat(0, 0.5);
	gaussianGenerator = new gaussGenerator(rEngine, *distribution);
}
//
//void MapParticle::updateMap(tyPolygon* safeArea, tyPolygon* wallArea)
//{
//	if (wallArea)
//		fillWallArea(wallArea);
//	if (safeArea)
//		clearSafeArea(safeArea);
//}

void MapParticle::updateMap(tySensor* sensor)
{
	if(sensor)
	{
		fillSafeArea(sensor);
		fillWallArea(sensor);
	}
}

//void MapParticle::fillWallArea(tyPolygon* wallArea)
//{
//	for (tyPolygon::iterator it = wallArea->begin() + 1;
//			it != wallArea->end() - 1; ++it)
//	{
//		int n_proximity = 0;
//
//		for (list<LocationWWeight>::iterator it_particle =
//				safeParticleList->begin(); it_particle != safeParticleList->end();
//				++it_particle)
//		{
//			if (euclidDistance(*it_particle, *it) < 20)
//				n_proximity++;
//		}
//		if (n_proximity > 10)
//			continue;
//
//		// create particles gaussian distributed
//		for (int i = 0; i < 10; ++i)
//		{
//			int x = it->x + (*gaussianGenerator)();
//			int y = it->y + (*gaussianGenerator)();
//			safeParticleList->push_back(LocationWWeight(x, y, NEW_PARTICLE_FACTOR));
//		}
//	}
//}


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

void MapParticle::generateVariations(float& length, float& theta)
{
	do
	{
		length = fabs((*gaussianGenerator)());
	} while (length > 1);
	do
	{
		theta = (*gaussianGenerator)();
	} while (theta > 1 || theta < -1);
}


bool cmpX(const LocationWWeight &a, const LocationWWeight &b)
{
	return a.x < b.x;
}


bool cmpY(const LocationWWeight& a, const LocationWWeight& b)
{
	return a.y < b.y;
}


void MapParticle::fillWallArea(tySensor* sensor)
{

	wallParticleGuard.lock();
	clearSafeArea(sensor->polySafe);

	for(int i = 0; i < WALL_PARTICLE_GENERATION_NUMBER; i++)
	{
		float theta;
		float length;

		generateVariations(length, theta);

		//apply variation
		length = sensor->distanceMM + length * MINIMUM_OBSTACLE_SIZE;
		theta = theta * sensor->angleSpan/2;

		float x = sensor->offsetXmapMM + length * cos(sensor->angleCenterRadMap + theta);
		float y = sensor->offsetYmapMM + length * sin(sensor->angleCenterRadMap + theta);

		wallParticleList->push_back(LocationWWeight(x, y, NEW_PARTICLE_FACTOR));
	}

	wallParticleList->sort(cmpX);
	minimizeSortedList(wallParticleList);
	wallParticleList->sort(cmpX);

	wallParticleGuard.unlock();
}

void MapParticle::fillSafeArea(tySensor* sensor)
{
	safeParticleGuard.lock();
	for(int i = 0; i < SAFE_PARTICLE_GENERATION_NUMBER; i++)
	{
		float theta;
		float length;

		//generate gaussian between 0 and 1
		generateVariations(length, theta);

		//apply variation
		length = length * sensor->distanceMM;
		theta = theta * sensor->angleSpan/2;

		float x = sensor->offsetXmapMM + length * cos(sensor->angleCenterRadMap + theta);
		float y = sensor->offsetYmapMM + length * sin(sensor->angleCenterRadMap + theta);

		safeParticleList->push_back(LocationWWeight(x, y, NEW_PARTICLE_FACTOR));
	}

	safeParticleList->sort(cmpX);
	minimizeSortedList(safeParticleList);
	safeParticleList->sort(cmpX);

	safeParticleGuard.unlock();
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


bool MapParticle::compareByX(const LocationWWeight &a, const LocationWWeight &b)
{
	return a.x < b.x;
}

bool MapParticle::compareByY(const LocationWWeight& a, const LocationWWeight& b)
{
	return a.y < b.y;
}

#define MINIMIZATION_DISTANCE 5

void MapParticle::minimizeSortedList(list<LocationWWeight>* particleList)
{
	for (list<LocationWWeight>::iterator it1 = particleList->begin();
			it1 != particleList->end();it1++)
	{
		for (list<LocationWWeight>::iterator it2 = it1;
				it2 != particleList->end() && it1->x - it2->x < MINIMIZATION_DISTANCE;)
		{
			if (it1 != it2 && euclidDistance(*it1, *it2) < MINIMIZATION_DISTANCE)
			{
				it1->x = (it1->x + it2->x) / 2;
				it1->y = (it1->y + it2->y) / 2;
				it1->weight = (it1->weight + it2->weight);
				it2 = particleList->erase(it2);
				break;
			}
			else
				++it2;
		}


	}
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















