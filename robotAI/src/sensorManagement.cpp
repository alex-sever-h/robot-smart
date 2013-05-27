/*
 * sensorManagement.cpp
 *
 *  Created on: May 22, 2013
 *      Author: alseh
 */

#include <stdio.h>
#include "sensorManagement.h"
#include "math.h"
#include <vector>
#include <iostream>

#define RADIANS(x)  (M_PI*((float)(x)/180))

using namespace std;


SensorManager::SensorManager() : sensors(4)
{
	sensors.at(0).id = "F_R";
	sensors.at(1).id = "F_L";
	sensors.at(2).id = "R_R";
	sensors.at(3).id = "R_L";

	sensors.at(0).angleCenter = RADIANS( +80);
	sensors.at(1).angleCenter = RADIANS(+100);
	sensors.at(2).angleCenter = RADIANS( -80);
	sensors.at(3).angleCenter = RADIANS(-100);

	sensors.at(0).angleSpan = RADIANS(20);
	sensors.at(1).angleSpan = RADIANS(20);
	sensors.at(2).angleSpan = RADIANS(20);
	sensors.at(3).angleSpan = RADIANS(20);

	sensors.at(0).offsetx = +3;
	sensors.at(1).offsetx = -3;
	sensors.at(2).offsetx = +3;
	sensors.at(3).offsetx = -3;

	sensors.at(0).offsety = +15;
	sensors.at(1).offsety = +15;
	sensors.at(2).offsety = -15;
	sensors.at(3).offsety = -15;

	sensors.at(0).dist = 0;
	sensors.at(1).dist = 0;
	sensors.at(2).dist = 0;
	sensors.at(3).dist = 0;

	sensors.at(0).poly = NULL;
	sensors.at(1).poly = NULL;
	sensors.at(2).poly = NULL;
	sensors.at(3).poly = NULL;
}

SensorManager::~SensorManager()
{

}

void SensorManager::registerMeasurement(string id, int length)
{
	for(unsigned int i = 0; i < sensors.size(); ++i)
		if(sensors.at(i).id == id)
		{
			sensors.at(i).dist = length;

			if(sensors.at(i).poly != NULL)
				delete sensors.at(i).poly;

			sensors.at(i).poly = calculateSensorPolygon(sensors.at(i).angleCenter,
					sensors.at(i).angleSpan, sensors.at(i).offsetx,
					sensors.at(i).offsety, sensors.at(i).dist);
		}
}


vector<LocationWWeight>	* SensorManager::calculateSensorPolygon(float angleCenter, float angleSpan, int cx, int cy, float dist)
{
	vector<LocationWWeight>	*poly;

	const int num_segments = 5;
	float theta = angleSpan / float(num_segments - 1);
	float tangetial_factor = tanf(theta);
	float radial_factor = cosf(theta);
	float x = dist * cosf(angleCenter - angleSpan/2);
	float y = dist * sinf(angleCenter - angleSpan/2);

	poly = new vector<LocationWWeight>;

	poly->push_back( LocationWWeight(cx, cy, 0) );

	for(int ii = 0; ii < num_segments; ii++)
	{
		poly->push_back( LocationWWeight( x + cx, y + cy, 0) );

		float tx = -y;
		float ty = x;

		x += tx * tangetial_factor;
		y += ty * tangetial_factor;

		x *= radial_factor;
		y *= radial_factor;
	}

	return poly;
}
