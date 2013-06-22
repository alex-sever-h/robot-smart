#include <stdio.h>
#include <string>
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

#include "RoboInterface.hpp"
#include "RoboBT.hpp"
#include "RoboUART.hpp"

#define ENABLE_GUI

using namespace std;
using namespace boost;

MapParticle world;
RoboInterface *robotInterface = new RoboBT(true);
RobotModel physicalRobot(0, 0, 0, robotInterface);

int flag;

void smartTask()
{
	RobotServer rs(&physicalRobot);
	cout << "-----------server initialized-----------\n";

	physicalRobot.setSrv(&rs);
	physicalRobot.placeRobotInMap(&world);

	robotInterface->startSensorPoller();

#if 0
	sleep(1);
	physicalRobot.move(1050);
#endif

#if 0
	sleep(1);
	physicalRobot.rotate(DEG_TO_RAD(360));
#endif

	while (1)
		;
}

int main(int argc, char **argv)
{

	smartTask();

	return 0;
}

