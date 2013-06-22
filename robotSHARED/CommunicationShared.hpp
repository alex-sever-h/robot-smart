/*
 * CommunicationShared.hpp
 *
 *  Created on: Jun 22, 2013
 *      Author: alseh
 */

#ifndef COMMUNICATIONSHARED_HPP_
#define COMMUNICATIONSHARED_HPP_

#include <string>

using namespace std;


class CommSettings{
	const static int port = 50000;

public:
	static string getHostname()
	{
		return string("localhost");
	}

	static string getPortString()
	{
		return string("50000");
	}

	static int getPortInt()
	{
		return port;
	}

};


enum dataType{
	ROBOT_POSITION = 0x01,
	FULL_PATH = 0x02,
	FULL_MAP  = 0x03,
	SENSOR_READING = 0x04,
};


#endif /* COMMUNICATIONSHARED_HPP_ */
