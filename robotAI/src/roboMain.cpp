#include <stdio.h>
#include <string>
#include "roboGUI.h"
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

#include <boost/asio.hpp>
#include <iostream>
#include "RobotClient.hpp"
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

	//
	//	float a = 0, b = 0, c = 0;
	//	while(1)
	//	{
	//		a+=1;
	//		b+=10;
	//		c+=100;
	//		sleep(1);
	//		physicalRobot.sendRobotInfo();
	//		physicalRobot.setPosition(a, b, c);
	//	}
	//rs.stop();


	physicalRobot.placeRobotInMap(&world);

	robotBTinterface.connectRobot();
	robotBTinterface.startSensorPoller();

	sleep(15);

	physicalRobot.moveAtLocation(Location(1000, 0, 30));


#if 0
	sleep(1);
	physicalRobot.move(1000);
#endif

#if 0
	sleep(1);
	physicalRobot.rotate(DEG_TO_RAD(360));
#endif

	while(1)
	{

	}
}

void gui_task()
{
	printf("startinggui\n");

	{
		int argc = 1;
		char appname[] = "roboAI";
		char *argv[1];
		argv[0] = appname;
		wxEntry(argc, argv);
	}
}


int main(int argc, char **argv)
{

	thread smartThread(smartTask);

#ifdef ENABLE_GUI
	thread gui_thread(gui_task);

	gui_thread.join();
#else

	smartThread.join();
#endif


	return 0;
}









