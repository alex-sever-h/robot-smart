/*
 * mapDiscrete.cpp
 *
 *  Created on: May 26, 2013
 *      Author: alseh
 */

#include "mapDiscrete.hpp"



MapDiscrete::MapDiscrete(unsigned int mapSizeX, unsigned int mapSizeY, int mapSizeXmm, int mapSizeYmm)
{
	this->mapSizeX = mapSizeX;
	this->mapSizeY = mapSizeY;
	this->mapSizeXmm = mapSizeXmm;
	this->mapSizeYmm = mapSizeYmm;

	map = new tyMapArray(boost::extents[mapSizeX][mapSizeY]);

	for(unsigned int ix = 0; ix < mapSizeX; ++ix)
		for(unsigned int iy = 0; iy < mapSizeY; ++iy)
		{
			(*map)[ix][iy] = 1;
		}
}

float MapDiscrete::getValueXY(int xMM, int yMM)
{
	int xa, ya;

	convertRealToMap(xMM, yMM, &xa, &ya);

	if(xa < 0 || xa >= mapSizeX) return 0;
	if(ya < 0 || ya >= mapSizeY) return 0;

	return (*map)[xa][ya];
}

void MapDiscrete::setValueXY(int xMM, int yMM, float val)
{
	int xa, ya;

	convertRealToMap(xMM, yMM, &xa, &ya);

	if(xa < 0 || xa >= mapSizeX) return;
	if(ya < 0 || ya >= mapSizeY) return;

	(*map)[xa][ya] = val;
}


void MapDiscrete::convertRealToMap(int realXmm, int realYmm, int* x, int* y) {
	*x = (unsigned int)((float)mapSizeX + (float)realXmm * (float)mapSizeX / mapSizeXmm)/2;
	*y = (unsigned int)((float)mapSizeY + (float)realYmm * (float)mapSizeY / mapSizeYmm)/2;
}



void MapDiscrete::fillMapPolygon(tyPolygon * poly, float factor)
{
	if(poly == NULL)
		return;

	tyPolygon *mapPoly = new tyPolygon;
	int minX;
	int maxX;
	int minY;
	int maxY;

	convertRealToMap(poly->at(0).x, poly->at(0).y, &minX, &minY);
	convertRealToMap(poly->at(0).x, poly->at(0).y, &maxX, &maxY);

	cout < "poly: \n";
	for (tyPolygon::iterator it=poly->begin(); it!=poly->end(); ++it)
	{
		int mapX, mapY;
		convertRealToMap((int)(it->x), (int)(it->y), &mapX, &mapY);
		mapPoly->push_back(LocationWWeight(mapX, mapY, 0));

		cout << (int)(it->x) << " x " << (int)(it->y) << "  ------>  " << mapX << " x " << mapY << endl;

		if(mapX < minX) minX = mapX;
		if(mapX > maxX) maxX = mapX;

		if(mapY < minY) minY = mapY;
		if(mapY > maxY) maxY = mapY;
	}
	cout << "endpoly: \n";

	if(maxY >= (int)mapSizeY) maxY = mapSizeY-1;
	if(maxX >= (int)mapSizeX) maxX = mapSizeX-1;

	for(int iy = minY; iy < maxY; ++iy)
	{
		if(iy < 0 || iy >= (int)mapSizeY){
			iy++;
			continue;
		}
		vector<int>* collisions = getIntersections(mapPoly, minX, maxX, iy);

		if(collisions->size() == 2){
			for(int ix = collisions->at(0) ; ix < collisions->at(1); ++ix)
			{
				if(ix < 0 || ix >= (int)mapSizeX){
					ix++;
					continue;
				}
				(*map)[ix][iy] = (*map)[ix][iy] + factor;
				if((*map)[ix][iy] > 1) (*map)[ix][iy] = 1;
				if((*map)[ix][iy] < 0) (*map)[ix][iy] = 0;
			}
		}
		delete collisions;
	}

	delete mapPoly;
}

vector<int>* MapDiscrete::getIntersections(tyPolygon* poly, int xCoordMin, int xCoordMax, int yCoord)
{
	vector<int>* intersections = new vector<int>;

	int j = poly->size() - 1 ;

	for (unsigned int i=0; i < poly->size(); i++)
	{
		if ( (poly->at(i).y < yCoord && poly->at(j).y >= yCoord) || (poly->at(j).y < yCoord && poly->at(i).y >= yCoord))
		{
			int xCollision = (float)poly->at(i).x + (((float)yCoord - (float)poly->at(i).y) / (float)(poly->at(j).y - (float)poly->at(i).y)) * (float)(poly->at(j).x - (float)poly->at(i).x);
			if(xCollision >= xCoordMin && xCollision <= xCoordMax)
				intersections->push_back( xCollision );
		}
		j=i;
	}

	return intersections;
}

void MapDiscrete::updateMap(tyPolygon * safeArea, tyPolygon * wallArea)
{
	fillMapPolygon(safeArea, -0.1);
	//fillMapPolygon(wallArea, +0.3);
}

