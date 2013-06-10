/*
 * geometricPlane.hpp
 *
 *  Created on: May 27, 2013
 *      Author: alseh
 */

#ifndef GEOMETRICPLANE_HPP_
#define GEOMETRICPLANE_HPP_


#include "locationWWeight.hpp"
#include <vector>

using namespace std;

typedef vector<LocationWWeight> tyPolygon;


class Vector{
public:
	float x;
	float y;
	float z;

public:
	Vector(float x, float y, float z){
		this->x = x;
		this->y = y;
		this->z = z;
	}
	Vector(const Vector &n){
		this->x = n.x;
		this->y = n.y;
		this->z = n.z;
	}
	Vector(const LocationWWeight &p1, const LocationWWeight &p2)
	{
		this->x = p2.x - p1.x;
		this->y = p2.y - p1.y;
		this->z = p2.z - p1.z;
	}

	Vector cross (const Vector *pV) const;
	float dot(const Vector &pV1) const;

};


bool insidePolygon3D(const LocationWWeight &q, tyPolygon &pPolygon);
bool insidePolygon2D(const LocationWWeight &q, tyPolygon &pPolygon);

float euclidDistance(LocationWWeight &a, LocationWWeight &b);
float euclidDistance(float x0, float y0, float x1, float y1);

#endif /* GEOMETRICPLANE_HPP_ */
