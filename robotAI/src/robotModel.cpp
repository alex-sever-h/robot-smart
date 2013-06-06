/*
 * robotModel.cpp
 *
 *  Created on: May 28, 2013
 *      Author: alseh
 */

#include "robotModel.hpp"
#include "boost/thread.hpp"

RobotModel::RobotModel(RoboBT* btInterface)
{
	positionXmm   = 0;
	positionYmm   = 0;
	orientationRad = 0;

	widthMM  = 200;
	lengthMM = 300;

	roboBTinterface = btInterface;
	rME = new RobotMovementEngine(this, btInterface);
	sensorManager = new SensorManager(this);

	btInterface->setSensMan(sensorManager);

	robotPolygon = NULL;
	updateRobotPolygon();

	sensorManager->updateSensorsOffset(positionXmm, positionYmm, orientationRad);
}

RobotModel::RobotModel(int x, int y, float theta, RoboBT* btInterface){
	positionXmm = x;
	positionYmm = y;
	orientationRad = theta;

	widthMM  = 200;
	lengthMM = 300;

	roboBTinterface = btInterface;
	rME = new RobotMovementEngine(this, btInterface);
	sensorManager = new SensorManager(this);

	btInterface->setSensMan(sensorManager);

	robotPolygon = NULL;
	updateRobotPolygon();

	sensorManager->updateSensorsOffset(positionXmm, positionYmm, orientationRad);
}

RobotModel::~RobotModel() {
	rME->stop();
	delete rME;

	delete sensorManager;
}

void RobotModel::setPosition(int x, int y, float theta) {
	positionXmm = x;
	positionYmm = y;
	orientationRad = theta;

//	cout << "robot pos set to: " << x << y << theta << endl;

	updateRobotPolygon();

	sensorManager->updateSensorsOffset(positionXmm, positionYmm, orientationRad);
}

void RobotModel::getPosition(int* x, int* y, float* theta) {
	*x = positionXmm;
	*y = positionYmm;
	*theta = orientationRad;
}

int RobotModel::move(int distanceMM) {
	return rME->move(distanceMM);
}

int RobotModel::rotate(float theta) {
	return rME->rotate(theta);
}

void RobotModel::setSensorManager(SensorManager* sensorManager)  {
	this->sensorManager = sensorManager;
}


void RobotModel::updateRobotPolygon() {
	if(robotPolygon)
		delete robotPolygon;

	int points[6][2] = {
			{+lengthMM/2 + 20, 0 },
			{+lengthMM/2,    -widthMM/2},
			{-lengthMM/2,    -widthMM/2},
			{-lengthMM/2 + 20,    0},
			{-lengthMM/2,    +widthMM/2},
			{+lengthMM/2,    +widthMM/2}
	};

	robotPolygon = new tyPolygon;

	for(unsigned int i = 0; i < 6; ++i)
	{
		int x, y;

		x = points[i][0] * cos(orientationRad) - points[i][1] * sin(orientationRad) + positionXmm;
		y = points[i][0] * sin(orientationRad) + points[i][1] * cos(orientationRad) + positionYmm;

		robotPolygon->push_back( LocationWWeight (x, y, 0) );
	}
}

tyPolygon * RobotModel::getRobotPoly(){
	tyPolygon * rPoly;
	rPoly = new tyPolygon(*robotPolygon);
	return rPoly;
}

void RobotModel::placeRobotInMap(MapParticle* world) {
	this->worldMap = world;
}

void RobotModel::updateWorld(tyPolygon * safeArea, tyPolygon * wallPoly)
{
//	thread *updateMapThread;
//	updateMapThread = new thread(&MapDiscrete::updateMap, worldMap,
//			new tyPolygon(*safeArea), new tyPolygon(*wallPoly));

	worldMap->updateMap(safeArea, wallPoly);
}

vector<tyPolygon*>* RobotModel::getSensorSafeAreas() {
	return sensorManager->getSensorSafeAreas();
}

vector<tyPolygon*>* RobotModel::getSensorWallAreas() {
	return sensorManager->getSensorWallAreas();
}
