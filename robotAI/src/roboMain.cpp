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


#define ENABLE_GUI


using namespace std;
using namespace boost;

MapParticle   		world;
RoboBT 		  		robotBTinterface(true);
RobotModel    		physicalRobot(0, 0, 0, &robotBTinterface);

int flag;

void smartTask()
{

	physicalRobot.placeRobotInMap(&world);

	robotBTinterface.connectRobot();

	while(1)
	{
		robotBTinterface.pollUpdateSensors();


//		if(flag)
//		{
//			if( physicalRobot.move(500) )
//				flag = !flag;
//		}
//		else


//		{
//			if( physicalRobot.rotate(M_PI) )
//				flag = !flag;
//		}
	}
}

void gui_task()
{
	int argc = 1;
	char appname[] = "roboAI";
	char *argv[1];
	argv[0] = appname;

	printf("startinggui\n");
	wxEntry(argc, argv);
}



int main(int argc, char **argv)
{
	thread bt_thread(smartTask);

#ifdef ENABLE_GUI
	thread gui_thread(gui_task);

	gui_thread.join();
#else

	bt_thread.join();
#endif


	return 0;
}









