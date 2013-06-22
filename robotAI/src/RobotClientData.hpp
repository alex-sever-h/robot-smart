/*
 * RobotClientData.hpp
 *
 *  Created on: Jun 21, 2013
 *      Author: alseh
 */

#ifndef ROBOTCLIENTDATA_HPP_
#define ROBOTCLIENTDATA_HPP_

#include "protobuf/robotdata.pb.h"
#include <list>
#include <boost/thread.hpp>

using namespace std;

class RobotClientData{

public:

	boost::mutex rInfoMutex;
	boost::mutex mInfoMutex;
	boost::mutex pInfoMutex;
	boost::mutex sInfoMutex;

	robotdata::RobotInfo   robotInfo;
	robotdata::FullMap     mapInfo;
	robotdata::FullPath    pathInfo;
	list<robotdata::SensorInfo*> sensorsInfo;

	void updateFromBuffer(void *receivedData, int nBytes, int dataType);
};


#endif /* ROBOTCLIENTDATA_HPP_ */
