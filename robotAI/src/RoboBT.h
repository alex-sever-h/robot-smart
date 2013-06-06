/*
 * RoboBT.h
 *
 *  Created on: May 21, 2013
 *      Author: alseh
 */

#ifndef ROBOBT_H_
#define ROBOBT_H_

#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>
#include <sstream>
#include <iostream>
#include "sensorManagement.h"

using namespace std;

class SensorManager;

typedef struct {
	string id;
	int    length;
} BtCmd;

class RoboBT{
	char name[100];
	char address[20];
	int  sock;
	struct sockaddr_rc btSocket;
	bool connected;
	string recvBuffer;

	SensorManager *sensMan;

public:
	RoboBT(bool predefined);
	virtual ~RoboBT();

	int   searchRobot();
	int   connectRobot();
	bool  pollUpdateSensors();
	void  setSensMan(SensorManager *sMan){this->sensMan = sMan;}

	void  move(int distance);
	void  rotate(int degrees);

protected:
	void  send(string &command);
	bool  readData();
};


#endif /* ROBOBT_H_ */
