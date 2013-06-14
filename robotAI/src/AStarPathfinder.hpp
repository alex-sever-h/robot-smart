/*
 * AStarPathfinder.hpp
 *
 *  Created on: Jun 8, 2013
 *      Author: alseh
 */

#ifndef ASTARPATHFINDER_HPP_
#define ASTARPATHFINDER_HPP_

#include "geometricPlane.hpp"
#include <list>
#include "robotModel.hpp"
#include "MapParticle.hpp"

using namespace std;

class RobotModel;
class MapParticle;

class PathNode {
public:
	PathNode(PathNode *p, float x, float y, float t, float c, float h){
		this->x = x;
		this->y = y;
		this->theta = t;
		this->parent = p;
		this->cost = c;
		this->heuristic = h;
		this->closed = false;
	}

	float x;
	float y;
	float theta;

	float cost;
	float heuristic;

	bool closed;

	PathNode *parent;
	list<PathNode *> children;
};

class AStarPathfinder{

	MapParticle *world;
	RobotModel  *pRobot;

private:
	tyPolygon	*generatePathArea(LocationWWeight start, LocationWWeight end, float theta);

	PathNode	*getMinCostLeafNode(PathNode *origin);
	void 		expandPathNode(PathNode* origin, const LocationWWeight& target, PathNode* current);
	bool 		arrivedAtGoal(const LocationWWeight& target, PathNode* currentNode);
	void expandOnDirection(PathNode* origin, float rotation, const LocationWWeight& target,
			PathNode* current);
	bool checkForAlreadyThere(PathNode *origin, PathNode *current, float xnew, float ynew);
	bool purgePathTree(PathNode *origin, PathNode *preserved);

public:
	AStarPathfinder(RobotModel *physicalRobot);
	virtual ~AStarPathfinder();

	tyPolygon	*generatePathArea(float xStart, float yStart, float xEnd, float yEnd, float theta);

	PathNode *generateRawPath(LocationWWeight target);
	bool     checkPathForMapUpdates(PathNode * path);
	void     deletePath(PathNode *path);

	void setWorld(MapParticle* world)
	{
		this->world = world;
	}
};


#endif /* ASTARPATHFINDER_HPP_ */
