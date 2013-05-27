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
#include "locationWWeight.hpp"
#include "geometricPlane.hpp"

using namespace std;

typedef struct {
	string id;

	float angleCenter;
	float angleSpan;
	int offsetx;
	int offsety;

	int dist;
	vector<LocationWWeight> *poly;
} tySensor;

void init_sensors();

class SensorManager {
public: // TODO: remove public
	vector<tySensor> sensors;

public:
	SensorManager();
	virtual ~SensorManager();

	void registerMeasurement(string id, int length);

	vector<LocationWWeight>	* calculateSensorPolygon(float angleCenter, float angleSpan, int cx, int cy, float dist);
};

#endif /* SENSORMANAGEMENT_H_ */
