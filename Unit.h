#pragma once
#include<cmath>
#include "Const.h"


class CUnit
{
public:
	bool exist;
	
	point edge[4];

	point center;

	point location;

	point gunCenter;

	float sizeX;
	float sizeY;

	int hp;
	
	float speed;
	float accel;
	float maxSpeed;
	int gear;
	
	float bodyRo;
	float bodyRoSpeed;
	
	float gunRo;
	float gunRoSpeed;

	float gunHeight;
	float gunWidth;

public:
	/*
	
	*/
	CUnit(float sx, float sy
		, float size_x, float size_y
		, float center_x, float center_y
		, int psz_hp
		, float sp
		, float maxSp
		, float ac
		, float bRo, float bRoSp
		, float gRo, float gRoSp
		, float gunSize_x, float gunSize_y
		, float gunCenter_x, float gunCenter_y);
	virtual ~CUnit();
	point getShootPoint();
	point nextLocation();
	void moveNextLocation();
	// 1~4, 0 이하는 1, 5이상은 4로 변경
	point getEdgePoint(int edgeNum);
	void speedUp();
	void speedDown();
	void renewSpeed();
	void CUnit::calEdgePoint(int ro, point edgePoints[4]);
	float nextTurnLeft(point edgePoints[4]);
	float nextTurnRight(point edgePoints[4]);
	void turnLeft();
	void turnRight();
	void gunTurnLeft();
	void gunTurnRight();
};

