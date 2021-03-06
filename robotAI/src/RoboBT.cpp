#include "RoboBT.hpp"
#include <boost/algorithm/string.hpp>
#include <vector>
#include <string>
#include <unistd.h>
#include "stdio.h"

using namespace std;
using namespace boost;

RoboBT::RoboBT(bool predefined) : RoboInterface()
{
	strcpy(name, "roboBT");
	sock = 0;
	if (predefined)
		strcpy(address, "00:12:6F:27:EF:A8");

	connectRobot();
}

RoboBT::~RoboBT()
{
	close(sock);
}


int RoboBT::searchRobot()
{
	inquiry_info *ii = NULL;
	int max_rsp, num_rsp;
	int dev_id, sock, len, flags;
	int i;
	char addr[19] =
	{ 0 };
	char lName[248] =
	{ 0 };

	if (strlen(address) == 0)
	{

		dev_id = hci_get_route(NULL);
		sock = hci_open_dev(dev_id);
		if (dev_id < 0 || sock < 0)
		{
			perror("opening socket");
			exit(1);
		}

		len = 8;
		max_rsp = 255;
		flags = IREQ_CACHE_FLUSH;
		ii = (inquiry_info*) malloc(max_rsp * sizeof(inquiry_info));

		num_rsp = hci_inquiry(dev_id, len, max_rsp, NULL, &ii, flags);
		if (num_rsp < 0)
			perror("hci_inquiry");

		for (i = 0; i < num_rsp; i++)
		{
			ba2str(&(ii + i)->bdaddr, addr);
			memset(lName, 0, sizeof(lName));
			if (hci_read_remote_name(sock, &(ii + i)->bdaddr, sizeof(lName),
					lName, 0) < 0)
				strcpy(lName, "[unknown]");
			printf("%s  %s\n", addr, lName);

			if (strcmp(name, lName) == 0)
			{
				printf("robot found !!!!\n");
				strcpy(address, addr);
			}
		}

		free(ii);
		close(sock);
	}

	if (strlen(address) == 0)
		return 0;
	else
		return 1;
}

int RoboBT::connectRobot()
{
	int status;

	//try to search the robot
	if (searchRobot() == 0)
		connected = false;

	// allocate a socket
	sock = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);

	// set the connection parameters (who to connect to)
	btSocket.rc_family = AF_BLUETOOTH;
	btSocket.rc_channel = (uint8_t) 1;
	str2ba(address, &btSocket.rc_bdaddr);

	// connect to server
	status = connect(sock, (struct sockaddr *) &btSocket, sizeof(btSocket));
	if (status < 0)
	{
		perror("uh oh");
		connected = false;
		return 0;
	}
	else
	{
		connected = true;
		return 1;
	}
}

void RoboBT::writeData(string &command)
{
	const char *buffer = command.c_str();
	unsigned int n_write = 0;

	while (n_write != strlen(buffer))
	{
		n_write += write(sock, buffer + n_write, strlen(buffer));
	}

		cout << "****" << command << "****" << endl;
}

bool RoboBT::readData()
{
	char buffer[128];
	int n_read;

	n_read = read(sock, buffer, 127);

	if (n_read > 0)
	{
		buffer[n_read] = '\0';
		recvBuffer = recvBuffer.append(buffer);
		return true;
	}
	else
		return false;
}

