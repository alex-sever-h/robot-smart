/*
 * robotModel.hpp
 *
 *  Created on: May 28, 2013
 *      Author: alseh
 */

#ifndef ROBOTMODEL_HPP_
#define ROBOTMODEL_HPP_

#include <cmath>
#include "RoboBT.h"
#include "RobotMovementEngine.hpp"
#include "sensorManagement.h"
#include "MapGeneric.hpp"
#include "mapDiscrete.hpp"
#include "MapParticle.hpp"

class SensorManager;
class RobotMovementEngine;
class RoboBT;

class RobotModel{
	volatile int 	positionXmm;
	volatile int 	positionYmm;
	volatile float 	orientationRad;

	int widthMM;
	int lengthMM;

	SensorManager       *sensorManager;

	RoboBT              *roboBTinterface;
	RobotMovementEngine *rME;

	tyPolygon           *robotPolygon;

	MapParticle			*worldMap;

public:
	RobotModel(RoboBT *btInterface);
	RobotModel(int x, int y, float theta, RoboBT *btInterface);
	virtual ~RobotModel();

	void placeRobotInMap(MapParticle *world);

	void setSensorManager(SensorManager *sensorManager);

	void setPosition(int xMM, int yMM, float thetaRad);
	void getPosition(int *xMM, int *yMM, float *thetaRad);

	int  move(int distanceMM);
	int  rotate(float thetaRad);

	void updateRobotPolygon();
	tyPolygon * getRobotPoly();

	vector<tyPolygon *> *getSensorSafeAreas();
	vector<tyPolygon *> *getSensorWallAreas();

	void updateWorld(tyPolygon * sensorArea, tyPolygon * wallPoly);
};


#endif /* ROBOTMODEL_HPP_ */
