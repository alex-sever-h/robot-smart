/*
 * sensorManagement.h
 *
 *  Created on: May 22, 2013
 *      Author: alseh
 */

#ifndef SENSORMANAGEMENT_H_
#define SENSORMANAGEMENT_H_

#include <string>
#include <vector>
#include <cmath>
#include "locationWWeight.hpp"
#include "geometricPlane.hpp"
#include "robotModel.hpp"

#define SENSOR_MAX_DISTANCE_MM 4000

using namespace std;

class tySensor {
public:
	string id;

	float angleSpan;

	float angleCenterOffset;
	int   offsetXrobot;
	int   offsetYrobot;

	float angleCenterRadMap;
	float   offsetXmapMM;
	float   offsetYmapMM;

	unsigned int distanceMM;
	tyPolygon *polySafe;
	tyPolygon *polyWall;

	tySensor(string id, int offsetRobotXmm, int offsetRobotYmm, int angleCenter)
	{
		this->id = id;
		this->angleCenterRadMap = DEG_TO_RAD( angleCenter );
		this->angleCenterOffset = DEG_TO_RAD( angleCenter );
		this->angleSpan         = DEG_TO_RAD(15);
		this->offsetXrobot = offsetRobotXmm;
		this->offsetYrobot = offsetRobotYmm;
		this->offsetXmapMM = (float)offsetRobotXmm;
		this->offsetYmapMM = (float)offsetRobotXmm;
		this->distanceMM = 0;
		this->polySafe = NULL;
		this->polyWall = NULL;
	}

};

void init_sensors();

class RobotModel;

class SensorManager {
	vector<tySensor*> sensors;

	RobotModel      *physicalRobot;

	void initSensors();

public:
	SensorManager(RobotModel *pR);
	virtual ~SensorManager();

	void registerMeasurement(string id, unsigned int length);

	void calculateSensorPolygons(
			bool sensorAtMax,
			vector<LocationWWeight>	** safePolygon,
			vector<LocationWWeight>	** linePolygon,
			float angleCenter, float angleSpan, int cx, int cy, float dist);

	void setSensorOffset(string sensor_id, int offsetx, int offsety);
	void updateSensorsOffset(float px, float py, float theta);

	vector<tyPolygon *> *getSensorSafeAreas();
	vector<tyPolygon *> *getSensorWallAreas();
};

#endif /* SENSORMANAGEMENT_H_ */
