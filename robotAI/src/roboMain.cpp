#include <stdio.h>
#include <string>
#include "roboGUI.h"
#include "RoboBT.h"
#include "geometricPlane.hpp"
#include "mapDiscrete.hpp"
#include "sensorManagement.h"
#include <iostream>
#include <boost/thread.hpp>


#define ENABLE_GUI


using namespace std;
using namespace boost;

SensorManager sMan;
MapDiscrete   world(1024, 1024);

void bluetooth_task()
{
	RoboBT robot(true);
	robot.setSensMan(&sMan);

	robot.connectRobot();

	while(1)
	{
		robot.pollUpdateSensors();
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



void testPIP()
{
	tyPolygon testPoly;
	testPoly.push_back(LocationWWeight(0, 0, 0));
	testPoly.push_back(LocationWWeight(0, 100, 0));
	testPoly.push_back(LocationWWeight(100, 100, 0));
	testPoly.push_back(LocationWWeight(100, 0, 0));

	LocationWWeight testPointInside(10, 60, 0);
	bool insideOK = insidePolygon(testPointInside, testPoly);
	cout << "ShouldBeTrue : " << insideOK << endl;

	LocationWWeight testPointOutside(50, 110, 0);
	bool insideNOK = insidePolygon(testPointOutside, testPoly);
	cout << "ShouldBeFalse : " << insideNOK << endl;

}



int main(int argc, char **argv)
{
	thread bt_thread(bluetooth_task);


#ifdef ENABLE_GUI
	thread gui_thread(gui_task);

	gui_thread.join();
#else

	bt_thread.join();
#endif


	return 0;
}









