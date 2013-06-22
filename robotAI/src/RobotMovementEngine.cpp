#include "RobotMovementEngine.hpp"
/*
 * RobotMovementEngine.cpp
 *
 *  Created on: Jun 3, 2013
 *      Author: alseh
 */

#include "RobotMovementEngine.hpp"
#include <iostream>

#define DEFAULT_ROT_TIMING 1200.0
#define DEFAULT_MOV_TIMING 1930.0

using namespace std;

#define MOVEMENT_RESOLUTION_MS 5

RobotMovementEngine::RobotMovementEngine(RobotModel* pR, RoboBT* btInt)
{
	this->physicalRobot = pR;
	this->robotInterface = btInt;

	this->movementDuration = 0;

	this->stopThread = NULL;
	this->updaterThread = NULL;

	this->pathFollowerThread = NULL;
	this->pathRemaining = NULL;

	this->rotationTiming = DEFAULT_ROT_TIMING;
	this->movementTiming = DEFAULT_MOV_TIMING;

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
	//	cout << " BTMOVE: " << distanceMM << endl;
	if (robotMovementState == STANDBY)
	{
		if (updaterThread)
			delete updaterThread;

		movementDuration = moveToMilisecs(distanceMM);

		if (distanceMM > 0)
		{
			robotMovementStateNext = MOVING_FORWARD;
			robotInterface->move_by_time(+movementDuration);

			stopThread = new thread(&RobotMovementEngine::movementEndThread,
					this, movementDuration);
			//			cout << "startUpdater  ==== " << updaterThread << endl;
		}
		else if (distanceMM < 0)
		{
			robotMovementStateNext = MOVING_BACKWARD;
			robotInterface->move_by_time(-movementDuration);

			stopThread = new thread(&RobotMovementEngine::movementEndThread,
					this, movementDuration);
			//			cout << "startUpdater  ==== " << updaterThread << endl;
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
			robotMovementStateNext = ROTATING_LEFT;
			robotInterface->rotate_by_time(+movementDuration);

			stopThread = new thread(&RobotMovementEngine::movementEndThread,
					this, movementDuration);
			//			cout << "startUpdater rl  ==== " << updaterThread << endl;
		}
		else if (theta < 0)
		{
			robotMovementStateNext = ROTATING_RIGHT;
			robotInterface->rotate_by_time(-movementDuration);

			stopThread = new thread(&RobotMovementEngine::movementEndThread,
					this, movementDuration);
			//			cout << "startUpdater rr  ==== " << updaterThread << endl;
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
	if(updaterThread)
		updaterThread->interrupt();
	if(stopThread)
		stopThread->interrupt();

	robotStateMutex.unlock();
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

		//		cout << "------------ PERIODICAL UPDATE ---------------\n";

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

	//wait movement to begin
	while(robotMovementState == STANDBY)
		this_thread::sleep(posix_time::milliseconds(MOVEMENT_RESOLUTION_MS));

	//sleep thread while moving
	while(robotMovementState != STANDBY)
	{
		//sleep thread while moving
		this_thread::sleep(posix_time::milliseconds(MOVEMENT_RESOLUTION_MS));
	}
	robotStateMutex.unlock();

	//stop updater thread
	updaterThread->interrupt();

	//	cout << "------------ final UPDATE ---------------\n";

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

	reduceRotation(&theta);

	physicalRobot->setPosition(x, y, theta);
}

void RobotMovementEngine::updatePhysicalRobot(Location loc, float distance, float thetaDeviance)
{
	loc.x     += cos(loc.theta) * distance;
	loc.y     += sin(loc.theta) * distance;
	loc.theta += thetaDeviance;

	reduceRotation(&loc.theta);
	//	cout << loc.x << "x" << loc.y << "@" << loc.theta << endl;
	physicalRobot->setPosition(loc);
}

int RobotMovementEngine::moveToMilisecs(float distanceCm)
{
	return fabs((float) distanceCm) * movementTiming / 1000;
}

int RobotMovementEngine::rotateToMilisecs(float theta)
{
	return fabs(theta) * rotationTiming / M_PI;
}

float RobotMovementEngine::msToDistance(int ms)
{
	return ms * 1000 / movementTiming;
}

float RobotMovementEngine::msToRotation(int ms)
{
	return (float) ms * M_PI / rotationTiming;
}


void RobotMovementEngine::pathFollowerMethod()
{
	while (pathRemaining != NULL)
	{
		//wait robot to finish last movement
		while (robotMovementState != STANDBY)
			;

		//get next robot movement if available
		if (pathRemaining && pathRemaining->children.size() != 0)
		{
			PathNode *nextNode = *pathRemaining->children.begin();

			float rotation;
			float movement;

			decideRotationMovement( pathRemaining, nextNode, &rotation, &movement);

			cout << physicalRobot->getPositionXmm() << " x " << physicalRobot->getPositionYmm()
																			<< " @ " << RAD_TO_DEG(physicalRobot->getOrientationRad()) << endl;

			//			cout << "rotate alpha =========== " << rotation	<< endl;
			rotate(rotation);
			if (stopThread)
			{
				//				cout << "waitUpdater  vvvv " << updaterThread << endl;
				stopThread->join();
				//				cout << "waitUpdater  ^^^^ " << updaterThread << endl;
				stopThread = NULL;
			}

			cout << physicalRobot->getPositionXmm() << " x " << physicalRobot->getPositionYmm()
																						<< " @ " << RAD_TO_DEG(physicalRobot->getOrientationRad()) << endl;

			move(movement);
			if (stopThread)
			{
				//				cout << "waitUpdater  vvvv " << updaterThread << endl;
				stopThread->join();
				//				cout << "waitUpdater  ^^^^ " << updaterThread << endl;
				stopThread = NULL;
			}

			cout << physicalRobot->getPositionXmm() << " x " << physicalRobot->getPositionYmm()
																						<< " @ " << RAD_TO_DEG(physicalRobot->getOrientationRad()) << endl;

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
	if(this->pathRemaining)
		this->pathFollowerThread = new thread( &RobotMovementEngine::pathFollowerMethod, this);
}

void RobotMovementEngine::reduceRotation(float* rotation)
{
	//reduce rotations to max a full circle around
	while (*rotation > 2*M_PI)
		*rotation -= 2 * M_PI;
	while (*rotation < -2*M_PI)
		*rotation += 2 * M_PI;
}

void RobotMovementEngine::decideRotationMovement(PathNode *origin,
		PathNode *target, float *rotation, float *movement)
{
	//calculate raw commands
	*rotation = target->theta - physicalRobot->getOrientationRad();
	*movement = euclidDistance(target->x, target->y, physicalRobot->getPositionXmm(), physicalRobot->getPositionYmm());

	cout << "FROM: " << RAD_TO_DEG(*rotation) << endl;

	//reduce rotations to max a full circle around
	reduceRotation(rotation);

	//decide clockwise or counter-clockwise
	if(*rotation > M_PI)
		*rotation = -(2*M_PI - *rotation);
	if(*rotation < -M_PI)
		*rotation = 2*M_PI + *rotation;

	cout << "to: " << RAD_TO_DEG(*rotation) << endl;

	if(*rotation > M_PI/2)
	{
		*rotation = -(M_PI - *rotation);
		*movement = -*movement;
	}
	if(*rotation < -M_PI/2)
	{
		*rotation = -(-M_PI - *rotation);
		*movement = -*movement;
	}

	cout << "TOOO: " << RAD_TO_DEG(*rotation) << endl;
}

void RobotMovementEngine::acknowledgeCommand(void)
{
	robotStateMutex.lock();
	robotMovementState = robotMovementStateNext;
	robotStateMutex.unlock();
	cout << "..... rme acknowledged \n";
}

void RobotMovementEngine::finalizeCommand(void)
{
	robotStateMutex.lock();
	robotMovementState = STANDBY;
	robotStateMutex.unlock();
	cout << "..... rme finalized \n";
}

void RobotMovementEngine::interruptPathFollowing()
{
	if(updaterThread)
		updaterThread->interrupt();
	if(stopThread)
		stopThread->interrupt();
	if (pathFollowerThread)
	{
		pathFollowerThread->interrupt();

		this_thread::sleep(posix_time::milliseconds(MOVEMENT_RESOLUTION_MS));
		posix_time::time_duration msdiff =
				posix_time::microsec_clock::universal_time() - movementStartTimeStamp;
		updatePhysicalRobot(msdiff.total_milliseconds());
	}
	stopMotion();

	pathRemaining = NULL;
	updaterThread = NULL;
	stopThread = NULL;
	pathFollowerThread = NULL;
}









