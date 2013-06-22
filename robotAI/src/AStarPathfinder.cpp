/*
 * AStarPathfinder.cpp
 *
 *  Created on: Jun 8, 2013
 *      Author: alseh
 */


#include "AStarPathfinder.hpp"
#include <math.h>
#include "protobuf/robotdata.pb.h"

#define STEP_ROTATION  (DEG_TO_RAD(10))
#define STEP_MOVEMENT  (400)
#define COST_MOVEMENT	(1.0)
#define COST_ROTATION	(1.0)

#define GOAL_PRECISION (10)
#define COLLISION_FACTOR (200)
#define PATH_COLLISION_LIMIT (80)

#define MAX_ASTAR_STEPS (100)

AStarPathfinder::AStarPathfinder(RobotModel* physicalRobot)
{
	this->world  = NULL;
	this->pRobot = physicalRobot;
}

AStarPathfinder::~AStarPathfinder() {
}


tyPolygon* AStarPathfinder::generatePathArea(LocationWWeight start,	LocationWWeight end, float theta)
{
	tyPolygon *pathArea = new tyPolygon;
	LocationWWeight helper;

	//compute start points
	helper.x = start.x + (pRobot->getWidthMm()/2) * sin(theta + M_PI/2);
	helper.y = start.y + (pRobot->getWidthMm()/2) * cos(theta + M_PI/2);
	pathArea->push_back(helper);

	helper.x = start.x + (pRobot->getWidthMm()/2) * sin(theta - M_PI/2);
	helper.y = start.y + (pRobot->getWidthMm()/2) * cos(theta - M_PI/2);
	pathArea->push_back(helper);

	//compute end points
	helper.x = end.x + (pRobot->getWidthMm()/2) * sin(theta - M_PI/2);
	helper.y = end.y + (pRobot->getWidthMm()/2) * cos(theta - M_PI/2);
	pathArea->push_back(helper);

	helper.x = end.x + (pRobot->getWidthMm()/2) * sin(theta + M_PI/2);
	helper.y = end.y + (pRobot->getWidthMm()/2) * cos(theta + M_PI/2);
	pathArea->push_back(helper);

	return pathArea;
}

tyPolygon* AStarPathfinder::generatePathArea(float xStart, float yStart, float xEnd, float yEnd, float theta)
{
	tyPolygon *pathArea = new tyPolygon;
	LocationWWeight helper;

	//compute start points
	helper.x = xStart + (-pRobot->getLengthMm()/2) * cos(theta) - (+pRobot->getWidthMm()+20/2) * sin(theta);
	helper.y = yStart + (-pRobot->getLengthMm()/2) * sin(theta) + (+pRobot->getWidthMm()+20/2) * cos(theta);
	pathArea->push_back(helper);

	helper.x = xStart + (-pRobot->getLengthMm()/2) * cos(theta) - (-pRobot->getWidthMm()+20/2) * sin(theta);
	helper.y = yStart + (-pRobot->getLengthMm()/2) * sin(theta) + (-pRobot->getWidthMm()+20/2) * cos(theta);
	pathArea->push_back(helper);

	//compute end points
	helper.x = xEnd   + (+pRobot->getLengthMm()/2) * cos(theta) - (-pRobot->getWidthMm()+20/2) * sin(theta);
	helper.y = yEnd   + (+pRobot->getLengthMm()/2) * sin(theta) + (-pRobot->getWidthMm()+20/2) * cos(theta);
	pathArea->push_back(helper);

	helper.x = xEnd   + (+pRobot->getLengthMm()/2) * cos(theta) - (+pRobot->getWidthMm()+20/2) * sin(theta);
	helper.y = yEnd   + (+pRobot->getLengthMm()/2) * sin(theta) + (+pRobot->getWidthMm()+20/2) * cos(theta);
	pathArea->push_back(helper);

	return pathArea;
}

bool AStarPathfinder::checkForAlreadyThere(PathNode *origin, PathNode *current, float xnew, float ynew)
{
	//just to make sure
	if(origin == NULL)
		return false;
	if(current == NULL)
		return false;

	//if first expansion
	if(origin == current)
		return false;

	//search all children for collision
	for(list<PathNode *>::iterator it = origin->children.begin(); it != origin->children.end();++it)
	{
		if((*it != current) && euclidDistance((*it)->x, (*it)->y, xnew, ynew) < PATH_COLLISION_LIMIT)
			return true;
	}

	//	//propagate through the tree
	for(list<PathNode *>::iterator it = origin->children.begin(); it != origin->children.end();++it)
	{
		if((*it != current) && checkForAlreadyThere((*it), current, xnew, ynew) == true )
			return true;
	}

	//if no children have collisions
	return false;
}

void AStarPathfinder::expandOnDirection(PathNode* origin, float rotation, const LocationWWeight& target, PathNode* current)
{
	float x, y;
	float theta;
	float cost;
	float heuristic;

	theta = current->theta + rotation;
	x = current->x + STEP_MOVEMENT * cos(theta);
	y = current->y + STEP_MOVEMENT * sin(theta);
	cost = current->cost + STEP_MOVEMENT * COST_MOVEMENT;
	heuristic = euclidDistance(x, y, target.x, target.y);

	tyPolygon *pathArea = generatePathArea(current->x, current->y, x, y, theta);
	if(insidePolygon2D(target, *pathArea))
	{
		pathArea = generatePathArea(current->x, current->y, x, y, theta);

		theta = atan2((target.y - current->y), (target.x - current->x));
		x = target.x;
		y = target.y;
		cost = current->cost + euclidDistance(current->x, current->y, target.x, target.y) * COST_MOVEMENT;
		heuristic = 0;
		delete pathArea;
		pathArea = generatePathArea(current->x, current->y, x, y, theta);
	}

	if(world)
	{
		if(world->computeCollisionFactor(pathArea) < COLLISION_FACTOR)
		{
			if(checkForAlreadyThere(origin, current, x, y) == false)
				current->children.push_back(new PathNode(current, x, y, theta, cost, heuristic));
		}
	}
	else
	{
		if(checkForAlreadyThere(origin, current, x, y) == false)
			current->children.push_back(new PathNode(current, x, y, theta, cost, heuristic));
	}

	delete pathArea;
}


void AStarPathfinder::expandPathNode(PathNode* origin, const LocationWWeight& target, PathNode* current)
{
	//cout << "expanding " << current << " chld: " << current->children.size() <<  endl;
	//optimize to make sure we get straight path
	float posToTargetAngle = atan2((target.y - current->y), (target.x - current->x));
	for (float rotation = 0; rotation < M_PI; rotation += STEP_ROTATION)
	{
		//positive rotation
		expandOnDirection(origin, posToTargetAngle + rotation, target, current);

		if(rotation != 0)
		{
			//positive rotation
			expandOnDirection(origin, posToTargetAngle - rotation, target, current);
		}
	}
	current->closed = true;
}


PathNode* AStarPathfinder::getMinCostLeafNode(PathNode* origin)
{
	if(origin->closed == true)
	{
		if(origin->children.empty())
			return NULL;
		//		cout << "scan children of:" << origin->x << " x " << origin->y
		//				<< " % " << origin->cost << " + " << origin->heuristic
		//				<< " = " << origin->cost  + origin->heuristic << endl;

		PathNode *leastCostlyNode = NULL;

		for(list<PathNode *>::iterator it = origin->children.begin(); it != origin->children.end(); ++it)
		{
			PathNode *testNode;

			testNode = getMinCostLeafNode(*it);

			if(testNode == NULL)
				continue;

			//			cout << "scan:" << testNode->x << " x " << testNode->y
			//				<< " % " << testNode->cost << " + " << testNode->heuristic
			//				<< " = " << testNode->cost  + testNode->heuristic << endl;

			if( !leastCostlyNode || (testNode->cost + testNode->heuristic < leastCostlyNode->cost + leastCostlyNode->heuristic))
				leastCostlyNode = testNode;
		}
		//		cout << "leastchildren::" << leastCostlyNode->x << " x " << leastCostlyNode->y
		//				<< " % " << leastCostlyNode->cost << " + " << leastCostlyNode->heuristic
		//				<< " = " << leastCostlyNode->cost  + leastCostlyNode->heuristic << endl;

		return leastCostlyNode;
	}
	else
	{
		return origin;
	}
}


bool AStarPathfinder::arrivedAtGoal(const LocationWWeight& target, PathNode* currentNode)
{
	float distance= euclidDistance(currentNode->x, currentNode->y, target.x, target.y);
	return distance <= GOAL_PRECISION;
}

bool AStarPathfinder::purgePathTree(PathNode* origin, PathNode* preserved)
{
	if(origin == preserved)
		return true;
	else
	{
		bool targetIsDownstream;
		for(list<PathNode *>::iterator it = origin->children.begin(); it != origin->children.end();)
		{
			if(purgePathTree(*it, preserved) == false)
			{
				delete *it;
				it = origin->children.erase(it);
			}
			else
			{
				targetIsDownstream = true;
				++it;
			}
		}
		return targetIsDownstream;
	}
}

PathNode* AStarPathfinder::generateRawPath(LocationWWeight target)
{
	PathNode *origin;
	{
		//set start point from physical robot state
		float x, y;
		float theta;
		pRobot->getPosition(&x, &y, &theta);
		float heuristic = euclidDistance(x, y, target.x, target.y);
		origin = new PathNode(NULL, x, y, theta, 0, heuristic);
	}

	for(int iterations = 0; iterations < MAX_ASTAR_STEPS; iterations++)
	{
		PathNode * currentNode = getMinCostLeafNode(origin);

		//all nodes closed
		if(currentNode == NULL)
			break;

		if(arrivedAtGoal(target, currentNode))
		{
			//cout << "AT GOAL !!!!!!!!!\n";
			purgePathTree(origin, currentNode);
			cout << "astar iterations: " << iterations << endl;
			return origin;
		}
		else
		{
			expandPathNode(origin, target, currentNode);
		}
	}

	//world->clearMap();

	cout << "no path found !!!!!\n";
#if 0
	purgePathTree(origin, NULL);
	delete origin;
	return NULL;
#else
	return origin;
#endif
}

bool AStarPathfinder::checkPathForMapUpdates(PathNode* path)
{
	if(!world)
		return true;
	if(!path)
		return true;

	if(path->children.size() == 0)
		return true;
	else
	{
		tyPolygon * area = generatePathArea(path->x, path->y,
				(*path->children.begin())->x, (*path->children.begin())->y, path->theta);

		if(world->computeCollisionFactor(area) < COLLISION_FACTOR*2)
		{
			delete area;
			return checkPathForMapUpdates(*path->children.begin());
		}
		else
		{
			delete area;
			return false;
		}
	}
}

void AStarPathfinder::addEachNodeToProtobuf(PathNode* originReal, robotdata::FullPath_PathDot *originProto)
{
	if(originReal == NULL)
		return;
	if(originProto == NULL)
		return;


	//search all children for collision
	for(list<PathNode *>::iterator it = originReal->children.begin(); it != originReal->children.end();++it)
	{
		robotdata::FullPath_PathDot *helper = originProto->add_nextdot();
		helper->set_x((*it)->x);
		helper->set_y((*it)->y);

		addEachNodeToProtobuf(*it, helper);
	}
}

google::protobuf::Message * AStarPathfinder::sendPath(PathNode * originReal)
{
	robotdata::FullPath * fullPath = new robotdata::FullPath;

	robotdata::FullPath_PathDot *originProto = new robotdata::FullPath_PathDot();
	originProto->set_x(originReal->x);
	originProto->set_y(originReal->y);

	addEachNodeToProtobuf(originReal, originProto);

	fullPath->set_allocated_firstdot(originProto);

	return fullPath;

}

void AStarPathfinder::deletePath(PathNode* path)
{
	purgePathTree(path, NULL);
	delete path;
}
