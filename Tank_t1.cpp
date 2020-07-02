#include "stdafx.h"
#include "Tank_t1.h"


CTank_t1::CTank_t1(float sx, float sy, float ro)
	:CUnit(sx, sy
		,TANK_T1_BODY_SIZE_X, TANK_T1_BODY_SIZE_Y
		,TANK_T1_BODY_CENTER_X, TANK_T1_BODY_CENTER_Y
		,TANK_T1_HP
		,0
		,TANK_T1_MAX_SPEED
		,TANK_T1_ACCEL
		,ro, TANK_T1_BODY_RO
		,0, TANK_T1_GUN_RO
		, TANK_T1_GUN_SIZE_X
		, TANK_T1_GUN_SIZE_Y
		, TANK_T1_GUN_CENTER_X
		, TANK_T1_GUN_CENTER_Y)
{
}


CTank_t1::~CTank_t1()
{
}