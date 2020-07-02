#include "stdafx.h"
#include "Tank_t2.h"


CTank_t2::CTank_t2(float sx, float sy, float ro)
	:CUnit(sx, sy
		, TANK_T2_BODY_SIZE_X, TANK_T2_BODY_SIZE_Y
		, TANK_T2_BODY_CENTER_X, TANK_T2_BODY_CENTER_Y
		, TANK_T2_HP
		, 0
		, TANK_T2_MAX_SPEED
		, TANK_T2_ACCEL
		, ro, TANK_T2_BODY_RO
		, 0, TANK_T2_GUN_RO
		, TANK_T2_GUN_SIZE_X
		, TANK_T2_GUN_SIZE_Y
		, TANK_T2_GUN_CENTER_X
		, TANK_T2_GUN_CENTER_Y)
{
}


CTank_t2::~CTank_t2()
{
}
