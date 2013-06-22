/*
 * RoboInterface.hpp
 *
 *  Created on: Jun 22, 2013
 *      Author: alseh
 */

#ifndef ROBOINTERFACE_HPP_
#define ROBOINTERFACE_HPP_

#include <boost/thread.hpp>
#include <string>

#include "sensorManagement.h"
#include "RobotMovementEngine.hpp"

using namespace std;
using namespace boost;

class SensorManager;

typedef struct
{
	string id;
	int length;
} InterfaceCmd;

class RoboInterface
{
protected:
	bool connected;
	string recvBuffer;

	SensorManager *sensMan;
	RobotMovementEngine *rme;

	thread *sensorPollerThread;

public:
	RoboInterface();
	virtual ~RoboInterface();

	void startSensorPoller();

	void setRme(RobotMovementEngine* rme)
	{
		this->rme = rme;
	}
	void setSensMan(SensorManager *sMan)
	{
		this->sensMan = sMan;
	}

	void move(int distance);
	void rotate(int degrees);

	void move_by_time(int timeMs);
	void rotate_by_time(int timeMs);

public:
	virtual int connectRobot() = 0;

	virtual void writeData(string &command) = 0;
	virtual bool readData() = 0;

	void pollUpdateSensors();

};

#endif /* ROBOINTERFACE_HPP_ */
