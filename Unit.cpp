#include "stdafx.h"
#include "Unit.h"


CUnit::CUnit(
	float sx, float sy
	, float size_x, float size_y
	, float center_x, float center_y
	, int psz_hp
	, float sp
	, float maxSp
	, float ac
	, float bRo, float bRoSp
	, float gRo, float gRoSp
	, float gunSize_x, float gunSize_y
	, float gunCenter_x, float gunCenter_y)
	: sizeX(size_x), sizeY(size_y)
	, hp(psz_hp)
	, speed(sp)
	, maxSpeed(maxSp)
	, accel(ac)
	, bodyRo(bRo + 90), bodyRoSpeed(bRoSp)
	, gunRo(gRo), gunRoSpeed(gRoSp)
	, gunHeight(gunSize_y), gunWidth(gunSize_x)
	, gear(0)
	, exist(true)
{
	maxSpeed = abs(maxSpeed);
	hp = abs(hp);
	while (bRo < 0) bRo += 360;
	while (bRo >= 360) bRo -= 360;
	while (gRo < 0) gRo += 360;
	while (gRo >= 360) gRo -= 360;

	edge[0].x = -size_x / 2;
	edge[0].y = -size_y / 2;

	edge[1].x = size_x / 2;
	edge[1].y = -size_y / 2;

	edge[2].x = size_x / 2;
	edge[2].y = size_y / 2;

	edge[3].x = -size_x / 2;
	edge[3].y = size_y / 2;

	center.x = center_x;
	center.y = center_y;

	location.x = sx;
	location.y = sy;

	gunCenter.x = gunCenter_x;
	gunCenter.y = gunCenter_y;
}


CUnit::~CUnit()
{
}


point CUnit::getShootPoint()
{
	point returnValue;
	double bodyR = sqrt((center.x - sizeX / 2)*(center.x - sizeX / 2) + (center.y - sizeY / 2)*(center.y - sizeY / 2));
	returnValue.x = bodyR * cos(bodyRo*PI / 180.0);
	returnValue.y = bodyR * sin(bodyRo*PI / 180.0);
	float ro = bodyRo + gunRo;
	double gunR = sqrt((gunWidth - gunCenter.x)*(gunWidth - gunCenter.x) + (gunHeight - gunCenter.y)*(gunHeight - gunCenter.y));
	returnValue.x += gunR * cos(ro*PI / 180.0);
	returnValue.y += gunR * sin(ro*PI / 180.0);
	

	return returnValue;
}


point CUnit::nextLocation()
{
	point returnValue;
	returnValue = location;
	returnValue.x += speed * cos(bodyRo*PI / 180.0);
	returnValue.y += speed * sin(bodyRo*PI / 180.0);

	return returnValue;
}



void CUnit::moveNextLocation()
{
	location = nextLocation();
}


// 1~4, 0 이하는 1, 5이상은 4로 변경
point CUnit::getEdgePoint(int edgeNum)
{
	if (edgeNum < 1) edgeNum = 1;
	if (edgeNum > 4) edgeNum = 4;
	return edge[edgeNum - 1];
}


void CUnit::speedUp()
{
	gear++;
	if (gear > 3) gear = 3;
	renewSpeed();
}


void CUnit::speedDown()
{
	gear--;
	if (gear < -1) gear = -1;
	renewSpeed();
}


void CUnit::renewSpeed()
{
	speed = (float)gear*maxSpeed / 3;
}

void CUnit::calEdgePoint(int ro, point edgePoints[4]) {
	double r = sqrt(sizeX*sizeX + sizeY*sizeY)/2;
	edgePoints[0].x = cos((ro+150)*PI / 180)*r;
	edgePoints[0].y = sin((ro+150)*PI / 180)*r;

	edgePoints[1].x = cos( (ro+210)*PI / 180)*r;
	edgePoints[1].y = sin((ro+210)*PI / 180)*r;

	edgePoints[2].x = cos( (ro-30)*PI / 180)*r;
	edgePoints[2].y = sin( (ro-30)*PI / 180)*r;

	edgePoints[3].x = cos((ro+30)*PI / 180)*r;
	edgePoints[3].y = sin( (ro+30)*PI / 180)*r;
}

float CUnit::nextTurnLeft(point edgePoints[4])
{
	float ro = bodyRo;
	ro += bodyRoSpeed;
	if (ro >= 360) ro -= 360;

	calEdgePoint(ro, edgePoints);
	return ro;
}


float CUnit::nextTurnRight(point edgePoints[4])
{
	float ro = bodyRo;
	ro -= bodyRoSpeed;
	if (ro < 0) ro += 360;

	calEdgePoint(ro, edgePoints);
	return ro;
}


void CUnit::turnLeft()
{
	bodyRo = nextTurnLeft(edge);
}


void CUnit::turnRight()
{
	bodyRo = nextTurnRight(edge);
}


void CUnit::gunTurnLeft()
{
	gunRo += gunRoSpeed;
	if (gunRo >= 360) gunRo -= 360;
}


void CUnit::gunTurnRight()
{
	gunRo -= gunRoSpeed;
	if (gunRo < 0) gunRo += 360;
}

