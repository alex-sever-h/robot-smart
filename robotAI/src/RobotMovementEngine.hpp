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

class RobotMovementEngine{
	RobotModel *physicalRobot;
	RoboBT     *robotInterface;

	thread	   *stopThread;
	thread	   *updaterThread;

	volatile tyRobotState      robotMovementState;
	posix_time::ptime movementStartTime;
	posix_time::ptime movementLastUpdateTime;

	int32_t		  movementDuration;

private:
	int moveToMilisecs(int distance);
	int rotateToMilisecs(float theta);

	int msToDistance(int ms);
	float msToRotation(int ms);

public:
	RobotMovementEngine(RobotModel *pR, RoboBT *btInt);

	void setPhysicalRobot(RobotModel* physicalRobot);

	void setRobotInterface(RoboBT* robotInterface);

	int move(int distance);
	int rotate(float theta);
	void stop();

	void movementEndThread();

	void positionUpdater();

	void updatePhysicalRobot(int distance, float thetaDeviance);

};



#endif /* ROBOTMOVEMENTENGINE_HPP_ */
