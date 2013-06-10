/*
 * sensorManagement.cpp
 *
 *  Created on: May 22, 2013
 *      Author: alseh
 */

#include <stdio.h>
#include "sensorManagement.h"
#include "mapDiscrete.hpp"
#include "math.h"
#include <vector>
#include <iostream>

using namespace std;

SensorManager::SensorManager(RobotModel *pR) : sensors(4)
{
	sensors.at(0).id = "F_R";
	sensors.at(1).id = "F_L";
	sensors.at(2).id = "R_R";
	sensors.at(3).id = "R_L";

	sensors.at(0).angleCenterRad = DEG_TO_RAD( +80);
	sensors.at(1).angleCenterRad = DEG_TO_RAD(+100);
	sensors.at(2).angleCenterRad = DEG_TO_RAD( -80);
	sensors.at(3).angleCenterRad = DEG_TO_RAD(-100);

	sensors.at(0).angleSpan = DEG_TO_RAD(20);
	sensors.at(1).angleSpan = DEG_TO_RAD(20);
	sensors.at(2).angleSpan = DEG_TO_RAD(20);
	sensors.at(3).angleSpan = DEG_TO_RAD(20);

	sensors.at(0).offsetXrobot = +30;
	sensors.at(1).offsetXrobot = -30;
	sensors.at(2).offsetXrobot = +30;
	sensors.at(3).offsetXrobot = -30;

	sensors.at(0).offsetYrobot = +120;
	sensors.at(1).offsetYrobot = +120;
	sensors.at(2).offsetYrobot = -120;
	sensors.at(3).offsetYrobot = -120;

	sensors.at(0).dist = 0;
	sensors.at(1).dist = 0;
	sensors.at(2).dist = 0;
	sensors.at(3).dist = 0;

	sensors.at(0).polySafe = NULL;
	sensors.at(1).polySafe = NULL;
	sensors.at(2).polySafe = NULL;
	sensors.at(3).polySafe = NULL;

	sensors.at(0).polyWall = NULL;
	sensors.at(1).polyWall = NULL;
	sensors.at(2).polyWall = NULL;
	sensors.at(3).polyWall = NULL;

	physicalRobot = pR;
}

SensorManager::~SensorManager()
{

}

void SensorManager::registerMeasurement(string id, int length)
{
	if(length > 30000)
		length = 5000;
	for(unsigned int i = 0; i < sensors.size(); ++i)
		if(sensors.at(i).id == id)
		{
			sensors.at(i).dist = length;

			calculateSensorPolygons(&sensors.at(i).polySafe, &sensors.at(i).polyWall,
					sensors.at(i).angleCenterRad, sensors.at(i).angleSpan,
					sensors.at(i).offsetXmapMM,	sensors.at(i).offsetYmapMM, sensors.at(i).dist);

			physicalRobot->updateWorld(sensors.at(i).polySafe, sensors.at(i).polyWall);

			cout << id << "  " << length << endl;
		}
}

void SensorManager::calculateSensorPolygons(
		vector<LocationWWeight>	** safePolygon,
		vector<LocationWWeight>	** wallPolygon,
		float angleCenter, float angleSpan, int cx, int cy, float dist)
{
	const int num_segments = 10;
	float theta = angleSpan / float(num_segments - 1);
	float tangetial_factor = tanf(theta);
	float radial_factor = cosf(theta);
	float x = dist * cosf(angleCenter - angleSpan/2);
	float y = dist * sinf(angleCenter - angleSpan/2);

	if(*safePolygon)
		delete *safePolygon;
	*safePolygon = new tyPolygon;

	if(*wallPolygon)
		delete *wallPolygon;
	*wallPolygon = new tyPolygon;

	(*safePolygon)->push_back( LocationWWeight(cx, cy, 0) );

	for(int ii = 0; ii < num_segments; ii++)
	{
		(*safePolygon)->push_back( LocationWWeight( x + cx, y + cy, 0) );
		(*wallPolygon)->push_back( LocationWWeight( x + cx, y + cy, 0) );

		float tx = -y;
		float ty = x;

		x += tx * tangetial_factor;
		y += ty * tangetial_factor;

		x *= radial_factor;
		y *= radial_factor;
	}

}

void SensorManager::setSensorOffset(string sensor_id, int offsetx, int offsety)
{
	for(unsigned int i = 0; i < sensors.size(); ++i)
		if(sensors.at(i).id == sensor_id)
		{
			sensors.at(i).offsetXmapMM = offsetx;
			sensors.at(i).offsetYmapMM = offsety;
		}
}

void SensorManager::updateSensorsOffset(int pxMM, int pyMM, float theta)
{
	float sAtan;
	float sDist;

	for(unsigned int i = 0; i < sensors.size(); ++i)
	{
		if(sensors.at(i).id == "F_R")
		{
			sAtan = atan((float)sensors.at(i).offsetXrobot/(float)sensors.at(i).offsetYrobot);
			sDist = sqrt((float)sensors.at(i).offsetXrobot * (float)sensors.at(i).offsetXrobot +
					(float)sensors.at(i).offsetYrobot * (float)sensors.at(i).offsetYrobot);

			sensors.at(i).offsetXmapMM  = (float)pxMM + (float)sDist * cos(theta - sAtan);
			sensors.at(i).offsetYmapMM  = (float)pyMM + (float)sDist * sin(theta - sAtan);
			sensors.at(i).angleCenterRad = +theta - DEG_TO_RAD(7);
		}
		else if(sensors.at(i).id == "F_L")
		{
			sAtan = atan((float)sensors.at(i).offsetXrobot/(float)sensors.at(i).offsetYrobot);
			sDist = sqrt((float)sensors.at(i).offsetXrobot * (float)sensors.at(i).offsetXrobot +
					(float)sensors.at(i).offsetYrobot * (float)sensors.at(i).offsetYrobot);

			sensors.at(i).offsetXmapMM = (float)pxMM + (float)sDist * cos(theta - sAtan);
			sensors.at(i).offsetYmapMM = (float)pyMM + (float)sDist * sin(theta - sAtan);
			sensors.at(i).angleCenterRad = +theta + DEG_TO_RAD(7);
		}
		else if(sensors.at(i).id == "R_R")
		{
			sAtan = atan((float)sensors.at(i).offsetXrobot/(float)sensors.at(i).offsetYrobot) - M_PI;
			sDist = sqrt((float)sensors.at(i).offsetXrobot * (float)sensors.at(i).offsetXrobot +
					(float)sensors.at(i).offsetYrobot * (float)sensors.at(i).offsetYrobot);

			sensors.at(i).offsetXmapMM = (float)pxMM + (float)sDist * cos(theta - sAtan);
			sensors.at(i).offsetYmapMM = (float)pyMM + (float)sDist * sin(theta - sAtan);
			sensors.at(i).angleCenterRad = +theta - DEG_TO_RAD(173);
		}
		else if(sensors.at(i).id == "R_L")
		{
			sAtan = atan((float)sensors.at(i).offsetXrobot/(float)sensors.at(i).offsetYrobot) - M_PI;
			sDist = sqrt((float)sensors.at(i).offsetXrobot * (float)sensors.at(i).offsetXrobot +
					(float)sensors.at(i).offsetYrobot * (float)sensors.at(i).offsetYrobot);

			sensors.at(i).offsetXmapMM = (float)pxMM + (float)sDist * cos(theta - sAtan);
			sensors.at(i).offsetYmapMM = (float)pyMM + (float)sDist * sin(theta - sAtan);
			sensors.at(i).angleCenterRad = +theta + DEG_TO_RAD(173);
		}

//		cout << sensors.at(i).id << sAtan << endl;
	}
}

vector<tyPolygon *> *SensorManager::getSensorSafeAreas(){
	vector<tyPolygon*> *sPolyPtrs = new vector<tyPolygon*>;

	for(unsigned int i = 0; i < sensors.size(); ++i)
	{
		if(sensors.at(i).polySafe)
			sPolyPtrs->push_back(new tyPolygon(*sensors.at(i).polySafe));
	}

	return sPolyPtrs;
}

vector<tyPolygon *> *SensorManager::getSensorWallAreas(){
	vector<tyPolygon*> *sPolyPtrs = new vector<tyPolygon*>;

	for(unsigned int i = 0; i < sensors.size(); ++i)
	{
		if(sensors.at(i).polyWall)
			sPolyPtrs->push_back(new tyPolygon(*sensors.at(i).polyWall));
	}

	return sPolyPtrs;
}




