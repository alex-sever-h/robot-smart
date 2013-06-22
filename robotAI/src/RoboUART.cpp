/*
 * RoboUART.cpp
 *
 *  Created on: Jun 22, 2013
 *      Author: alseh
 */

#include "RoboUART.hpp"
#include <boost/algorithm/string.hpp>


RoboUART::RoboUART() : RoboInterface()
{
	tty_fd = 0;

	connectRobot();
}

RoboUART::~RoboUART()
{
	close(tty_fd);
}

int RoboUART::connectRobot()
{
	tty_fd = open(MODEMDEVICE, O_RDWR | O_NOCTTY );
	if (tty_fd <0)
	{
		perror(MODEMDEVICE);
		return 0;
	}

	bzero(&port_settings, sizeof(port_settings)); /* clear struct for new port settings */

	/*
	          BAUDRATE: Set bps rate. You could also use cfsetispeed and cfsetospeed.
	          CRTSCTS : output hardware flow control (only used if the cable has
	                    all necessary lines. See sect. 7 of Serial-HOWTO)
	          CS8     : 8n1 (8bit,no parity,1 stopbit)
	          CLOCAL  : local connection, no modem contol
	          CREAD   : enable receiving characters
	 */
	port_settings.c_cflag = BAUDRATE | CS8 | CLOCAL | CREAD;

	/*
	          IGNPAR  : ignore bytes with parity errors
	          ICRNL   : map CR to NL (otherwise a CR input on the other computer
	                    will not terminate input)
	          otherwise make device raw (no other input processing)
	 */
	//new_io.c_iflag = IGNPAR | ICRNL;

	/*
	         Raw output.
	 */
	port_settings.c_oflag = 0;

	/*
	          ICANON  : enable canonical input
	          disable all echo functionality, and don't send signals to calling program
	 */
	port_settings.c_lflag = ICANON;

	/*
	          now clean the modem line and activate the settings for the port
	 */
	tcflush(tty_fd, TCIFLUSH);
	tcsetattr(tty_fd, TCSANOW, &port_settings);

	connected = true;
	return 1;
}

void RoboUART::writeData(string& command)
{
	const char *buffer = command.c_str();
	unsigned int n_write = 0;

	while (n_write != strlen(buffer))
	{
		n_write += write(tty_fd, buffer + n_write, strlen(buffer));
	}

	cout << "**write-uart**" << command << "****" << endl;
}

bool RoboUART::readData()
{
	char buffer[128];
	int n_read;

	n_read = read(tty_fd, buffer, 127);

	if (n_read > 0)
	{
		buffer[n_read] = '\0';
		recvBuffer = recvBuffer.append(buffer);
		return true;
	}
	else
		return false;
}
