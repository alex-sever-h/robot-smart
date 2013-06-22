#include <stdio.h>
#include <string>
#include "RoboBT.h"
#include "geometricPlane.hpp"
#include "mapDiscrete.hpp"
#include "sensorManagement.h"
#include <iostream>
#include <boost/thread.hpp>
#include "MapParticle.hpp"
#include "robotModel.hpp"
#include "AStarPathfinder.hpp"
#include "Location.hpp"

#include "unistd.h"

#include <boost/asio.hpp>
#include <iostream>
#include "RobotServer.hpp"

#define ENABLE_GUI


using namespace std;
using namespace boost;

MapParticle   		world;
RoboBT 		  		robotBTinterface(true);
RobotModel    		physicalRobot(0, 0, 0, &robotBTinterface);

int flag;


void smartTask()
{
	RobotServer rs;
	cout << "-----------server initialized-----------\n";

	physicalRobot.setSrv(&rs);
	physicalRobot.placeRobotInMap(&world);

	robotBTinterface.connectRobot();
	robotBTinterface.startSensorPoller();

	sleep(1);

	physicalRobot.moveAtLocation(Location(1000, 0, 30));

#if 0
	sleep(1);
	physicalRobot.move(1050);
#endif

#if 0
	sleep(1);
	physicalRobot.rotate(DEG_TO_RAD(360));
#endif

	while(1);
}

int main(int argc, char **argv)
{

	smartTask();

	return 0;
}









