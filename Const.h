#pragma once

#define FRAME_PER_SECOND 30
#define TIME_PER_FRAME (1000/FRAME_PER_SECOND)

#define CLIENT_X 1024
#define CLIENT_Y 768

#define MAP_WIDTH 200
#define MAP_HEIGHT 100

#define CR_X 309
#define CR_Y 309

#define PI 3.1415926

static const wchar_t fileName_tank_img[] = L"image\\tanks.png";
static const wchar_t fileName_map_img[] = L"image\\map_rect.png";

static const wchar_t fileName_crh_w_img[] = L"image\\Crosshair_white.png";
static const wchar_t fileName_crh_r_img[] = L"image\\Crosshair_red.png";
static const wchar_t fileName_crh_g_img[] = L"image\\Crosshair_green.png";

static const wchar_t fileName_cursor_img[] = L"image\\curser.png";
static const wchar_t fileName_explosion_img[] = L"image\\explosion.png";
static const wchar_t fileName_shooter_img[] = L"image\\shooter.png";
static const wchar_t fileName_shell_img[] = L"image\\shell.png";

static const wchar_t fileName_hp_g[] = L"image\\hp_g.png";
static const wchar_t fileName_hp_r[] = L"image\\hp_r.png";

static const wchar_t fileName_reloadBack[] = L"image\\reloadBack.png";
static const wchar_t fileName_reload[] = L"image\\reload.png";
static const wchar_t fileName_reloadComplete[] = L"image\\reloadComplete.png";



struct point {
	float x;
	float y;
	point operator+(point& pt) {
		point a;
		a = *this;
		a.x += pt.x;
		a.y += pt.y;
		return a;
	}
};

// 맵 타일
#define MAP_SIZE_X 512
#define MAP_SIZE_Y 384

#define MAP_TILE_SIZE_X 32
#define MAP_TILE_SIZE_Y 32

#define MAP_MAX_X (MAP_SIZE_X / MAP_TILE_SIZE_X)
#define MAP_MAX_Y (MAP_SIZE_Y / MAP_TILE_SIZE_Y)

// 폭발 타일
#define EXP_SIZE_X 256
#define EXP_SIZE_Y 256

#define EXP_TILE_SIZE_X 64
#define EXP_TILE_SIZE_Y 64

#define EXP_MAX_X (EXP_SIZE_X / EXP_TILE_SIZE_X)
#define EXP_MAX_Y (EXP_SIZE_Y / EXP_TILE_SIZE_Y)

// 슈터 타일
#define SHOOTER_SIZE_X 512
#define SHOOTER_SIZE_Y 320

#define SHOOTER_TILE_SIZE_X 64
#define SHOOTER_TILE_SIZE_Y 64

#define SHOOTER_MAX_X (SHOOTER_SIZE_X / SHOOTER_TILE_SIZE_X)
#define SHOOTER_MAX_Y (SHOOTER_SIZE_Y / SHOOTER_TILE_SIZE_Y)


#define SHELL_SIZE_X 13
#define SHELL_SIZE_Y 42

#define CURSOR_SIZE_X 60
#define CURSOR_SIZE_Y 60


// 탱크 정보
///////////
// Hotchkiss
///////////
#define TANK_T1_GUN_START_X 72
#define TANK_T1_GUN_START_Y (338-129-39)
#define TANK_T1_GUN_SIZE_X  22
#define TANK_T1_GUN_SIZE_Y  39

#define TANK_T1_BODY_START_X 66
#define TANK_T1_BODY_START_Y (338-190-62)
#define TANK_T1_BODY_SIZE_X  34
#define TANK_T1_BODY_SIZE_Y  62

static const int tank_t1_info[][4] = {
	 TANK_T1_GUN_START_X
	,TANK_T1_GUN_START_Y
	,TANK_T1_GUN_SIZE_X
	,TANK_T1_GUN_SIZE_Y
	,TANK_T1_BODY_START_X
	,TANK_T1_BODY_START_Y
	,TANK_T1_BODY_SIZE_X
	,TANK_T1_BODY_SIZE_Y };

#define TANK_T1_GUN_CENTER_X 10
#define TANK_T1_GUN_CENTER_Y 10
#define TANK_T1_BODY_CENTER_X 17
#define TANK_T1_BODY_CENTER_Y 31

///////////
// T-34/85
///////////
#define TANK_T2_GUN_START_X 158
#define TANK_T2_GUN_START_Y (338-113-59)
#define TANK_T2_GUN_SIZE_X  24
#define TANK_T2_GUN_SIZE_Y  59
			  
#define TANK_T2_BODY_START_X 154
#define TANK_T2_BODY_START_Y (338-193-64)
#define TANK_T2_BODY_SIZE_X  34
#define TANK_T2_BODY_SIZE_Y  64

static const int tank_t2_info[][4] = {
	 TANK_T2_GUN_START_X
	,TANK_T2_GUN_START_Y
	,TANK_T2_GUN_SIZE_X
	,TANK_T2_GUN_SIZE_Y
	,TANK_T2_BODY_START_X
	,TANK_T2_BODY_START_Y
	,TANK_T2_BODY_SIZE_X
	,TANK_T2_BODY_SIZE_Y };

#define TANK_T2_GUN_CENTER_X 12
#define TANK_T2_GUN_CENTER_Y 14
#define TANK_T2_BODY_CENTER_X 17
#define TANK_T2_BODY_CENTER_Y 36


///////////
// 판저 4호 전차
///////////
#define TANK_T3_GUN_START_X 252
#define TANK_T3_GUN_START_Y (338-117-61)
#define TANK_T3_GUN_SIZE_X  24
#define TANK_T3_GUN_SIZE_Y  61
			  
#define TANK_T3_BODY_START_X 249
#define TANK_T3_BODY_START_Y (338-195-58)
#define TANK_T3_BODY_SIZE_X  34
#define TANK_T3_BODY_SIZE_Y  58

static const int tank_t3_info[][4] = {
	 TANK_T3_GUN_START_X
	,TANK_T3_GUN_START_Y
	,TANK_T3_GUN_SIZE_X
	,TANK_T3_GUN_SIZE_Y
	,TANK_T3_BODY_START_X
	,TANK_T3_BODY_START_Y
	,TANK_T3_BODY_SIZE_X
	,TANK_T3_BODY_SIZE_Y };

#define TANK_T3_GUN_CENTER_X 12
#define TANK_T3_GUN_CENTER_Y 19
#define TANK_T3_BODY_CENTER_X 17
#define TANK_T3_BODY_CENTER_Y 29


///////////
// 판터
///////////
#define TANK_T4_GUN_START_X 335
#define TANK_T4_GUN_START_Y (338-109-69)
#define TANK_T4_GUN_SIZE_X  25
#define TANK_T4_GUN_SIZE_Y  69
			  
#define TANK_T4_BODY_START_X 333
#define TANK_T4_BODY_START_Y (338-195-61)
#define TANK_T4_BODY_SIZE_X  36
#define TANK_T4_BODY_SIZE_Y  61

static const int tank_t4_info[][4] = {
	 TANK_T4_GUN_START_X
	,TANK_T4_GUN_START_Y
	,TANK_T4_GUN_SIZE_X
	,TANK_T4_GUN_SIZE_Y
	,TANK_T4_BODY_START_X
	,TANK_T4_BODY_START_Y
	,TANK_T4_BODY_SIZE_X
	,TANK_T4_BODY_SIZE_Y };

#define TANK_T4_GUN_CENTER_X 12
#define TANK_T4_GUN_CENTER_Y 16
#define TANK_T4_BODY_CENTER_X 18
#define TANK_T4_BODY_CENTER_Y 30.5



///////////
// T-34/85
///////////
///////////
// T-34/85
///////////
///////////
// 판저 4호 전차
///////////
///////////
// 판터
///////////

#define TANK_T1_HP 100
#define TANK_T2_HP 120
#define TANK_T3_HP 150
#define TANK_T4_HP 200

#define TANK_T1_MAX_SPEED (38.0/FRAME_PER_SECOND)
#define TANK_T2_MAX_SPEED (36.0/FRAME_PER_SECOND)
#define TANK_T3_MAX_SPEED (30.0/FRAME_PER_SECOND)
#define TANK_T4_MAX_SPEED (28.0/FRAME_PER_SECOND)

// 현재 미사용 중인 정의
#define TANK_T1_ACCEL 1
#define TANK_T2_ACCEL 1
#define TANK_T3_ACCEL 1
#define TANK_T4_ACCEL 1


#define TANK_T1_BODY_RO (45.0/FRAME_PER_SECOND)
#define TANK_T2_BODY_RO (43.0/FRAME_PER_SECOND)
#define TANK_T3_BODY_RO (36.0/FRAME_PER_SECOND)
#define TANK_T4_BODY_RO (32.0/FRAME_PER_SECOND)

#define TANK_T1_GUN_RO (38.0/FRAME_PER_SECOND)
#define TANK_T2_GUN_RO (34.0/FRAME_PER_SECOND)
#define TANK_T3_GUN_RO (28.0/FRAME_PER_SECOND)
#define TANK_T4_GUN_RO (24.0/FRAME_PER_SECOND)


#define TANK_T1_SHELL_ATK 20
#define TANK_T2_SHELL_ATK 30
#define TANK_T3_SHELL_ATK 40
#define TANK_T4_SHELL_ATK 60

#define TANK_T1_SHELL_SPEED (800.0/FRAME_PER_SECOND)
#define TANK_T2_SHELL_SPEED (900.0/FRAME_PER_SECOND)
#define TANK_T3_SHELL_SPEED (1100.0/FRAME_PER_SECOND)
#define TANK_T4_SHELL_SPEED (1200.0/FRAME_PER_SECOND)

#define TANK_T1_RELOAD_TIME 7000
#define TANK_T2_RELOAD_TIME 9000
#define TANK_T3_RELOAD_TIME 10000
#define TANK_T4_RELOAD_TIME 13000