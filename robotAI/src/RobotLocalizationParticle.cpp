/*
 * RobotLocalizationParticle.cpp
 *
 *  Created on: Jun 6, 2013
 *      Author: alseh
 */

#include "RobotLocalizationParticle.hpp"


RobotLocalizationParticle::RobotLocalizationParticle(int x, int y, float theta) {
	this->x = x;
	this->y = y;
	this->theta = theta;
}

RobotLocalizationParticle::~RobotLocalizationParticle() {
}





RobotLocalizationManager::RobotLocalizationManager() {
	particles = new vector<RobotLocalizationParticle>;
}




RobotLocalizationManager::~RobotLocalizationManager() {
}

float RobotLocalizationParticle::senseWorld(MapDiscrete* world,
		vector<tyPolygon*>* sensorAreas) {
}
