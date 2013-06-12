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

SensorManager::SensorManager(RobotModel *pR)
{
	this->sensors.push_back(new tySensor("FR_C", +110,   0,   0));
	this->sensors.push_back(new tySensor("FR_L", +110, +20, +15));
	this->sensors.push_back(new tySensor("FR_R", +110, -20, -15));

	this->sensors.push_back(new tySensor("RE_C", -110,   0,  180));
	this->sensors.push_back(new tySensor("RE_L", -110, +20, +165));
	this->sensors.push_back(new tySensor("RE_R", -110, -20, -165));

	this->sensors.push_back(new tySensor("RI_C",   0, -60,  -90));
	this->sensors.push_back(new tySensor("RI_F", +65, -60,  -80));
	this->sensors.push_back(new tySensor("RI_R", -65, -60, -100));

	this->sensors.push_back(new tySensor("LE_C",   0, +60,  +90));
	this->sensors.push_back(new tySensor("LE_F", +65, +60,  +80));
	this->sensors.push_back(new tySensor("LE_R", -65, +60, +100));

	physicalRobot = pR;
}

SensorManager::~SensorManager()
{

}

void SensorManager::registerMeasurement(string id, unsigned int length)
{
	//	cout << id << "  " << length << endl;
	for(unsigned int i = 0; i < sensors.size(); ++i)
		if(sensors.at(i)->id == id)
		{
			if(length > SENSOR_MAX_DISTANCE_MM)
			{
				delete sensors.at(i)->polySafe;
				delete sensors.at(i)->polyWall;

				sensors.at(i)->polySafe = NULL;
				sensors.at(i)->polyWall = NULL;
			}
			else
			{
				sensors.at(i)->distanceMM = length < SENSOR_MAX_DISTANCE_MM ? length : SENSOR_MAX_DISTANCE_MM;

				calculateSensorPolygons(sensors.at(i)->distanceMM == SENSOR_MAX_DISTANCE_MM,
						&sensors.at(i)->polySafe,
						&sensors.at(i)->polyWall,
						sensors.at(i)->angleCenterRadMap, sensors.at(i)->angleSpan,
						sensors.at(i)->offsetXmapMM,	sensors.at(i)->offsetYmapMM, sensors.at(i)->distanceMM);

				physicalRobot->updateWorld(sensors.at(i));
			}
			break;
		}
}

void SensorManager::calculateSensorPolygons(
		bool sensorAtMax,
		vector<LocationWWeight>	** safePolygon,
		vector<LocationWWeight>	** wallPolygon,
		float angleCenter, float angleSpan, int cx, int cy, float dist)
{
	//	cout << "start polygon distance:" << dist << "\n";
	const int num_segments = 10;
	float theta = angleSpan / float(num_segments - 1);
	float tangetial_factor = tanf(theta);
	float radial_factor = cosf(theta);
	float x = dist * cosf(angleCenter - angleSpan/2);
	float y = dist * sinf(angleCenter - angleSpan/2);

	tyPolygon *localSafePolygon = new tyPolygon;
	tyPolygon *localWallPolygon = new tyPolygon;

	localSafePolygon->push_back( LocationWWeight(cx, cy, 0) );

	//	cout << cx << " x " << cy << endl;

	for(int ii = 0; ii < num_segments; ii++)
	{
		localSafePolygon->push_back( LocationWWeight( x + cx, y + cy, 0) );
		localWallPolygon->push_back( LocationWWeight( x + cx, y + cy, 0) );

		//		cout << x + cx << " x " << y + cy << endl;

		float tx = -y;
		float ty = x;

		x += tx * tangetial_factor;
		y += ty * tangetial_factor;

		x *= radial_factor;
		y *= radial_factor;
	}


	if(safePolygon)
	{
		if(*safePolygon)
			delete *safePolygon;
		*safePolygon = localSafePolygon;
	}
	if(wallPolygon)
	{
		if(*wallPolygon)
			delete *wallPolygon;
		if(!sensorAtMax)
			*wallPolygon = localWallPolygon;
		else
		{
			delete localWallPolygon;
			*wallPolygon = NULL;
		}
	}

	//	cout << "end polygon distance:\n\n";
}

//TODO: REMOVE vvvvv
void SensorManager::setSensorOffset(string sensor_id, int offsetx, int offsety)
{
	for(unsigned int i = 0; i < sensors.size(); ++i)
		if(sensors.at(i)->id == sensor_id)
		{
			sensors.at(i)->offsetXmapMM = offsetx;
			sensors.at(i)->offsetYmapMM = offsety;
		}
}

void SensorManager::updateSensorsOffset(float robotXmm, float robotYmm, float robotTheta)
{
	float sAtan;
	float sDist;

	for(unsigned int i = 0; i < sensors.size(); ++i)
	{
		sAtan = atan2((float)sensors.at(i)->offsetYrobot , (float)sensors.at(i)->offsetXrobot);

		sDist = sqrt(powf((float)sensors.at(i)->offsetXrobot, 2) + powf((float)sensors.at(i)->offsetYrobot, 2));

		sensors.at(i)->offsetXmapMM  = (float)robotXmm + (float)sDist * cos(robotTheta + sAtan);
		sensors.at(i)->offsetYmapMM  = (float)robotYmm + (float)sDist * sin(robotTheta + sAtan);

		sensors.at(i)->angleCenterRadMap = robotTheta + sensors.at(i)->angleCenterOffset;

		//		cout << sensors.at(i)->id << sAtan << endl;
	}
}

vector<tyPolygon *> *SensorManager::getSensorSafeAreas(){
	vector<tyPolygon*> *sPolyPtrs = new vector<tyPolygon*>;

	for(unsigned int i = 0; i < sensors.size(); ++i)
	{
		if(sensors.at(i)->polySafe)
			sPolyPtrs->push_back(new tyPolygon(*sensors.at(i)->polySafe));
	}

	return sPolyPtrs;
}

vector<tyPolygon *> *SensorManager::getSensorWallAreas(){
	vector<tyPolygon*> *sPolyPtrs = new vector<tyPolygon*>;

	for(unsigned int i = 0; i < sensors.size(); ++i)
	{
		if(sensors.at(i)->polyWall)
			sPolyPtrs->push_back(new tyPolygon(*sensors.at(i)->polyWall));
	}

	return sPolyPtrs;
}




