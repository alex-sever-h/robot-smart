/*
 * MapParticle.hpp
 *
 *  Created on: Jun 6, 2013
 *      Author: alseh
 */

#ifndef MAPPARTICLE_HPP_
#define MAPPARTICLE_HPP_

#include <boost/random/mersenne_twister.hpp>
#include <boost/random/normal_distribution.hpp>
#include <boost/random/variate_generator.hpp>

#include <boost/thread.hpp>

#include "RobotServer.hpp"
#include "MapGeneric.hpp"
#include "sensorManagement.h"
#include <vector>
#include <list>
#include "protobuf/robotdata.pb.h"

using namespace std;
using namespace boost;


typedef mt19937                                     REngine;          // Mersenne Twister
typedef normal_distribution<float>                 DistNormalFloat;  // Normal Distribution
typedef variate_generator<REngine,DistNormalFloat>  gaussGenerator;              // Variate generator

class tySensor;

class MapParticle : MapGeneric{
	list<LocationWWeight> *safeParticleList;
	list<LocationWWeight> *wallParticleList;

	mutex safeParticleGuard;
	mutex wallParticleGuard;

	REngine         rEngine;
	DistNormalFloat *distribution;
	gaussGenerator  *gaussianGenerator;

	void generateVariations(float& length, float& theta);
	void minimizeSortedList(list<LocationWWeight> *particleList);

public:

	bool compareByX(const LocationWWeight &a, const LocationWWeight &b);
	bool compareByY(const LocationWWeight &a, const LocationWWeight &b);

public:
	MapParticle();
	virtual ~MapParticle(){}

//	virtual void updateMap(tyPolygon *safeArea, tyPolygon *wallArea);
//	void fillWallArea(tyPolygon *wallArea);
	void clearSafeArea(tyPolygon *wallArea);

	virtual void updateMap(tySensor *sensor);
	void fillWallArea(tySensor *sensor);
	void fillSafeArea(tySensor *sensor);

	float computeCollisionFactor(tyPolygon *area);

	void clearMap(void);

	google::protobuf::Message * sendFullMap();

	list<LocationWWeight>* getWallParticleList() const
	{
		return wallParticleList;
	}

	const list<LocationWWeight>* getSafeParticleList() const
	{
		return safeParticleList;
	}
};



#endif /* MAPPARTICLE_HPP_ */
