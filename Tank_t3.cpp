#include "stdafx.h"
#include "Tank_t3.h"


CTank_t3::CTank_t3(float sx, float sy, float ro)
	:CUnit(sx, sy
		, TANK_T3_BODY_SIZE_X, TANK_T3_BODY_SIZE_Y
		, TANK_T3_BODY_CENTER_X, TANK_T3_BODY_CENTER_Y
		, TANK_T3_HP
		, 0
		, TANK_T3_MAX_SPEED
		, TANK_T3_ACCEL
		, ro, TANK_T3_BODY_RO
		, 0, TANK_T3_GUN_RO
		, TANK_T3_GUN_SIZE_X
		, TANK_T3_GUN_SIZE_Y
		, TANK_T3_GUN_CENTER_X
		, TANK_T3_GUN_CENTER_Y)
{
}


CTank_t3::~CTank_t3()
{
}
