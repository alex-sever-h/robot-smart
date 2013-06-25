/*
 * RoboInterface.cpp
 *
 *  Created on: Jun 22, 2013
 *      Author: alseh
 */

#include "RoboInterface.hpp"
#include <boost/algorithm/string.hpp>

RoboInterface::RoboInterface()
{
	rme = NULL;
	connected = false;

	sensorPollerThread = NULL;
	sensMan = NULL;
}

RoboInterface::~RoboInterface()
{
}

void RoboInterface::move(int distance)
{
	string cmd_start;

	if (distance > 0)
		cmd_start = "M_LR+450+450\r";
	else if (distance < 0)
		cmd_start = "M_LR-450-450\r";
	else
		cmd_start = "M_LR+000+000\r";

	if (connected)
		writeData(cmd_start);
}

void RoboInterface::rotate(int degrees)
{
	string cmd_start;

	if (degrees > 0)
		cmd_start = "M_LR-450+450\r";
	else if (degrees < 0)
		cmd_start = "M_LR+450-450\r";
	else
		cmd_start = "M_LR+000+000\r";

	if (connected)
		writeData(cmd_start);
}

void RoboInterface::move_by_time(int timeMs)
{
	string cmd_start;
	char intBuffer[10];
	sprintf(intBuffer+1, "%04d", abs(timeMs));

	if(timeMs >= 0)
		intBuffer[0] = '+';
	else if(timeMs < 0)
		intBuffer[0] = '-';

	cmd_start = "M_FT" + string(intBuffer) + "\r";

	//cout << cmd_start << endl;

	if (connected)
		writeData(cmd_start);
}

void RoboInterface::rotate_by_time(int timeMs)
{
	string cmd_start;

	char intBuffer[10];
	sprintf(intBuffer+1, "%04d", abs(timeMs));

	if(timeMs >= 0)
		intBuffer[0] = '+';
	else if(timeMs < 0)
		intBuffer[0] = '-';

	cmd_start = "M_RT" + string(intBuffer) + "\r";

	//cout << cmd_start << endl;

	if (connected)
		writeData(cmd_start);
}

void RoboInterface::startSensorPoller()
{
	sensorPollerThread = new thread(&RoboInterface::pollUpdateSensors, this);
}

void RoboInterface::stopMovement()
{
	string cmd_start = "M_INTERRUPT\r";

	if (connected)
		writeData(cmd_start);
}

void RoboInterface::pollUpdateSensors()
{
	InterfaceCmd btcmd;
	vector<string> fields;
	vector<string> cmd;
	size_t i;

	while (1)
	{
		readData();

		split(fields, recvBuffer, is_any_of("\n"), token_compress_on);

		for (i = 0; i < fields.size() - 1; ++i)
		{
			split(cmd, fields[i], is_any_of(" :"), token_compress_on);
			if (cmd.size() == 2)
			{
				unsigned int value;
				istringstream(cmd[1]) >> value;
				//				cout << cmd[0] << "-->" << cmd[1] << " === " << len << endl;
				if (sensMan)
					sensMan->registerMeasurement(cmd[0], value);

				if (cmd[0] == "TIM")
					rme->updatePhysicalRobot(value);
			}
			else
			{
				if(fields[i] == "ACK")
				{
					cout << "ACKNOWLEDGEDACKNOWLKNOWLEDGEDACKNOWLEDGED\n";
					if(rme)
						rme->acknowledgeCommand();
				}
				if(fields[i] == "RDY")
				{
					cout << "READYREADYREADYRYREADYEADYREADYREADYREADY\n";
					if(rme)
						rme->finalizeCommand();
				}
			}
		}
		recvBuffer = fields[i];
	}

}
