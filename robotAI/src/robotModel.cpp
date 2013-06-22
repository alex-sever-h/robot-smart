/*
 * robotModel.cpp
 *
 *  Created on: May 28, 2013
 *      Author: alseh
 */

#include "robotModel.hpp"
#include "boost/thread.hpp"
#include "protobuf/robotdata.pb.h"
#include <pthread.h>

RobotModel::RobotModel(RoboInterface* btInterface)
{
	positionXmm = 0;
	positionYmm = 0;
	orientationRad = 0;

	widthMM = 200;
	lengthMM = 250;

	roboInterface = btInterface;
	rME = new RobotMovementEngine(this, btInterface);
	roboInterface->setRme(rME);
	sensorManager = new SensorManager(this);
	pathFinder = new AStarPathfinder(this);

	btInterface->setSensMan(sensorManager);

	srv = NULL;

	robotPolygon = NULL;
	sendRobotInfo();

	path = NULL;

	sensorManager->updateSensorsOffset(positionXmm, positionYmm,
			orientationRad);
}

RobotModel::RobotModel(float x, float y, float theta, RoboInterface* btInterface)
{
	positionXmm = x;
	positionYmm = y;
	orientationRad = theta;

	widthMM = 200;
	lengthMM = 300;

	roboInterface = btInterface;
	rME = new RobotMovementEngine(this, btInterface);
	roboInterface->setRme(rME);
	sensorManager = new SensorManager(this);
	pathFinder = new AStarPathfinder(this);

	btInterface->setSensMan(sensorManager);

	srv = NULL;

	robotPolygon = NULL;
	sendRobotInfo();

	path = NULL;

	sensorManager->updateSensorsOffset(positionXmm, positionYmm,
			orientationRad);
}

RobotModel::~RobotModel()
{
	rME->stopMotion();
	delete rME;

	delete sensorManager;
}

void RobotModel::setPosition(float x, float y, float theta)
{
	positionXmm = x;
	positionYmm = y;
	orientationRad = theta;

	//	cout << "robot pos set to: " << x << y << theta << endl;
	sendRobotInfo();
	sensorManager->updateSensorsOffset(positionXmm, positionYmm,
			orientationRad);
}

void RobotModel::getPosition(float* x, float* y, float* theta)
{
	*x = positionXmm;
	*y = positionYmm;
	*theta = orientationRad;
}

int RobotModel::move(int distanceMM)
{
	return rME->move(distanceMM);
}

int RobotModel::rotate(float theta)
{
	return rME->rotate(theta);
}

void RobotModel::sendRobotInfo()
{
	if(srv)
	{
		robotdata::RobotInfo robotInfo;

		robotInfo.set_length(lengthMM);
		robotInfo.set_width(widthMM);
		robotInfo.set_posx(positionXmm);
		robotInfo.set_posy(positionYmm);
		robotInfo.set_theta(orientationRad);

		srv->sendSerializedData(&robotInfo, ROBOT_POSITION);
	}
}

tyPolygon * RobotModel::getRobotPoly()
{
	tyPolygon * rPoly;
	rPoly = new tyPolygon(*robotPolygon);
	return rPoly;
}

void RobotModel::placeRobotInMap(MapParticle* world)
{
	this->worldMap = world;
	this->pathFinder->setWorld(worldMap);
}

int countUP = 0;

vector<tyPolygon*>* RobotModel::getSensorSafeAreas()
{
	return sensorManager->getSensorSafeAreas();
}

vector<tyPolygon*>* RobotModel::getSensorWallAreas()
{
	return sensorManager->getSensorWallAreas();
}

Location RobotModel::getPosition(void)
{
	return Location(positionXmm, positionYmm, orientationRad);
}

void RobotModel::setPosition(Location location)
{
	positionXmm = location.x;
	positionYmm = location.y;
	orientationRad = location.theta;

	sendRobotInfo();
	sensorManager->updateSensorsOffset(positionXmm, positionYmm,
			orientationRad);
}

void RobotModel::updateWorld(tySensor* sensor)
{
	worldMap->updateMap(sensor);

	if (pathFinder->checkPathForMapUpdates(path) == false)
	{
		rME->interruptPathFollowing();

		delete path;
		path = NULL;
		path = pathFinder->generateRawPath(
				LocationWWeight(target.x, target.y, 0));

		rME->followPath(path);

		cout << "detect collition ``````````````````````````\n";
	}

	sendMessage(worldMap->sendFullMap(), FULL_MAP);
}

void RobotModel::pathfinderThread(const Location& target)
{
	this->setTarget(target);
	path = pathFinder->generateRawPath(LocationWWeight(target.x, target.y, 0));
	if(path)
	{
		sendMessage(pathFinder->sendPath(path), FULL_PATH);
		rME->followPath(path);
		cout << "path found ... start movement" << endl;
	}
}

void RobotModel::sendMessage(google::protobuf::Message* message, enum dataType enumDataType)
{
	srv->sendSerializedData(message, enumDataType);
	delete message;
}

void RobotModel::moveAtLocation(Location target)
{
	rME->interruptPathFollowing();

	cout << "start pathfinder " << target.x << "x" << target.y << endl;

	thread pathfinder(&RobotModel::pathfinderThread, this, target);

	struct sched_param param;
	param.sched_priority = 9;
	pthread_t threadID = pathfinder.native_handle();

	pthread_setschedparam(threadID, SCHED_RR, &param);
}

