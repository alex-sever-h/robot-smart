/*
 * robotModel.hpp
 *
 *  Created on: May 28, 2013
 *      Author: alseh
 */

#ifndef ROBOTMODEL_HPP_
#define ROBOTMODEL_HPP_

#include <cmath>
#include "RoboInterface.hpp"
#include "RobotMovementEngine.hpp"
#include "sensorManagement.h"
#include "MapGeneric.hpp"
#include "mapDiscrete.hpp"
#include "MapParticle.hpp"
#include "AStarPathfinder.hpp"
#include "Location.hpp"
#include "RobotServer.hpp"

class tySensor;
class SensorManager;
class RobotMovementEngine;
class RoboInterface;
class AStarPathfinder;
class PathNode;
class MapParticle;
class RobotServer;

class RobotModel
{
	volatile float positionXmm;
	volatile float positionYmm;
	volatile float orientationRad;

	int widthMM;
	int lengthMM;

	SensorManager *sensorManager;

	RoboInterface *roboInterface;
	RobotMovementEngine *rME;
	AStarPathfinder *pathFinder;

	tyPolygon *robotPolygon;

	MapParticle *worldMap;

	RobotServer *srv;

	Location target;

	void pathfinderThread(const Location& target);

public:
	PathNode *path;
	RobotModel(RoboInterface *btInterface);
	RobotModel(float x, float y, float theta, RoboInterface *btInterface);
	virtual ~RobotModel();

	void placeRobotInMap(MapParticle *world);

	void setPosition(float xMM, float yMM, float thetaRad);
	void getPosition(float *xMM, float *yMM, float *thetaRad);

	Location getPosition(void);
	void setPosition(Location);

	int move(int distanceMM);
	int rotate(float thetaRad);

	void sendRobotInfo();
	tyPolygon * getRobotPoly();

	vector<tyPolygon *> *getSensorSafeAreas();
	vector<tyPolygon *> *getSensorWallAreas();

	void updateWorld(tySensor *sensor);

	void sendMessage(google::protobuf::Message *message, enum dataType enumDataType);

	int getWidthMm() const
	{
		return widthMM;
	}
	int getLengthMm() const
	{
		return lengthMM;
	}

	void moveAtLocation(Location target);

	Location getTarget() const
	{
		return target;
	}

	void setTarget(Location target)
	{
		this->target = target;
	}

	volatile float getOrientationRad() const
	{
		return orientationRad;
	}

	volatile float getPositionXmm() const
	{
		return positionXmm;
	}

	volatile float getPositionYmm() const
	{
		return positionYmm;
	}

	void setSrv(RobotServer* srv)
	{
		this->srv = srv;
	}
};

#endif /* ROBOTMODEL_HPP_ */
