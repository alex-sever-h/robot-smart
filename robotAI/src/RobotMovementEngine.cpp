#include "RobotMovementEngine.hpp"
/*
 * RobotMovementEngine.cpp
 *
 *  Created on: Jun 3, 2013
 *      Author: alseh
 */

#include "RobotMovementEngine.hpp"
#include <iostream>

using namespace std;

#define MOVEMENT_RESOLUTION_MS 33

RobotMovementEngine::RobotMovementEngine(RobotModel* pR, RoboBT* btInt) {
	this->physicalRobot = pR;
	this->robotInterface = btInt;

	this->movementDuration = 0;
	this->stopThread = NULL;
	this->updaterThread = NULL;

	this->robotMovementState = STANDBY;
}

void RobotMovementEngine::setPhysicalRobot(RobotModel* physicalRobot) {
	this->physicalRobot = physicalRobot;
}

void RobotMovementEngine::setRobotInterface(RoboBT* robotInterface) {
	this->robotInterface = robotInterface;
}

int RobotMovementEngine::move(int distanceMM) {
//	cout << robotMovementState << endl;
	if(robotMovementState == STANDBY)
	{
		if(updaterThread) delete updaterThread;

		movementDuration = moveToMilisecs(distanceMM);

		if(distanceMM > 0)
		{
			robotInterface->move(999);
			robotMovementState = MOVING_FORWARD;
			updaterThread = new thread( &RobotMovementEngine::positionUpdater, this );
		}
		else if(distanceMM < 0)
		{
			robotInterface->move(-999);
			robotMovementState = MOVING_BACKWARD;
			updaterThread = new thread( &RobotMovementEngine::positionUpdater, this );
		}
		else
		{
			robotInterface->move(0);
			robotMovementState = STANDBY;
		}

		return 1;
	}
	else
		return 0;
}

int RobotMovementEngine::rotate(float theta){
	if(robotMovementState == STANDBY)
	{
		if(updaterThread) delete updaterThread;

		movementDuration = rotateToMilisecs(theta);

		if(theta > 0)
		{
			robotInterface->rotate(RAD_TO_DEG(theta));
			robotMovementState = ROTATING_LEFT;
			updaterThread = new thread(&RobotMovementEngine::positionUpdater, this);
		}
		else if(theta < 0)
		{
			robotInterface->rotate(RAD_TO_DEG(theta));
			robotMovementState = ROTATING_RIGHT;
			updaterThread = new thread(&RobotMovementEngine::positionUpdater, this);
		}
		else
		{
			robotInterface->move(0);
			robotMovementState = STANDBY;
		}

		movementStartTime      = posix_time::microsec_clock::local_time();
		movementLastUpdateTime = posix_time::microsec_clock::local_time();

		return 1;
	}
	else
		return 0;
}

void RobotMovementEngine::stop() {
	robotInterface->move(0);
	robotMovementState = STANDBY;
}

void RobotMovementEngine::positionUpdater() {
	do
	{
		int remainderMs;

		this_thread::sleep(posix_time::milliseconds(MOVEMENT_RESOLUTION_MS));
		movementDuration -= MOVEMENT_RESOLUTION_MS;

		if(movementDuration > 0)
			remainderMs = MOVEMENT_RESOLUTION_MS;
		else
			remainderMs = MOVEMENT_RESOLUTION_MS + movementDuration;

		switch(robotMovementState)
		{
		case MOVING_FORWARD:
			updatePhysicalRobot(+msToDistance(remainderMs), 0);
			break;
		case MOVING_BACKWARD:
			updatePhysicalRobot(-msToDistance(remainderMs), 0);
			break;
		case ROTATING_LEFT:
			updatePhysicalRobot(0, +msToRotation(remainderMs));
			break;
		case ROTATING_RIGHT:
			updatePhysicalRobot(0, -msToRotation(remainderMs));
			break;
		default:
			break;
		}

//		cout << "remaining: " << movementDuration << endl;
	}while(movementDuration > 0);

	this->stop();
	this->robotMovementState = STANDBY;
}

void RobotMovementEngine::movementEndThread() {
	this_thread::sleep(posix_time::milliseconds(movementDuration));

	robotInterface->move(0);
	robotMovementState = STANDBY;
}

void RobotMovementEngine::updatePhysicalRobot(int distance, float thetaDeviance)
{
	int x, y;
	float theta;

//	cout << "update " << distance << endl;

	physicalRobot->getPosition(&x, &y, &theta);

	x += cos(theta) * distance;
	y += sin(theta) * distance;
	theta += thetaDeviance;

	physicalRobot->setPosition(x, y, theta);
}


int RobotMovementEngine::moveToMilisecs(int distanceCm) {
	return abs(distanceCm) * 1800 / 1000;
}

int RobotMovementEngine::rotateToMilisecs(float theta) {
	return fabs(theta) * 1700 / M_PI;
}

int RobotMovementEngine::msToDistance(int ms) {
	return ms * 1000 / 1800;
}

float RobotMovementEngine::msToRotation(int ms) {
	return (float)ms * M_PI / 1700;
}



