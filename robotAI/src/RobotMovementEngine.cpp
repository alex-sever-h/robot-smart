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

RobotMovementEngine::RobotMovementEngine(RobotModel* pR, RoboInterface* btInt)
{
	this->physicalRobot = pR;
	this->robotInterface = btInt;

	this->movementDuration = 0;

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

void RobotMovementEngine::setRobotInterface(RoboInterface* robotInterface)
{
	this->robotInterface = robotInterface;
}

int RobotMovementEngine::move(int distanceMM)
{

	movementDuration = moveToMilisecs(distanceMM);
	movementInitialLocation = physicalRobot->getPosition();

	updatePrev = 0;

	//waitACK = true;
	waitRDY = true;

	if (movementDuration != 0 && distanceMM > 0)
	{
		robotMovementState = MOVING_FORWARD;
		robotInterface->move_by_time(+movementDuration);
	}
	else if (movementDuration != 0 && distanceMM < 0)
	{
		robotMovementState = MOVING_BACKWARD;
		robotInterface->move_by_time(-movementDuration);
	}
	else
	{
		robotMovementState = STANDBY;
		robotInterface->stopMovement();
	}

	return 1;
}

int RobotMovementEngine::rotate(float theta)
{

	movementDuration = rotateToMilisecs(theta);
	movementInitialLocation = physicalRobot->getPosition();

	updatePrev = 0;

	//waitACK = true;
	waitRDY = true;

	if (movementDuration != 0 && theta > 0)
	{
		robotMovementState = ROTATING_LEFT;
		robotInterface->rotate_by_time(+movementDuration);

	}
	else if (movementDuration != 0 && theta < 0)
	{
		robotMovementState = ROTATING_RIGHT;
		robotInterface->rotate_by_time(-movementDuration);
	}
	else
	{
		robotMovementState = STANDBY;
		robotInterface->stopMovement();
	}

	return 1;
}

void RobotMovementEngine::stopMotion()
{

	//waitACK = true;
	//waitRDY = true;

	robotMovementState = STANDBY;

	//do{
	robotInterface->stopMovement();
	robotInterface->stopMovement();
	//waitAcknowledge();
	//} while (waitACK);
	//while(waitRDY);
}

void RobotMovementEngine::updatePhysicalRobot(int passedMs)
{
	if(passedMs <= updatePrev)
		return;

	cout << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> UPDATE " << passedMs << endl;
	switch (robotMovementState)
	{
	case MOVING_FORWARD:
		cout << "---------------- update move forward" << endl;
		updatePhysicalRobot(movementInitialLocation, +msToDistance(passedMs), 0);
		break;
	case MOVING_BACKWARD:
		cout << "---------------- update move backward" << endl;
		updatePhysicalRobot(movementInitialLocation, -msToDistance(passedMs), 0);
		break;
	case ROTATING_LEFT:
		cout << "---------------- update rotation left" << endl;
		updatePhysicalRobot(movementInitialLocation, 0, +msToRotation(passedMs));
		break;
	case ROTATING_RIGHT:
		cout << "---------------- update rotation right" << endl;
		updatePhysicalRobot(movementInitialLocation, 0, -msToRotation(passedMs));
		break;
	default:
		cout << "---------------- update with 0" << endl;
		updatePhysicalRobot(physicalRobot->getPosition(), 0, 0);
		break;
	}
}

void RobotMovementEngine::updatePhysicalRobot(float distance, float thetaDeviance)
{
	float x, y;
	float theta;

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
	cout << "-----------------------------START  PATHFOLLOWING-------------------------" << endl;
	while (pathRemaining != NULL)
	{
		//wait robot to finish last movement
//		while (robotMovementState != STANDBY)
//			;

		//get next robot movement if available
		if (pathRemaining && pathRemaining->children.size() != 0)
		{
			PathNode *nextNode = *pathRemaining->children.begin();

			float rotation;
			float movement;

			decideRotationMovement( pathRemaining, nextNode, &rotation, &movement);

			cout << "-----0-----" << physicalRobot->getPositionXmm() << " x " << physicalRobot->getPositionYmm() << " @ " << RAD_TO_DEG(physicalRobot->getOrientationRad()) << endl;
			rotate(rotation);
			boost::this_thread::sleep(boost::posix_time::milliseconds(movementDuration + 100));
			if (waitRDY)
			{
				stopMotion();
				waitRDY = false;
			}
			cout << "-----1-----" << physicalRobot->getPositionXmm() << " x " << physicalRobot->getPositionYmm() << " @ " << RAD_TO_DEG(physicalRobot->getOrientationRad()) << endl;


			move(movement);
			boost::this_thread::sleep(boost::posix_time::milliseconds(movementDuration + 100));
			if (waitRDY){
				stopMotion();
				waitRDY = false;
			}
			cout << "-----2-----" << physicalRobot->getPositionXmm() << " x " << physicalRobot->getPositionYmm()
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

			cout << "-----------------------------END    PATHFOLLOWING-------------------------" << endl;

			return;
		}
	}
}


void RobotMovementEngine::followPath(PathNode* path)
{
	if(pathFollowerThread)
		interruptPathFollowing();
	else
	{
		this->pathRemaining = path;
		if(this->pathRemaining)
			this->pathFollowerThread = new thread( &RobotMovementEngine::pathFollowerMethod, this);
	}
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

	//cout << "FROM: " << RAD_TO_DEG(*rotation) << endl;

	//reduce rotations to max a full circle around
	reduceRotation(rotation);

	//decide clockwise or counter-clockwise
	if(*rotation > M_PI)
		*rotation = -(2*M_PI - *rotation);
	if(*rotation < -M_PI)
		*rotation = 2*M_PI + *rotation;

	//cout << "to: " << RAD_TO_DEG(*rotation) << endl;

//	if(*rotation > M_PI/2)
//	{
//		*rotation = -(M_PI - *rotation);
//		*movement = -*movement;
//	}
//	if(*rotation < -M_PI/2)
//	{
//		*rotation = -(-M_PI - *rotation);
//		*movement = -*movement;
//	}

	//cout << "TOOO: " << RAD_TO_DEG(*rotation) << endl;
}

#define ACK_TIMEOUT 200

bool RobotMovementEngine::waitAcknowledge(void)
{
	posix_time::ptime wStart = posix_time::microsec_clock::universal_time();

	//	while (waitACK)
	//	{
	//		posix_time::time_duration msdiff =
	//				posix_time::microsec_clock::universal_time() - wStart;
	//		if(msdiff.total_milliseconds() > ACK_TIMEOUT)
	//			break;
	//	}
	//
	//	return !waitACK;
}

void RobotMovementEngine::acknowledgeCommand(void)
{
//	robotStateMutex.lock();
//	robotMovementState = robotMovementState;
//	robotStateMutex.unlock();
//
//	//waitACK = false;
//
//	cout << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> rme acknowledged \n";
}

void RobotMovementEngine::finalizeCommand(void)
{
//	robotStateMutex.lock();
//	robotMovementState = STANDBY;
//	robotMovementState = STANDBY;
//	robotStateMutex.unlock();
//
	waitRDY = false;
//
//	cout << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> rme finalized \n";
}

void RobotMovementEngine::interruptPathFollowing()
{
	cout << ">>>>INT>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n";
	if (pathFollowerThread)
	{
		pathFollowerThread->interrupt();
	}

	cout << ".................path follower stopped" << endl;

	stopMotion();

	pathRemaining = NULL;
	pathFollowerThread = NULL;



	cout << "<<<< INT finished \n" << endl;
}









