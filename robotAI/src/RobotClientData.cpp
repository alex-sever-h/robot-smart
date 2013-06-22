/*
 * RobotClientData.cpp
 *
 *  Created on: Jun 21, 2013
 *      Author: alseh
 */

#include <iostream>
#include "boost/array.hpp"
#include "RobotClientData.hpp"
#include "RobotServer.hpp"

using namespace std;

void RobotClientData::updateFromBuffer(void *receivedData, int nBytes, int dataType)
{
	switch(dataType)
	{
	case ROBOT_POSITION:
		rInfoMutex.lock();
		this->robotInfo.ParseFromArray(receivedData, nBytes);
		cout << "posx:  " << robotInfo.posx() << endl;
		cout << "posy:  " << robotInfo.posy() << endl;
		cout << "theta: " << robotInfo.theta() << endl;
		rInfoMutex.unlock();
		break;
	case FULL_PATH:
		pInfoMutex.lock();
		this->pathInfo.ParseFromArray(receivedData, nBytes);
		cout << "pathx:  " << pathInfo.firstdot().x() << endl;
		cout << "pathy:  " << pathInfo.firstdot().y() << endl;
		pInfoMutex.unlock();
		break;
	case SENSOR_READING:
	{
		sInfoMutex.lock();
		robotdata::SensorInfo *sInfo = new robotdata::SensorInfo;
		sInfo->ParseFromArray(receivedData, nBytes);
		for(list<robotdata::SensorInfo *>::iterator it = sensorsInfo.begin();
				it != sensorsInfo.end(); ++it)
		{
			if((*it)->sensor_id() == sInfo->sensor_id())
			{
				delete (*it);
				sensorsInfo.erase(it);
				break;
			}
		}
		sensorsInfo.push_back(sInfo);
		sInfoMutex.unlock();
	}break;
	case FULL_MAP:
		mInfoMutex.lock();
		mapInfo.ParseFromArray(receivedData, nBytes);
		mInfoMutex.unlock();
		break;
	default:
		cout << "unknown package" << endl;
		break;
	}
}
