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

#define MOVEMENT_RESOLUTION_MS 50

#define TIMING_ROTATION   2200.0
#define TIMING_MOVEMENT   1800.0

RobotMovementEngine::RobotMovementEngine(RobotModel* pR, RoboBT* btInt)
{
	this->physicalRobot = pR;
	this->robotInterface = btInt;

	this->movementDuration = 0;

	this->stopThread = NULL;
	this->updaterThread = NULL;

	this->pathFollowerThread = NULL;
	this->pathRemaining = NULL;

	this->robotMovementState = STANDBY;
}

void RobotMovementEngine::setPhysicalRobot(RobotModel* physicalRobot)
{
	this->physicalRobot = physicalRobot;
}

void RobotMovementEngine::setRobotInterface(RoboBT* robotInterface)
{
	this->robotInterface = robotInterface;
}

int RobotMovementEngine::move(int distanceMM)
{
	cout << " BTMOVE: " << distanceMM << endl;
	if (robotMovementState == STANDBY)
	{
		if (updaterThread)
			delete updaterThread;

		movementDuration = moveToMilisecs(distanceMM);

		if (distanceMM > 0)
		{
			robotInterface->move(999);
			robotMovementState = MOVING_FORWARD;
			stopThread = new thread(&RobotMovementEngine::movementEndThread,
					this, movementDuration);
			cout << "startUpdater  ==== " << updaterThread << endl;
		}
		else if (distanceMM < 0)
		{
			robotInterface->move(-999);
			robotMovementState = MOVING_BACKWARD;
			stopThread = new thread(&RobotMovementEngine::movementEndThread,
					this, movementDuration);
			cout << "startUpdater  ==== " << updaterThread << endl;
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

int RobotMovementEngine::rotate(float theta)
{
	if (robotMovementState == STANDBY)
	{
		if (updaterThread)
			delete updaterThread;

		movementDuration = rotateToMilisecs(theta);

		if (theta > 0)
		{
			robotInterface->rotate(RAD_TO_DEG(theta));
			robotMovementState = ROTATING_LEFT;
			stopThread = new thread(&RobotMovementEngine::movementEndThread,
					this, movementDuration);
			cout << "startUpdater rl  ==== " << updaterThread << endl;
		}
		else if (theta < 0)
		{
			robotInterface->rotate(RAD_TO_DEG(theta));
			robotMovementState = ROTATING_RIGHT;
			stopThread = new thread(&RobotMovementEngine::movementEndThread,
					this, movementDuration);
			cout << "startUpdater rr  ==== " << updaterThread << endl;
		}
		else
		{
			robotInterface->move(0);
			robotMovementState = STANDBY;
		}

		movementStartTimeStamp = posix_time::microsec_clock::local_time();
		movementLastUpdateTime = posix_time::microsec_clock::local_time();

		return 1;
	}
	else
		return 0;
}

void RobotMovementEngine::stopMotion()
{
	cout << " BTSTOP !" << endl;
	if(updaterThread)
		updaterThread->interrupt();
	if(stopThread)
		stopThread->interrupt();
	robotInterface->move(0);
	robotMovementState = STANDBY;
}

void RobotMovementEngine::updatePhysicalRobot(int passedMs)
{
	switch (robotMovementState)
	{
	case MOVING_FORWARD:
		updatePhysicalRobot(movementInitialLocation, +msToDistance(passedMs), 0);
		break;
	case MOVING_BACKWARD:
		updatePhysicalRobot(movementInitialLocation, -msToDistance(passedMs), 0);
		break;
	case ROTATING_LEFT:
		updatePhysicalRobot(movementInitialLocation, 0, +msToRotation(passedMs));
		break;
	case ROTATING_RIGHT:
		updatePhysicalRobot(movementInitialLocation, 0, -msToRotation(passedMs));
		break;
	default:
		break;
	}
}

void RobotMovementEngine::positionUpdater()
{
	//run unstopped because movementEndThread will kill me
	while(1)
	{
		this_thread::sleep(posix_time::milliseconds(MOVEMENT_RESOLUTION_MS));

		posix_time::time_duration msdiff =
				posix_time::microsec_clock::universal_time() - movementStartTimeStamp;

		cout << "------------ PERIODICAL UPDATE ---------------\n";

		updatePhysicalRobot(msdiff.total_milliseconds());
	}
}

void RobotMovementEngine::movementEndThread(int totalMovementDuration)
{
	//set movement start trackers
	movementInitialLocation = physicalRobot->getPosition();
	movementStartTimeStamp = posix_time::microsec_clock::universal_time();

	//start updater thread
	updaterThread = new thread(&RobotMovementEngine::positionUpdater, this);

	//sleep thread while moving
	this_thread::sleep(posix_time::milliseconds(totalMovementDuration));

	//stop updater thread
	updaterThread->interrupt();

	cout << "------------ final UPDATE ---------------\n";

	//update with final precise position
	updatePhysicalRobot(totalMovementDuration);

	//stop robot from moving
	this->stopMotion();
}

void RobotMovementEngine::updatePhysicalRobot(float distance, float thetaDeviance)
{
	float x, y;
	float theta;

	//	cout << "update " << distance << endl;

	physicalRobot->getPosition(&x, &y, &theta);

	x += cos(theta) * distance;
	y += sin(theta) * distance;
	theta += thetaDeviance;

	physicalRobot->setPosition(x, y, theta);
}

void RobotMovementEngine::updatePhysicalRobot(Location loc, float distance, float thetaDeviance)
{
	loc.x     += cos(loc.theta) * distance;
	loc.y     += sin(loc.theta) * distance;
	loc.theta += thetaDeviance;

	cout << loc.x << "x" << loc.y << "@" << loc.theta << endl;
	physicalRobot->setPosition(loc);
}

int RobotMovementEngine::moveToMilisecs(float distanceCm)
{
	return fabs((float) distanceCm) * TIMING_MOVEMENT / 1000;
}

int RobotMovementEngine::rotateToMilisecs(float theta)
{
	return fabs(theta) * TIMING_ROTATION / M_PI;
}

float RobotMovementEngine::msToDistance(int ms)
{
	return ms * 1000 / TIMING_MOVEMENT;
}

float RobotMovementEngine::msToRotation(int ms)
{
	return (float) ms * M_PI / TIMING_ROTATION;
}


void RobotMovementEngine::pathFollowerMethod()
{
	while (pathRemaining != NULL)
	{
		//wait robot to finish last movement
		while (robotMovementState != STANDBY)
			;

		//get next robot movement if available
		if (pathRemaining->children.size() != 0)
		{
			PathNode *nextNode = *pathRemaining->children.begin();

			cout << "rotate alpha =========== "
					<< RAD_TO_DEG(nextNode->theta - physicalRobot->getOrientationRad())
					<< endl;
			rotate(nextNode->theta - physicalRobot->getOrientationRad());
			if (stopThread)
			{
				cout << "waitUpdater  vvvv " << updaterThread << endl;
				stopThread->join();
				cout << "waitUpdater  ^^^^ " << updaterThread << endl;
				stopThread = NULL;
			}

			move(
					euclidDistance(nextNode->x, nextNode->y, pathRemaining->x,
							pathRemaining->y));
			if (stopThread)
			{
				cout << "waitUpdater  vvvv " << updaterThread << endl;
				stopThread->join();
				cout << "waitUpdater  ^^^^ " << updaterThread << endl;
				stopThread = NULL;
			}

			//advance to next step
			pathRemaining = nextNode;
		}
		else
		{
			//got to goal !!!!!! yepee
			cout << "got to goal !!!!!! yepee" << endl;
			pathRemaining = NULL;
			pathFollowerThread = NULL;
			return;
		}
	}
}


void RobotMovementEngine::followPath(PathNode* path)
{
	if(pathFollowerThread)
		interruptPathFollowing();
	this->pathRemaining = path;
	this->pathFollowerThread = new thread( &RobotMovementEngine::pathFollowerMethod, this);
}


void RobotMovementEngine::interruptPathFollowing()
{
	if(updaterThread)
		updaterThread->interrupt();
	if(stopThread)
		stopThread->interrupt();
	if (pathFollowerThread)
		pathFollowerThread->interrupt();

	this_thread::sleep(posix_time::milliseconds(MOVEMENT_RESOLUTION_MS));
	posix_time::time_duration msdiff =
			posix_time::microsec_clock::universal_time() - movementStartTimeStamp;
	updatePhysicalRobot(msdiff.total_milliseconds());

	stopMotion();

	pathRemaining = NULL;
	updaterThread = NULL;
	stopThread = NULL;
	pathFollowerThread = NULL;
}









