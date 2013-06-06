/*
 * RobotLocalizationParticle.hpp
 *
 *  Created on: Jun 6, 2013
 *      Author: alseh
 */

#ifndef ROBOTLOCALIZATIONPARTICLE_HPP_
#define ROBOTLOCALIZATIONPARTICLE_HPP_

#include <vector>

using namespace std;

class RobotLocalizationParticle{
	int x;
	int y;
	float theta;

public:
	RobotLocalizationParticle(int x, int y, float theta);
	virtual ~RobotLocalizationParticle();

	float senseWorld(MapDiscrete *world, vector<tyPolygon *> *sensorAreas);
};


class RobotLocalizationManager{
	vector<RobotLocalizationParticle> *particles;

public:
	RobotLocalizationManager();
	virtual ~RobotLocalizationManager();
};


#endif /* ROBOTLOCALIZATIONPARTICLE_HPP_ */
