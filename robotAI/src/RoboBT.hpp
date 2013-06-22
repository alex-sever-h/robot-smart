/*
 * RoboBT.h
 *
 *  Created on: May 21, 2013
 *      Author: alseh
 */

#ifndef ROBOBT_H_
#define ROBOBT_H_

#include <unistd.h>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>

#include "RoboInterface.hpp"


using namespace std;
using namespace boost;

class RoboBT : public RoboInterface
{
	//bt specific
	char name[100];
	char address[20];
	int sock;
	struct sockaddr_rc btSocket;

	int searchRobot();

public:
	RoboBT(bool predefined);
	virtual ~RoboBT();

	int connectRobot();
	void writeData(string &command);
	bool readData();
};

#endif /* ROBOBT_H_ */
