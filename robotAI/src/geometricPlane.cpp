/*
 * geometricPlane.cpp
 *
 *  Created on: May 27, 2013
 *      Author: alseh
 */

#include "locationWWeight.hpp"
#include "mapDiscrete.hpp"
#include "sensorManagement.h"
#include "geometricPlane.hpp"
#include <stdio.h>

Vector Vector::cross (const Vector *pV) const
{
	return Vector (y*pV->z - z*pV->y, z*pV->x - x*pV->z, x*pV->y - y*pV->x);
}

//dot product form p dot(q)
float Vector::dot(const Vector &pV1) const
{
	return x*pV1.x +y*pV1.y + z*pV1.z;
}



bool insidePolygon3D(const LocationWWeight &q, tyPolygon &pPolygon)
{
	Vector *vP,*vQ;
	Vector *v0,*vLast,*vI,*vIplus1;

	// First determine cross product vP between test point q and first and last vertex of Polygon

	v0    = new Vector(q, pPolygon.at(0) );
	vLast = new Vector(q, pPolygon.at(pPolygon.size()-1) );

	vP    = new Vector(vLast->cross(v0));

//	printf("%f %f %f\n", v0->x, v0->y, v0->z);
//	printf("%f %f %f\n", vLast->x, vLast->y, vLast->z);
//	printf("%f %f %f\n", vP->x, vP->y, vP->z);
//	printf(".........................................\n");

	//Next determine dot product of vP with vQ (cross product of v
	for (unsigned int i = 0 ; i < pPolygon.size() - 1 ; i++)
	{

		vI      = new Vector(q , pPolygon.at(i) );
		vIplus1 = new Vector(q , pPolygon.at(i+1));
		vQ      = new Vector(vI->cross(vIplus1));
//
//		printf("%f %f %f\n", vI->x, vI->y, vI->z);
//		printf("%f %f %f\n", vIplus1->x, vIplus1->y, vIplus1->z);
//		printf("%f %f %f\n", vQ->x, vQ->y, vQ->z);
//		printf(".........................................\n");
//
//		cout << "dotp : " << vP->dot(*vQ) << endl;

		// Finally check if sum < 0 if yes then outside(false), if not then inside(true)
		if( vP->dot(*vQ) < 0)
			return false;
	}

	return true;
}

bool insidePolygon2D(const LocationWWeight &q, tyPolygon &pPolygon)
{
	float vP,vQ;
	Vector *v0,*vLast,*vI,*vIplus1;

	// First determine cross product vP between test point q and first and last vertex of Polygon

	v0    = new Vector(q, pPolygon.at(0) );
	vLast = new Vector(q, pPolygon.at(pPolygon.size()-1) );
	vP    = vLast->x * v0->y - vLast->y * v0->x;


	//Next determine dot product of vP with vQ (cross product of v
	for (unsigned int i = 0 ; i < pPolygon.size() - 1 ; i++)
	{

		vI      = new Vector(q , pPolygon.at(i) );
		vIplus1 = new Vector(q , pPolygon.at(i+1));
		vQ      = vI->x * vIplus1->y - vI->y * vIplus1->x;

		// Finally check if sum < 0 if yes then outside(false), if not then inside(true)
		if( vP * vQ < 0)
			return false;
	}

	return true;
}

