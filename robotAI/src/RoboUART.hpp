/*
 * RoboUART.hpp
 *
 *  Created on: Jun 22, 2013
 *      Author: alseh
 */

#ifndef ROBOUART_HPP_
#define ROBOUART_HPP_

#include <unistd.h>
#include <fcntl.h>
#include <termios.h>

#include "RoboInterface.hpp"

#define BAUDRATE B460800
#define MODEMDEVICE "/dev/ttyS0"

using namespace std;
using namespace boost;

class RoboUART : public RoboInterface
{
	// UART specific
	struct termios tio;
	struct termios port_settings;
	int tty_fd;

public:
	RoboUART();
	virtual ~RoboUART();

	int connectRobot();
	void writeData(string &command);
	bool readData();
};

#endif /* ROBOUART_HPP_ */
