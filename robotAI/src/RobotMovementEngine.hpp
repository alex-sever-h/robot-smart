/*
 * RobotMovementEngine.hpp
 *
 *  Created on: May 30, 2013
 *      Author: alseh
 */

#ifndef ROBOTMOVEMENTENGINE_HPP_
#define ROBOTMOVEMENTENGINE_HPP_

#include <boost/date_time.hpp>
#include <boost/thread.hpp>
#include "RoboBT.h"
#include "robotModel.hpp"
#include "AStarPathfinder.hpp"
#include "Location.hpp"

using namespace boost;

typedef enum {
	STANDBY,
	MOVING_FORWARD,
	MOVING_BACKWARD,
	ROTATING_LEFT,
	ROTATING_RIGHT
} tyRobotState;

class RobotModel;
class RoboBT;
class PathNode;

class RobotMovementEngine{
	RobotModel *physicalRobot;
	RoboBT     *robotInterface;

	float rotationTiming;
	float movementTiming;

	Location   movementInitialLocation;

	thread	   *stopThread;
	thread	   *updaterThread;

	thread		*pathFollowerThread;

	volatile tyRobotState      robotMovementState;
	posix_time::ptime 			movementStartTimeStamp;
	posix_time::ptime 			movementLastUpdateTime;

	int32_t		  movementDuration;

	PathNode *pathRemaining;

	int moveToMilisecs(float distance);
	int rotateToMilisecs(float theta);

	void decideRotationMovement(PathNode *origin, PathNode *target, float *rotation, float *movement);

	float msToDistance(int ms);
	float msToRotation(int ms);
	void updatePhysicalRobot(int passedMs);
	void reduceRotation(float* rotation);

public:
	RobotMovementEngine(RobotModel *pR, RoboBT *btInt);

	void setPhysicalRobot(RobotModel* physicalRobot);

	void setRobotInterface(RoboBT* robotInterface);

	int move(int distance);
	int rotate(float theta);
	void stopMotion();

	void movementEndThread(int msRemaining);
	void positionUpdater();

	void updatePhysicalRobot(float distance, float thetaDeviance);
	void updatePhysicalRobot(Location oldLocation, float distance, float thetaDeviance);

	void followPath(PathNode *path);

	void pathFollowerMethod();
	void interruptPathFollowing();

	float getMovementTiming() const
	{
		return movementTiming;
	}

	void setMovementTiming(float movementTiming)
	{
		this->movementTiming = movementTiming;
	}

	float getRotationTiming() const
	{
		return rotationTiming;
	}

	void setRotationTiming(float rotationTiming)
	{
		this->rotationTiming = rotationTiming;
	}
};



#endif /* ROBOTMOVEMENTENGINE_HPP_ */
