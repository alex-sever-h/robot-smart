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

#include "MapGeneric.hpp"
#include <vector>

using namespace std;
using namespace boost;


typedef mt19937                                     REngine;          // Mersenne Twister
typedef normal_distribution<float>                 DistNormalFloat;  // Normal Distribution
typedef variate_generator<REngine,DistNormalFloat>  gaussGenerator;              // Variate generator


class MapParticle : MapGeneric{
	vector<LocationWWeight> *particleList;

	REngine         rEngine;
	DistNormalFloat *distribution;
	gaussGenerator  *gGenWall;

public:


public:
	MapParticle();
	virtual ~MapParticle(){}

	virtual void updateMap(tyPolygon * safeArea, tyPolygon * wallArea);

	void fillWallArea(tyPolygon *wallArea);
	void clearSafeArea(tyPolygon *wallArea);

	vector<LocationWWeight>* getParticleList() const {
		return particleList;
	}
};



#endif /* MAPPARTICLE_HPP_ */
