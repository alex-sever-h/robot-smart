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

#define DEG_TO_RAD(x)  (M_PI*((float)(x)/180))
#define RAD_TO_DEG(x)  (((float)(x)*180)/M_PI)

using namespace std;

typedef struct {
	string id;

	float angleCenterRad;
	float angleSpan;
	int offsetXmapMM;
	int offsetYmapMM;
	int offsetXrobot;
	int offsetYrobot;
	int dist;

	tyPolygon *polySafe;
	tyPolygon *polyWall;
} tySensor;

void init_sensors();

class RobotModel;

class SensorManager {
	vector<tySensor> sensors;

	RobotModel      *physicalRobot;

public:
	SensorManager(RobotModel *pR);
	virtual ~SensorManager();

	void registerMeasurement(string id, int length);

	void calculateSensorPolygons(
			vector<LocationWWeight>	** safePolygon,
			vector<LocationWWeight>	** linePolygon,
			float angleCenter, float angleSpan, int cx, int cy, float dist);

	void setSensorOffset(string sensor_id, int offsetx, int offsety);
	void updateSensorsOffset(int px, int py, float theta);

	vector<tyPolygon *> *getSensorSafeAreas();
	vector<tyPolygon *> *getSensorWallAreas();
};

#endif /* SENSORMANAGEMENT_H_ */
