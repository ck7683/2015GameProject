#include "stdafx.h"
#include "Tank_t4.h"


CTank_t4::CTank_t4(float sx, float sy, float ro)
	:CUnit(sx, sy
		, TANK_T4_BODY_SIZE_X, TANK_T4_BODY_SIZE_Y
		, TANK_T4_BODY_CENTER_X, TANK_T4_BODY_CENTER_Y
		, TANK_T4_HP
		, 0
		, TANK_T4_MAX_SPEED
		, TANK_T4_ACCEL
		, ro, TANK_T4_BODY_RO
		, 0, TANK_T4_GUN_RO
		, TANK_T4_GUN_SIZE_X
		, TANK_T4_GUN_SIZE_Y
		, TANK_T4_GUN_CENTER_X
		, TANK_T4_GUN_CENTER_Y)
{
}


CTank_t4::~CTank_t4()
{
}
