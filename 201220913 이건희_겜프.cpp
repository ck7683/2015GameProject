#include "stdafx.h"
#include "201220913 이건희_겜프.h"

#define MAX_LOADSTRING 100
#define isKey(vk_key) GetAsyncKeyState(vk_key)

#include "egl/egl.h"
#include "vg/openvg.h"
#include "vgLoadImage.h"
#include "DXUTsound.h"
#include "Const.h"

#include "Unit.h"
#include "Tank_t1.h"
#include "Tank_t2.h"
#include "Tank_t3.h"
#include "Tank_t4.h"

#include "Shell.h"

#define TANK_MAX 100
#define SHELL_MAX (TANK_MAX*3)
#define HIT_MAX (SHELL_MAX*2)

#define LIMIT_SHOOT_TIME 700

#define TEAM_1_START_X 1000
#define TEAM_1_START_Y 1500

#define TEAM_2_START_X 3000
#define TEAM_2_START_Y 1500

#define GUNSOUND_VOLUME (-500)

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

												// window handle
HWND hWnd;

/////////////////
// 게임 주 데이터
RECT rect; // windowRect;
int myNum;
bool myShoot = false;
int myShootCnt;
point viewPoint;
point wall[MAP_HEIGHT*MAP_WIDTH][4];
int wallCnt = 0;
int waitTime;

int settingY;

int stage = 0;
bool stageEnd = true;

struct MapData {
	int x;
	int y;
	bool wall;
};
struct Tank {
	bool exist;
	CUnit* unit;
	int team;
	int maxReloadTime;
	int reloadTime;
	CSound* sound_fire;
};
struct Shell {
	CShell sh;
	int team;
	int shoot_time;
};
struct Hit {
	bool use;
	point lo;
	int ani;
	int maxAni;
};
int hitCount = 0;
int desCount = 0;

Hit hit[HIT_MAX];
Hit destroy[TANK_MAX];

MapData map[MAP_HEIGHT][MAP_WIDTH];
Tank tank[TANK_MAX];

Shell shell[SHELL_MAX];
int shell_cnt;

struct AI {
	bool haveTank;
	int targetIndex;
	
#define ZEALOT 1 // 얜 무작정 돌격
#define MARINE 2 // 얜 중간 사거리 유지
#define SIEGE 3 // 얜 최대 사거리 유지
	int priority;
	float plus_ro;
};
AI ai[TANK_MAX];
int t1s = 0, t1e = 0;
int t2s = 0, t2e = 0;


///////////////////////
// VG global variables
EGLDisplay display;
EGLSurface surface;
EGLContext context;

///////
// 소리
CSoundManager* csm = NULL;
CSound* sound_move;
CSound* sound_idle;
struct BoomSound {
	CSound* sound_destroy;
	bool play;
};
BoomSound sound_boom[5];
int soundNum_destroy;

#define BG_MUSIC_MAX 3
CSound* bgMusic[BG_MUSIC_MAX];
int nextMusic = 0;

///////////////
// VGImage 선언
VGImage img_tank;

VGImage img_tank_t1_gun;
VGImage img_tank_t1_body;

VGImage img_tank_t2_gun;
VGImage img_tank_t2_body;

VGImage img_tank_t3_gun;
VGImage img_tank_t3_body;

VGImage img_tank_t4_gun;
VGImage img_tank_t4_body;


VGImage img_map_tile[MAP_SIZE_Y / MAP_TILE_SIZE_Y][MAP_SIZE_X / MAP_TILE_SIZE_X];
VGImage img_map;

VGImage img_explosion_tile[EXP_SIZE_Y / EXP_TILE_SIZE_Y][EXP_SIZE_X / EXP_TILE_SIZE_X];
VGImage img_explosion;

VGImage img_shooter_tile[SHOOTER_SIZE_Y / SHOOTER_TILE_SIZE_Y][SHOOTER_SIZE_X / SHOOTER_TILE_SIZE_X];
VGImage img_shooter;

VGImage img_shell;
VGImage img_crh_w;
VGImage img_crh_r;
VGImage img_crh_g;
VGImage img_cursor;

VGImage img_hp_g;
VGImage img_hp_r;

VGImage img_reloadBack;
VGImage img_reload;
VGImage img_reloadComplete;

VGImage img_number_g[10];
VGImage img_number_r[10];
VGImage img_number[10];

VGImage img_abcd;
VGImage img_radar;
VGImage img_gTri;
VGImage img_rTri;

///////////
// 함수 선언
void setMap(int x, int y, int tile_x, int tile_y);
void timerProc();
void loadImageFunc();
void Action();
void DrawMouseCursor(int curTime);
float ccw(point p1, point p2, point p3);
float ccw(float x1, float y1, float x2, float y2, float x3, float y3);
bool isCrash(point edge[4], point start, point end);
void fire(int index, int curTime);
void makeTank(int index, int type, int team);
void DrawMap();
void stageClear(int curTime);
void stageSetting(int curTime);


void ai_player(int curTime);
void ai_zealot(int index, int curTime, float ro, double distant);
void ai_marine(int index, int curTime, float ro, double distant);
void ai_siege(int index, int curTime, float ro, double distant);

int nextTarget(int index, int tg, int myTeam);

//////////////////////////////////////////////////////////////////////////////////////////////////////////
//											구	분	선													//
//////////////////////////////////////////////////////////////////////////////////////////////////////////




// 이 코드 모듈에 들어 있는 함수의 정방향 선언입니다.
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: 여기에 코드를 입력합니다.

	// 전역 문자열을 초기화합니다.
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_MY201220913_, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// 응용 프로그램 초기화를 수행합니다.
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MY201220913_));

	FILE *fo = fopen("frame_output.txt", "w");

	MSG msg;
	DWORD lastTime = GetTickCount();
	const int frame = TIME_PER_FRAME;
	while (1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, 1))
		{
			if (!TranslateAccelerator(msg.hwnd, NULL, &msg))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			if (msg.message == WM_QUIT) break;
		}
		DWORD curTime = GetTickCount();
		if (curTime - lastTime > frame - 1)
		{
			char debugMsg[100];
			sprintf(debugMsg, "frame delay : %d\n", curTime - lastTime);
			OutputDebugStringA(debugMsg);
			fprintf(fo, "frame delay : %d\n", curTime - lastTime);
			lastTime = lastTime + frame;
			timerProc();
		}
	}
	fclose(fo);
	return (int)msg.wParam;
}



//
//  함수: MyRegisterClass()
//
//  목적: 창 클래스를 등록합니다.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDC_MY201220913_));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;// MAKEINTRESOURCEW(IDC_MY201220913_);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

//
//   함수: InitInstance(HINSTANCE, int)
//
//   목적: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   설명:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

	hWnd = CreateWindowW(szWindowClass, szTitle, WS_CAPTION | WS_SYSMENU | WS_EX_CLIENTEDGE,
		CW_USEDEFAULT, 0, CLIENT_X, CLIENT_Y, nullptr, nullptr, hInstance, nullptr);
	if (!hWnd)
	{
		return FALSE;
	}
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	srand((unsigned)time(NULL));

	return TRUE;
}


//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  목적:  주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 응용 프로그램 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// 메뉴 선택을 구문 분석합니다.
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	break;

	case WM_CREATE:
	{
		FILE* mapFile = NULL;
		// mapFile = fopen("map.txt", "r");
		if (mapFile != NULL) {
			int tile_x = 0, tile_y = 0;
			bool end = false;
			for (int i = 0; i < MAP_HEIGHT; i++) {
				for (int l = 0; l < MAP_WIDTH; l++) {
					if (EOF == fscanf(mapFile, "%2d", &tile_x)) end = true;
					if (EOF == fscanf(mapFile, "%2d", &tile_y)) end = true;
					if (end) break;
					setMap(l, i, tile_x, tile_y);
				}
				if (end) break;
			}
			fclose(mapFile);
		}
		else {
			int tile_x = 0, tile_y = 0;
			for (int i = 0; i < MAP_HEIGHT; i++) {
				for (int l = 0; l < MAP_WIDTH; l++) {
					if (rand() % 2) {
						tile_x = 10;
						tile_y = 7;
					}
					else {
						tile_x = 10;
						tile_y = 4;
					}
					setMap(l, i, tile_x, tile_y);
				}
			}
		}


		display = eglGetDisplay(GetDC(hWnd));
		eglInitialize(display, NULL, NULL);
		eglBindAPI(EGL_OPENVG_API);
		EGLint conf_list[] = { EGL_RED_SIZE, 8,
			EGL_GREEN_SIZE, 8,
			EGL_BLUE_SIZE, 8,
			EGL_ALPHA_SIZE, 8,
			EGL_NONE };
		EGLConfig config;
		EGLint num_config;
		eglChooseConfig(display, conf_list, &config, 1, &num_config);
		surface = eglCreateWindowSurface(display, config, hWnd, NULL);
		context = eglCreateContext(display, 0, NULL, NULL);

		VGfloat color[4] = { 0.22,0.71,0.93,1 };
		vgSetfv(VG_CLEAR_COLOR, 4, color);

		loadImageFunc();

		csm = new CSoundManager();
		csm->Initialize(hWnd, DSSCL_PRIORITY);
		csm->SetPrimaryBufferFormat(2, 22050, 16);


		waitTime = -1000;
		GetClientRect(hWnd, &rect);

		stageSetting(GetTickCount());


		csm->Create(&sound_move, TEXT("sound\\marder-moving2.wav"), DSBCAPS_CTRLVOLUME, GUID_NULL);
		csm->Create(&sound_idle, TEXT("sound\\marder-idle.wav"), DSBCAPS_CTRLVOLUME, GUID_NULL);
		for (int i = 0; i < 5; i++) {
			csm->Create(&sound_boom[i].sound_destroy, TEXT("sound\\destroy.wav"), DSBCAPS_CTRLVOLUME, GUID_NULL);
		}
		soundNum_destroy = 0;


		for (int i = 0; i < BG_MUSIC_MAX; i++) {
			wchar_t text[100];
			swprintf(text, L"sound\\wot%d.wav", i + 1);
			csm->Create(&bgMusic[i], text, DSBCAPS_CTRLVOLUME, GUID_NULL);
		} nextMusic = 0;

		//CSound* crashSound = NULL;
		//csm->Create(&crashSound, TEXT("crash.wav"), 0, GUID_NULL);
		//crashSound->Play(0, DSBPLAY_LOOPING); // 반복재생
		//crashSound->Play(0); // 한번 재생
		//g_pSound->IsSoundPlaying(); // 재생 확인
		//buf->SetVolume(-5000); // 볼륨
		//g_pSound->Stop(); // 일시정지


		ShowCursor(false);
	}
	break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		// TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다.
		EndPaint(hWnd, &ps);
	}
	break;
	case WM_KEYDOWN: {
		if (wParam == 'W') {
			if (tank[myNum].exist) tank[myNum].unit->speedUp();
		}
		if (wParam == 'S') {
			if (tank[myNum].exist) tank[myNum].unit->speedDown();
		}

		if (wParam == 'R') {
			if (tank[myNum].exist) tank[myNum].reloadTime = GetTickCount() - 30000;
		}

		if (wParam == 'H') {
			if (tank[myNum].exist) tank[myNum].unit->hp = TANK_T4_HP;
		}

		if (wParam == 'O') {
			stageClear(GetTickCount());
			stage-=2;
		}
		if (wParam == 'P') {
			stageClear(GetTickCount());
		}

		break;
	}
	case WM_LBUTTONDOWN: {
		fire(myNum, GetTickCount());
	}
						 break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// 정보 대화 상자의 메시지 처리기입니다.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
void setMap(int x, int y, int tile_x, int tile_y) {
	if (x < 0 || x >= MAP_WIDTH || y < 0 || y >= MAP_HEIGHT || tile_x < 0 || tile_x >= MAP_MAX_X || tile_y < 0 || tile_y >= MAP_MAX_Y) return;
	map[y][x].x = tile_x;
	map[y][x].y = tile_y;
	if (tile_y >= 3 && tile_y < 9 && tile_x < 12) map[y][x].wall = false;
	else if (tile_x >= 9 && tile_x < 12 && tile_y < 3) map[y][x].wall = false;
	else if (tile_x >= 6 && tile_x < 9 && y == 2) map[y][x].wall = false;
	else if (tile_x == 15) map[y][x].wall = false;
	else if (tile_x == 1 && tile_y == 1) map[y][x].wall = false;
	else if (tile_y == 10 && (tile_x == 4 || tile_x == 10)) map[y][x].wall = false;
	else if (tile_x == 13 && tile_y == 7) map[y][x].wall = false;
	else {
		map[y][x].wall = true;
		wall[wallCnt][0].x = x * 32;
		wall[wallCnt][0].y = y * 32;
		wall[wallCnt][1].x = x * 32 + 31;
		wall[wallCnt][1].y = y * 32;
		wall[wallCnt][2].x = x * 32 + 31;
		wall[wallCnt][2].y = y * 32 + 31;
		wall[wallCnt][3].x = x * 32;
		wall[wallCnt][3].y = y * 32 + 31;
		wallCnt++;
	}
}


void loadImageFunc(void) {
	img_tank = vgLoadImage(fileName_tank_img);
	img_map = vgLoadImage(fileName_map_img);
	img_crh_w = vgLoadImage(fileName_crh_w_img);
	img_crh_r = vgLoadImage(fileName_crh_r_img);
	img_crh_g = vgLoadImage(fileName_crh_g_img);
	img_cursor = vgLoadImage(fileName_cursor_img);
	img_explosion = vgLoadImage(fileName_explosion_img);
	img_shooter = vgLoadImage(fileName_shooter_img);
	img_shell = vgLoadImage(fileName_shell_img);
	img_hp_g = vgLoadImage(fileName_hp_g);
	img_hp_r = vgLoadImage(fileName_hp_r);
	img_reloadBack = vgLoadImage(fileName_reloadBack);
	img_reload = vgLoadImage(fileName_reload);
	img_reloadComplete = vgLoadImage(fileName_reloadComplete);
	img_abcd = vgLoadImage(L"image\\abcd.png");
	img_radar = vgLoadImage(L"image\\radar.png");
	img_gTri = vgLoadImage(L"image\\green_triangle.png");;
	img_rTri = vgLoadImage(L"image\\red_triangle.png");;

	for (int i = 0; i < 10; i++) {
		wchar_t text[100];
		swprintf(text, L"image\\%d.png", i);
		img_number[i] = vgLoadImage(text);
		swprintf(text, L"image\\%d_g.png", i);
		img_number_g[i] = vgLoadImage(text);
		swprintf(text, L"image\\%d_r.png", i);
		img_number_r[i] = vgLoadImage(text);
	}

	img_tank_t1_gun = vgCreateImage(VG_sRGBA_8888, TANK_T1_GUN_SIZE_X, TANK_T1_GUN_SIZE_Y, VG_IMAGE_QUALITY_NONANTIALIASED);
	img_tank_t1_body = vgCreateImage(VG_sRGBA_8888, TANK_T1_BODY_SIZE_X, TANK_T1_BODY_SIZE_Y, VG_IMAGE_QUALITY_NONANTIALIASED);
	vgCopyImage(img_tank_t1_gun, 0, 0, img_tank, tank_t1_info[0][0], tank_t1_info[0][1], tank_t1_info[0][2], tank_t1_info[0][3], VG_FALSE);
	vgCopyImage(img_tank_t1_body, 0, 0, img_tank, tank_t1_info[1][0], tank_t1_info[1][1], tank_t1_info[1][2], tank_t1_info[1][3], VG_FALSE);

	img_tank_t2_gun = vgCreateImage(VG_sRGBA_8888, TANK_T2_GUN_SIZE_X, TANK_T2_GUN_SIZE_Y, VG_IMAGE_QUALITY_NONANTIALIASED);
	img_tank_t2_body = vgCreateImage(VG_sRGBA_8888, TANK_T2_BODY_SIZE_X, TANK_T2_BODY_SIZE_Y, VG_IMAGE_QUALITY_NONANTIALIASED);
	vgCopyImage(img_tank_t2_gun, 0, 0, img_tank, tank_t2_info[0][0], tank_t2_info[0][1], tank_t2_info[0][2], tank_t2_info[0][3], VG_FALSE);
	vgCopyImage(img_tank_t2_body, 0, 0, img_tank, tank_t2_info[1][0], tank_t2_info[1][1], tank_t2_info[1][2], tank_t2_info[1][3], VG_FALSE);

	img_tank_t3_gun = vgCreateImage(VG_sRGBA_8888, TANK_T3_GUN_SIZE_X, TANK_T3_GUN_SIZE_Y, VG_IMAGE_QUALITY_NONANTIALIASED);
	img_tank_t3_body = vgCreateImage(VG_sRGBA_8888, TANK_T3_BODY_SIZE_X, TANK_T3_BODY_SIZE_Y, VG_IMAGE_QUALITY_NONANTIALIASED);
	vgCopyImage(img_tank_t3_gun, 0, 0, img_tank, tank_t3_info[0][0], tank_t3_info[0][1], tank_t3_info[0][2], tank_t3_info[0][3], VG_FALSE);
	vgCopyImage(img_tank_t3_body, 0, 0, img_tank, tank_t3_info[1][0], tank_t3_info[1][1], tank_t3_info[1][2], tank_t3_info[1][3], VG_FALSE);

	img_tank_t4_gun = vgCreateImage(VG_sRGBA_8888, TANK_T4_GUN_SIZE_X, TANK_T4_GUN_SIZE_Y, VG_IMAGE_QUALITY_NONANTIALIASED);
	img_tank_t4_body = vgCreateImage(VG_sRGBA_8888, TANK_T4_BODY_SIZE_X, TANK_T4_BODY_SIZE_Y, VG_IMAGE_QUALITY_NONANTIALIASED);
	vgCopyImage(img_tank_t4_gun, 0, 0, img_tank, tank_t4_info[0][0], tank_t4_info[0][1], tank_t4_info[0][2], tank_t4_info[0][3], VG_FALSE);
	vgCopyImage(img_tank_t4_body, 0, 0, img_tank, tank_t4_info[1][0], tank_t4_info[1][1], tank_t4_info[1][2], tank_t4_info[1][3], VG_FALSE);



	for (int i = 0; i < MAP_MAX_Y; i++) {
		for (int l = 0; l < MAP_MAX_X; l++) {
			img_map_tile[i][l] = vgCreateImage(VG_sRGBA_8888, MAP_TILE_SIZE_X, MAP_TILE_SIZE_Y, VG_IMAGE_QUALITY_NONANTIALIASED);
			vgCopyImage(
				img_map_tile[i][l], 0, 0
				, img_map
				, l * MAP_TILE_SIZE_X
				, i * MAP_TILE_SIZE_Y
				, MAP_TILE_SIZE_X
				, MAP_TILE_SIZE_Y
				, VG_FALSE);
		}
	}


	for (int i = 0; i < SHOOTER_MAX_Y; i++) {
		for (int l = 0; l < SHOOTER_MAX_X; l++) {
			img_shooter_tile[i][l] = vgCreateImage(VG_sRGBA_8888, SHOOTER_TILE_SIZE_X, SHOOTER_TILE_SIZE_Y, VG_IMAGE_QUALITY_NONANTIALIASED);
			vgCopyImage(
				img_shooter_tile[i][l], 0, 0
				, img_shooter
				, l * SHOOTER_TILE_SIZE_X
				, i * SHOOTER_TILE_SIZE_Y
				, SHOOTER_TILE_SIZE_X
				, SHOOTER_TILE_SIZE_Y
				, VG_FALSE);
		}
	}

	for (int i = 0; i < EXP_MAX_Y; i++) {
		for (int l = 0; l < EXP_MAX_X; l++) {
			img_explosion_tile[i][l] = vgCreateImage(VG_sRGBA_8888, EXP_TILE_SIZE_X, EXP_TILE_SIZE_Y, VG_IMAGE_QUALITY_NONANTIALIASED);
			vgCopyImage(
				img_explosion_tile[i][l], 0, 0
				, img_explosion
				, l * EXP_TILE_SIZE_X
				, i * EXP_TILE_SIZE_Y
				, EXP_TILE_SIZE_X
				, EXP_TILE_SIZE_Y
				, VG_FALSE);
		}
	}

}

float ccw(point p1, point p2, point p3) {
	return ccw(p1.x, p1.y, p2.x, p2.y, p3.x, p3.y);
}

float ccw(float x1, float y1, float x2, float y2, float x3, float y3) {
	return (x1*y2 + x2*y3 + x3*y1) - (x2*y1 + x3*y2 + x1*y3);
}

void fire(int index, int curTime) {
	if (index < 0) return;
	if (index >= TANK_MAX) return;
	if (tank[index].exist == false) return;
	if (tank[index].reloadTime + tank[index].maxReloadTime > curTime) return;

	tank[index].reloadTime = curTime + rand()%1000 - 500;
	if (index == myNum) {
		myShoot = true;
		myShootCnt = 16;
	}
	CUnit& unit = *tank[index].unit;

	point shoot = unit.getShootPoint();
	shoot.x += unit.location.x;
	shoot.y += unit.location.y;
	if (dynamic_cast<CTank_t1*>(&unit)) shell[shell_cnt].sh.set(TANK_T1_SHELL_ATK, shoot.x, shoot.y, unit.bodyRo + unit.gunRo, TANK_T1_SHELL_SPEED);
	if (dynamic_cast<CTank_t2*>(&unit)) shell[shell_cnt].sh.set(TANK_T2_SHELL_ATK, shoot.x, shoot.y, unit.bodyRo + unit.gunRo, TANK_T2_SHELL_SPEED);
	if (dynamic_cast<CTank_t3*>(&unit)) shell[shell_cnt].sh.set(TANK_T3_SHELL_ATK, shoot.x, shoot.y, unit.bodyRo + unit.gunRo, TANK_T3_SHELL_SPEED);
	if (dynamic_cast<CTank_t4*>(&unit)) shell[shell_cnt].sh.set(TANK_T4_SHELL_ATK, shoot.x, shoot.y, unit.bodyRo + unit.gunRo, TANK_T4_SHELL_SPEED);
	shell[shell_cnt].shoot_time = curTime;
	shell[shell_cnt].team = tank[index].team;

	shell_cnt++;
	if (shell_cnt >= SHELL_MAX) shell_cnt = 0;

	int soundVolume = 0;
	//double dx = tank[index].unit->location.x - viewPoint.x - CLIENT_X / 2;
	//double dy = tank[index].unit->location.y - viewPoint.y - CLIENT_Y / 2;
	double dx = (tank[myNum].unit->location.x - tank[index].unit->location.x)/24;
	double dy = (tank[myNum].unit->location.y - tank[index].unit->location.y)/24;
	double rr = (dx*dx + dy*dy);
	if(index != myNum) soundVolume = -rr + GUNSOUND_VOLUME;
	else soundVolume = 0;
	if (soundVolume < -10000) soundVolume = -9999 ;

	if (tank[index].sound_fire != NULL) {
		tank[index].sound_fire->Reset();
		tank[index].sound_fire->Play(0, 0UL, soundVolume);
	}
}


void makeTank(int index, int type, int team) {

	if (index < 0) return;
	if (index >= TANK_MAX) return;
	if (tank[index].exist) return;

	if (type < 1) type = 1;
	if (type > 4) type = 4;

	if (team < 1) team = 1;
	if (team > 2) team = 2;

	int x = rand() % 400 - 200;
	if (index == myNum) x = -250;
	int y = settingY*(rand()%2? 1 : -1);
	settingY += 20;

	tank[index].exist = true;
	tank[index].team = team;
	if (type == 1) {
		tank[index].maxReloadTime = TANK_T1_RELOAD_TIME;
		if (team == 1) tank[index].unit = new CTank_t1(TEAM_1_START_X + x, TEAM_1_START_Y + y, 270);
		if (team == 2) tank[index].unit = new CTank_t1(TEAM_2_START_X + x, TEAM_2_START_Y + y, 90);
		csm->Create(&tank[index].sound_fire, L"sound\\medium_gun_01.wav", DSBCAPS_CTRLVOLUME, GUID_NULL);
	}
	else if (type == 2) {
		tank[index].maxReloadTime = TANK_T2_RELOAD_TIME;
		if (team == 1) tank[index].unit = new CTank_t2(TEAM_1_START_X + x, TEAM_1_START_Y + y, 270);
		if (team == 2) tank[index].unit = new CTank_t2(TEAM_2_START_X + x, TEAM_2_START_Y + y, 90);
		csm->Create(&tank[index].sound_fire, L"sound\\main_gun_01.wav", DSBCAPS_CTRLVOLUME, GUID_NULL);
	}
	else if (type == 3) {
		tank[index].maxReloadTime = TANK_T3_RELOAD_TIME;
		if (team == 1) tank[index].unit = new CTank_t3(TEAM_1_START_X + x, TEAM_1_START_Y + y, 270);
		if (team == 2) tank[index].unit = new CTank_t3(TEAM_2_START_X + x, TEAM_2_START_Y + y, 90);
		csm->Create(&tank[index].sound_fire, L"sound\\large_gun_01.wav", DSBCAPS_CTRLVOLUME, GUID_NULL);
	}
	else if (type == 4) {
		tank[index].maxReloadTime = TANK_T4_RELOAD_TIME;
		if (team == 1) tank[index].unit = new CTank_t4(TEAM_1_START_X + x, TEAM_1_START_Y + y, 270);
		if (team == 2) tank[index].unit = new CTank_t4(TEAM_2_START_X + x, TEAM_2_START_Y + y, 90);
		csm->Create(&tank[index].sound_fire, L"sound\\huge_gun_01.wav", DSBCAPS_CTRLVOLUME, GUID_NULL);
	}

	tank[index].reloadTime = 0;
}

void DrawTank(int index) {
	if (!tank[index].exist) return;
	CUnit& unit = *tank[index].unit;

	vgLoadIdentity();
	vgTranslate(unit.location.x - viewPoint.x, unit.location.y - viewPoint.y);
	vgScale(0.15, 0.15);
	vgTranslate(-CR_X / 2, -CR_Y / 2);
	if (index != myNum) {
		if (tank[index].team == 1) vgDrawImage(img_crh_g);
		else if (tank[index].team == 2) vgDrawImage(img_crh_r);
	}
	else {
		vgDrawImage(img_crh_w);
	}
	vgLoadIdentity();
	vgTranslate(unit.location.x - viewPoint.x, unit.location.y - viewPoint.y);
	vgRotate(unit.bodyRo - 90);
	vgTranslate(-unit.sizeX / 2, -unit.sizeY / 2);
	if (dynamic_cast<CTank_t1*>(&unit)) vgDrawImage(img_tank_t1_body);
	if (dynamic_cast<CTank_t2*>(&unit)) vgDrawImage(img_tank_t2_body);
	if (dynamic_cast<CTank_t3*>(&unit)) vgDrawImage(img_tank_t3_body);
	if (dynamic_cast<CTank_t4*>(&unit)) vgDrawImage(img_tank_t4_body);
	vgTranslate(unit.sizeX / 2, unit.sizeY / 2);
	vgTranslate(unit.center.x - unit.sizeX / 2, unit.center.y - unit.sizeY / 2);
	vgRotate(unit.gunRo);
	vgTranslate(-unit.gunCenter.x, -unit.gunCenter.y);
	if (dynamic_cast<CTank_t1*>(&unit)) vgDrawImage(img_tank_t1_gun);
	if (dynamic_cast<CTank_t2*>(&unit)) vgDrawImage(img_tank_t2_gun);
	if (dynamic_cast<CTank_t3*>(&unit)) vgDrawImage(img_tank_t3_gun);
	if (dynamic_cast<CTank_t4*>(&unit)) vgDrawImage(img_tank_t4_gun);

	int full_hpbar;
	int hpbar;
	if (dynamic_cast<CTank_t1*>(&unit)) full_hpbar = TANK_T1_HP / 10;
	if (dynamic_cast<CTank_t2*>(&unit)) full_hpbar = TANK_T2_HP / 10;
	if (dynamic_cast<CTank_t3*>(&unit)) full_hpbar = TANK_T3_HP / 10;
	if (dynamic_cast<CTank_t4*>(&unit)) full_hpbar = TANK_T4_HP / 10;
	hpbar = unit.hp / 10;
	if (hpbar == 0) hpbar = 1;
	for (int i = 0; i < full_hpbar; i++) {
		vgLoadIdentity();
		vgTranslate(unit.location.x - viewPoint.x, unit.location.y - viewPoint.y);
		vgTranslate(-(full_hpbar * 5) / 2 + i * 5, -unit.sizeY / 2 - 10);
		if (i < hpbar) {
			vgDrawImage(img_hp_g);
		}
		else {
			vgDrawImage(img_hp_r);
		}
	}
}

void Action() {



	if (tank[myNum].exist && !stageEnd) {
		if (sound_idle != NULL && tank[myNum].unit->gear == 0) {
			long llll = -1;
			if (sound_move != NULL && (sound_move->IsSoundPlaying())) {
				sound_move->Stop();
				sound_move->Reset();
			}
			sound_idle->Play(0);
		}
		else if (sound_move != NULL) {
			if (sound_idle != NULL && (sound_idle->IsSoundPlaying())) {
				sound_idle->Stop();
				sound_idle->Reset();
			}
			sound_move->Play(0, 0UL, -(3 - tank[myNum].unit->gear) * 200);
		}
	}
	else {
		if (sound_move != NULL) sound_move->Stop();
		if (sound_idle != NULL) sound_idle->Stop();
	}

	point edge[4];
	if (isKey('A') && tank[myNum].exist) {
		bool move = true;
		//tank[myNum].unit->nextTurnLeft(edge);

		//for (int l = 0; l < wallCnt; l++) {
		//	if (isCrash(wall[l], tank[myNum].unit->location + tank[myNum].unit->edge[0], tank[myNum].unit->location + edge[0])) move = false;
		//	if (isCrash(wall[l], tank[myNum].unit->location + tank[myNum].unit->edge[1], tank[myNum].unit->location + edge[1])) move = false;
		//	if (isCrash(wall[l], tank[myNum].unit->location + tank[myNum].unit->edge[2], tank[myNum].unit->location + edge[2])) move = false;
		//	if (isCrash(wall[l], tank[myNum].unit->location + tank[myNum].unit->edge[3], tank[myNum].unit->location + edge[3])) move = false;
		//}

		if (move) tank[myNum].unit->turnLeft();
	}

	if (isKey('D') && tank[myNum].exist) {
		bool move = true;
		//tank[myNum].unit->nextTurnRight(edge);

		//for (int l = 0; l < wallCnt; l++) {
		//	if (isCrash(wall[l], tank[myNum].unit->location + tank[myNum].unit->edge[0], tank[myNum].unit->location + edge[0])) move = false;
		//	if (isCrash(wall[l], tank[myNum].unit->location + tank[myNum].unit->edge[1], tank[myNum].unit->location + edge[1])) move = false;
		//	if (isCrash(wall[l], tank[myNum].unit->location + tank[myNum].unit->edge[2], tank[myNum].unit->location + edge[2])) move = false;
		//	if (isCrash(wall[l], tank[myNum].unit->location + tank[myNum].unit->edge[3], tank[myNum].unit->location + edge[3])) move = false;
		//}
		if (move) tank[myNum].unit->turnRight();
	}


	POINT pt = { 0 };
	GetCursorPos(&pt);
	ScreenToClient(hWnd, &pt);
	pt.y = rect.bottom - pt.y;

	pt.x += viewPoint.x;
	pt.y += viewPoint.y;
	float dx = pt.x - tank[myNum].unit->location.x;
	float dy = pt.y - tank[myNum].unit->location.y;

	double d = (double)dx*dx + (double)dy*dy;
	if (d > 100) {
		float dro = 90;
		if (abs(dx) > 0.5) dro = atan(dy / dx) * 180 / PI;

		if (dro < 0) dro += 180;
		if (dy < 0) dro += 180;

		float ro = tank[myNum].unit->bodyRo + tank[myNum].unit->gunRo;
		if (ro >= 360) ro -= 360;


		float ddro = dro - ro;
		if (ddro < 0) ddro += 360;


		if (ddro > 1) {
			if (ddro > 180) {
				tank[myNum].unit->gunTurnRight();
			}
			else tank[myNum].unit->gunTurnLeft();
		}
	}
}

void DrawMouseCursor(int curTime) {
	int const viewSpeed = 10;
	POINT pt = { 0 };
	GetCursorPos(&pt);
	ScreenToClient(hWnd, &pt);
	if (pt.x < 0) pt.x = 0;
	if (pt.y < 0) pt.y = 0;
	if (pt.x > rect.right) pt.x = rect.right;
	if (pt.y > rect.bottom) pt.y = rect.bottom;
	pt.y = rect.bottom - pt.y;


	if (tank[myNum].exist) {
		point view;
		view.x = (pt.x - CLIENT_X / 2) * 2 / 3 + tank[myNum].unit->location.x - CLIENT_X / 2;
		view.y = (pt.y - CLIENT_Y / 2) * 2 / 3 + tank[myNum].unit->location.y - CLIENT_Y / 2;
		if (myShoot) {
			if (myShootCnt != 0) {
				view.x += rand() % myShootCnt - myShootCnt / 2;
				view.y += rand() % myShootCnt - myShootCnt / 2;
			}
			myShootCnt--;
			if (myShootCnt < 1) myShoot = false;
		}

		if (viewPoint.x > view.x + viewSpeed) viewPoint.x = viewPoint.x -= viewSpeed;
		if (viewPoint.y > view.y + viewSpeed) viewPoint.y = viewPoint.y -= viewSpeed;
		if (viewPoint.x < view.x - viewSpeed) viewPoint.x = viewPoint.x += viewSpeed;
		if (viewPoint.y < view.y - viewSpeed) viewPoint.y = viewPoint.y += viewSpeed;
		if (abs(viewPoint.x - view.x) <= viewSpeed) viewPoint.x = view.x;
		if (abs(viewPoint.y - view.y) <= viewSpeed) viewPoint.y = view.y;
		if (viewPoint.x < 0) viewPoint.x = 0;
		if (viewPoint.y < 0) viewPoint.y = 0;

		if (viewPoint.x > (MAP_WIDTH - 1) * 32 - CLIENT_X) viewPoint.x = (MAP_WIDTH - 1) * 32 - CLIENT_X;
		if (viewPoint.y > (MAP_HEIGHT - 1) * 32 - CLIENT_Y) viewPoint.y = (MAP_HEIGHT - 1) * 32 - CLIENT_Y;
	}


	vgLoadIdentity();
	vgTranslate(pt.x, pt.y);
	vgTranslate(-CURSOR_SIZE_X / 2, -CURSOR_SIZE_Y / 2);
	vgDrawImage(img_cursor);

	if (tank[myNum].exist) {
		int cc = curTime - tank[myNum].reloadTime;
		int reloadPer = 50 * (cc > tank[myNum].maxReloadTime ? tank[myNum].maxReloadTime : cc ) / tank[myNum].maxReloadTime;
		if (reloadPer < 50) {
			vgLoadIdentity();
			vgTranslate(pt.x, pt.y);
			vgTranslate(CURSOR_SIZE_X / 2 + 10, -CURSOR_SIZE_Y / 2 + 5);
			vgDrawImage(img_reloadBack);

			for (int i = 0; i < reloadPer; i++) {
				vgLoadIdentity();
				vgTranslate(pt.x, pt.y);
				vgTranslate(CURSOR_SIZE_X / 2 + 10, -CURSOR_SIZE_Y / 2 + 5);
				vgTranslate(1, 1 + i);
				vgDrawImage(img_reload);
			}
		}
		else {
			vgLoadIdentity();
			vgTranslate(pt.x, pt.y);
			vgTranslate(CURSOR_SIZE_X / 2 + 10, -CURSOR_SIZE_Y / 2 + 5);
			vgDrawImage(img_reloadComplete);
		}
	}
}
bool isCrash(point edge[4], point start, point end) {
	int minus, plus;
	minus = plus = 0;
	for (int i = 0; i < 4; i++) {
		float returnValue = ccw(start, end, edge[i]);
		if (returnValue < 0) minus++;
		else if (returnValue > 0) plus++;
	}
	if (minus > 0 && plus > 0) {
		float r1 = ccw(edge[0], edge[2], start);
		float r2 = ccw(edge[0], edge[2], end);
		if ((double)r1*r2 < 0) return true;
		else {
			r1 = ccw(edge[1], edge[3], start);
			r2 = ccw(edge[1], edge[3], end);
			if ((double)r1*r2 < 0) return true;
			else return false;
		}
	}
	else return false;
}

void DrawMap() {
	int x, y;
	x = (viewPoint.x) / MAP_TILE_SIZE_X;
	y = (viewPoint.y) / MAP_TILE_SIZE_Y;
	if (x < 0) x = 0;
	if (y < 0) y = 0;
	int ex, ey;
	ex = x + CLIENT_X / MAP_TILE_SIZE_X + 1;
	ey = y + CLIENT_Y / MAP_TILE_SIZE_Y + 1;
	if (ex >= MAP_WIDTH) ex = MAP_WIDTH - 1;
	if (ey >= MAP_HEIGHT) ey = MAP_HEIGHT - 1;

	for (int i = y; i < ey; i++) {
		for (int l = x; l < ex; l++) {
			vgLoadIdentity();
			vgTranslate(-viewPoint.x, -viewPoint.y);
			vgTranslate(l * 32, i * 32);
			vgDrawImage(img_map_tile[map[i][l].y][map[i][l].x]);
		}
	}
}

void Draw() {
	int enemy = 0;
	int alli = 0;

	DrawMap();

	int curTime = GetTickCount();
	Action();
	for (int i = 0; i < TANK_MAX; i++) {
		if (tank[i].exist) {
			if (tank[i].team == 2) enemy++;
			else if (tank[i].team == 1) alli++;
			bool move = true;
			//point edge[4];
			//edge[0] = tank[i].unit->edge[0];
			//edge[1] = tank[i].unit->edge[1];
			//edge[2] = tank[i].unit->edge[2];
			//edge[3] = tank[i].unit->edge[3];

			//point lo1 = tank[i].unit->location;
			//point lo2 = tank[i].unit->nextLocation();
			//for (int l = 0; l < wallCnt; l++) {
			//	if (isCrash(wall[l], tank[i].unit->location + tank[i].unit->edge[0], tank[i].unit->nextLocation() + tank[i].unit->edge[0])) move = false;
			//	if (isCrash(wall[l], tank[i].unit->location + tank[i].unit->edge[1], tank[i].unit->nextLocation() + tank[i].unit->edge[1])) move = false;
			//	if (isCrash(wall[l], tank[i].unit->location + tank[i].unit->edge[2], tank[i].unit->nextLocation() + tank[i].unit->edge[2])) move = false;
			//	if (isCrash(wall[l], tank[i].unit->location + tank[i].unit->edge[3], tank[i].unit->nextLocation() + tank[i].unit->edge[3])) move = false;
			//}
			if (move) {
				point pt = tank[i].unit->nextLocation();
				if (pt.x < 64) tank[i].unit->location.x = 64;
				if (pt.y < 64) tank[i].unit->location.y = 64;
				if (pt.x > (MAP_WIDTH - 3) * 32) tank[i].unit->location.x = (MAP_WIDTH - 3) * 32;
				if (pt.y > (MAP_HEIGHT - 4) * 32) tank[i].unit->location.y = (MAP_HEIGHT - 4) * 32;
				tank[i].unit->moveNextLocation();
			}
			DrawTank(i);
		}
	}

	for (int i = 0; i < SHELL_MAX; i++) {
		if (shell[i].sh.exist) {
			if (curTime - shell[i].shoot_time > LIMIT_SHOOT_TIME) {
				shell[i].sh.exist = false;
			}

			point start;
			start.x = shell[i].sh.x;
			start.y = shell[i].sh.y;
			point end = shell[i].sh.nextLocation();
			bool crash = false;
			for (int l = 0; l < TANK_MAX; l++) {
				if (tank[l].exist && tank[l].team != shell[i].team) {
					point edge[4];
					edge[0] = tank[l].unit->edge[0];
					edge[0].x += +tank[l].unit->location.x;
					edge[0].y += +tank[l].unit->location.y;
					edge[1] = tank[l].unit->edge[1];
					edge[1].x += +tank[l].unit->location.x;
					edge[1].y += +tank[l].unit->location.y;
					edge[2] = tank[l].unit->edge[2];
					edge[2].x += +tank[l].unit->location.x;
					edge[2].y += +tank[l].unit->location.y;
					edge[3] = tank[l].unit->edge[3];
					edge[3].x += +tank[l].unit->location.x;
					edge[3].y += +tank[l].unit->location.y;
					if (isCrash(edge, start, end)) {
						shell[i].sh.exist = false;
						crash = true;
						hit[hitCount].maxAni = 40;
						hit[hitCount].ani = 0;
						hit[hitCount].lo = tank[l].unit->location;
						hit[hitCount].use = true;
						hitCount++;
						if (hitCount >= HIT_MAX) hitCount = 0;

						tank[l].unit->hp -= shell[i].sh.atk;
						if (tank[l].unit->hp <= 0) {
							tank[l].exist = false;
							destroy[desCount].maxAni = 16;
							destroy[desCount].ani = 0;
							destroy[desCount].lo = tank[l].unit->location;
							destroy[desCount].use = true;
							desCount++;
							if (desCount >= HIT_MAX) desCount = 0;

							int soundVolume = 0;
							double dx = tank[l].unit->location.x - viewPoint.x - CLIENT_X / 2;
							double dy = tank[l].unit->location.y - viewPoint.y - CLIENT_Y / 2;
							double rr = (dx*dx + dy*dy) / (16 * 16);
							soundVolume = -rr;
							if (soundVolume < -10000) soundVolume = -10000;

							if (sound_boom[soundNum_destroy].sound_destroy != NULL) {
								sound_boom[soundNum_destroy].sound_destroy->Reset();
								sound_boom[soundNum_destroy].sound_destroy->Play(0, 0UL, soundVolume);
							}
							sound_boom[soundNum_destroy].play = false;
							soundNum_destroy++;
							if (soundNum_destroy >= 5) soundNum_destroy = 0;

						} // if (tank[l].unit->hp <= 0)
						break;
					} // if (isCrash(edge, start, end))
				} // if (tank[l].exist && tank[l].team != shell[i].team)
			} // for (int l = 0; l < TANK_MAX; l++)
			if (!crash) {
				vgLoadIdentity();
				shell[i].sh.move();
				vgTranslate(shell[i].sh.x - viewPoint.x, shell[i].sh.y - viewPoint.y);
				vgRotate(shell[i].sh.ro - 90);
				vgScale(0.5, 0.5);
				vgTranslate(-SHELL_SIZE_X / 2, -SHELL_SIZE_Y / 2);
				vgDrawImage(img_shell);
				vgLoadIdentity();
			}
		}
	}

	for (int i = 0; i < HIT_MAX; i++) {
		if (hit[i].use) {
			vgLoadIdentity();
			vgTranslate(hit[i].lo.x - viewPoint.x, hit[i].lo.y - viewPoint.y);
			vgTranslate(-SHOOTER_TILE_SIZE_X / 2, -SHOOTER_TILE_SIZE_Y / 2);
			vgDrawImage(img_shooter_tile[4 - hit[i].ani / 8][hit[i].ani % 8]);
			hit[i].ani += 2;
			if (hit[i].ani >= hit[i].maxAni) hit[i].use = false;
		}
	}
	for (int i = 0; i < TANK_MAX; i++) {
		if (destroy[i].use) {
			vgLoadIdentity();
			vgTranslate(destroy[i].lo.x - viewPoint.x, destroy[i].lo.y - viewPoint.y);
			vgScale(1.5, 1.5);
			vgTranslate(-EXP_TILE_SIZE_X / 2, -EXP_TILE_SIZE_Y / 2);
			vgDrawImage(img_explosion_tile[3 - destroy[i].ani / 4][destroy[i].ani % 4]);
			destroy[i].ani += 1;
			if (destroy[i].ani >= destroy[i].maxAni) destroy[i].use = false;
		}
	}

	DrawMouseCursor(curTime);

	int n1;
	int n2;
	int n3;
	n1 = n2 = n3 = enemy;
	n1 /= 100;
	n2 /= 10;
	n3 %= 10;
	n2 %= 10;
	n1 %= 10;

	vgLoadIdentity();
	vgTranslate(-16 + CLIENT_X * 6 / 7, -32 + CLIENT_Y - 80);
	vgDrawImage(img_number_r[n1]);
	vgTranslate(32, 0);
	vgDrawImage(img_number_r[n2]);
	vgTranslate(32, 0);
	vgDrawImage(img_number_r[n3]);

	n1 = n2 = n3 = alli;
	n1 /= 100;
	n2 /= 10;
	n3 %= 10;
	n2 %= 10;
	n1 %= 10;
	vgLoadIdentity();
	vgTranslate(-16 + CLIENT_X * 1 / 20, -32 + CLIENT_Y - 80);
	vgDrawImage(img_number_g[n1]);
	vgTranslate(32, 0);
	vgDrawImage(img_number_g[n2]);
	vgTranslate(32, 0);
	vgDrawImage(img_number_g[n3]);

	n1 = n2 = n3 = stage;
	n1 /= 100;
	n2 /= 10;
	n3 %= 10;
	n2 %= 10;
	n1 %= 10;
	vgLoadIdentity();
	vgTranslate(-16 + CLIENT_X * 1 / 2, -32 + CLIENT_Y - 80);
	if(n1 != 0) vgDrawImage(img_number[n1]);
	vgTranslate(32, 0);
	if(n1 != 0 || n2 != 0) vgDrawImage(img_number[n2]);
	vgTranslate(32, 0);
	vgDrawImage(img_number[n3]);

	vgLoadIdentity();
	vgTranslate(400, 0);
	vgDrawImage(img_abcd);


	vgLoadIdentity();
	vgTranslate(711 / 8, 720 / 8);
	vgScale(0.25, 0.25);
	vgTranslate(-711 / 2, -720 / 2);
	vgDrawImage(img_radar);

	// 600 520
	for (int i = 1; i <= t2e; i++) {
		if (tank[i].exist) {
			vgLoadIdentity();
			float dx = 0;
			float dy = 0;
			double distant = 0;
			if (tank[myNum].exist) {
				dx = tank[i].unit->location.x - tank[myNum].unit->location.x;
				dy = tank[i].unit->location.y - tank[myNum].unit->location.y;
				distant = sqrt(dx*dx + dy*dy);
			}
			dx /= 2000;
			dy /= 2000;
			double t = distant / 2000;
			if (distant > 2000) {
				dx /= t;
				dy /= t;
			}
			vgTranslate(70*dx,70*dy );
			vgTranslate(711 / 8, 720 / 8);
			vgScale(0.02, 0.02);
			vgTranslate(-600 / 2, -520 / 2);
			if (tank[i].team == 1) vgDrawImage(img_gTri);
			if (tank[i].team == 2) vgDrawImage(img_rTri);
		}
	}

	if (tank[myNum].exist == false && !stageEnd) alli = 0;
	if ((alli == 0 || enemy == 0) && !stageEnd) {
		if (alli == 0) stage--;
		stageEnd = true;
		stageClear(curTime);
		waitTime = curTime + 3000;
	}
	if (stageEnd) stageSetting(curTime);
}


void timerProc(void) {
	vgClear(0, 0, rect.right + 100, rect.bottom + 100);
	eglMakeCurrent(display, surface, surface, context);
	vgLoadIdentity();
	vgSeti(VG_MATRIX_MODE, VG_MATRIX_IMAGE_USER_TO_SURFACE);

	ai_player(GetTickCount());
	Draw();
	int beforeMusic = nextMusic - 1;
	if (beforeMusic < 0) beforeMusic = BG_MUSIC_MAX - 1;
	if (bgMusic[beforeMusic] != NULL && !bgMusic[beforeMusic]->IsSoundPlaying()) {
		if(bgMusic[nextMusic]!= NULL) bgMusic[nextMusic]->Play(0);
		nextMusic++;
		if (nextMusic >= BG_MUSIC_MAX) nextMusic = 0;
	}

	eglSwapBuffers(display, surface);
}

void stageClear(int curTime) {
	stageEnd = true;
	settingY = 0;
	for (int i = 0; i < TANK_MAX; i++) {
		tank[i].exist = false;
	}
	for (int i = 0; i < SHELL_MAX; i++) {
		shell[i].sh.exist = false;
	}
	for (int i = 0; i < HIT_MAX; i++) {
		hit[i].use = false;
	}
	hitCount = desCount = shell_cnt = 0;

}

void stageSetting(int curTime) {
	if (stageEnd && waitTime < curTime) {
		stageEnd = false;
	}
	else return;

	stageClear(curTime);

	myNum = 0;
	makeTank(myNum, 4, 1);
	if (stage < 0) stage = 0;
	if (stage > 10) stage = 10;
	if (stage < 11) {
		int alli = stage * 4;
		int enemy = stage * 5 + stage*stage/10;
		if (stage == 0) {
			alli = 1;
			enemy = 1;
		}

		int n = alli + enemy;
		if (n > TANK_MAX) n = TANK_MAX;

		t1s = 0, t1e = alli - 1;
		t2s = alli, t2e = n - 1;

		for (int i = 1; i < alli; i++) {
			makeTank(i, (rand() % 5) / 4 + 3, 1);
			if (tank[i].exist) {
				ai[i].haveTank = true;
				ai[i].priority = rand() % 3 + 1;
				ai[i].targetIndex = (rand() % (n - alli)) + alli;
				ai[i].plus_ro = (rand() % 2 == 1 ? -1 : 1);
			}
		}
		settingY = 0;
		for (int i = alli; i < n; i++) {
			makeTank(i, (rand() % 3) / 2 + 1, 2);
			if (tank[i].exist) {
				ai[i].haveTank = true;
				ai[i].priority = rand() % 4;
				if (ai[i].priority == 0) ai[i].priority = 1;
				ai[i].targetIndex = rand() % alli;
				ai[i].plus_ro = (rand() % 2 == 1 ? -1 : 1);
			}
		}
		for (int i = 0; i < n; i++) {
			ai[i].targetIndex = nextTarget(i, ai[i].targetIndex, tank[i].team);
		}
	}

	viewPoint.x = tank[myNum].unit->location.x - CLIENT_X / 2;
	viewPoint.y = tank[myNum].unit->location.y - CLIENT_Y / 2;

	stageEnd = false;
	stage++;
}
void ai_player(int curTime) {
	for (int i = 1; i <= t2e; i++) {
		if (tank[i].exist) {
			if (tank[i].unit->gear < 3) tank[i].unit->speedUp();
			int tg = ai[i].targetIndex;
			if (!tank[tg].exist) tg = nextTarget(i, tg, tank[i].team);
			if (tank[tg].exist == false) continue;

			float dx = tank[tg].unit->location.x - tank[i].unit->location.x;
			float dy = tank[tg].unit->location.y - tank[i].unit->location.y;

			double dd = (double)dx*dx + (double)dy*dy;
			float dro = 90;
			if (dd > 100) {
				if (abs(dx) > 0.5) dro = atan(dy / dx) * 180 / PI;

				if (dro < 0) dro += 180;
				if (dy < 0) dro += 180;
				dro -= tank[i].unit->bodyRo;
				if (dro < 0) dro += 360;
				if (dro >= 360) dro -= 360;
			}

			double distant = sqrt(dd);
			if (ai[i].priority == ZEALOT) {
				ai_zealot(i, curTime, dro, distant);

			}
			else if (ai[i].priority == MARINE) {
				ai_marine(i, curTime, dro, distant);

			}
			else if (ai[i].priority == SIEGE) {
				ai_siege(i, curTime, dro, distant);

			}

			if (dynamic_cast<CTank_t1*>(tank[i].unit)) if (distant < TANK_T1_SHELL_SPEED*FRAME_PER_SECOND*LIMIT_SHOOT_TIME / 1000) fire(i, curTime);
			if (dynamic_cast<CTank_t2*>(tank[i].unit)) if (distant < TANK_T2_SHELL_SPEED*FRAME_PER_SECOND*LIMIT_SHOOT_TIME / 1000) fire(i, curTime);
			if (dynamic_cast<CTank_t3*>(tank[i].unit)) if (distant < TANK_T3_SHELL_SPEED*FRAME_PER_SECOND*LIMIT_SHOOT_TIME / 1000) fire(i, curTime);
			if (dynamic_cast<CTank_t4*>(tank[i].unit)) if (distant < TANK_T4_SHELL_SPEED*FRAME_PER_SECOND*LIMIT_SHOOT_TIME / 1000) fire(i, curTime);
		}
	}
}

void ai_zealot(int index, int curTime, float ro, double distant) {
	if (abs(ro) > 2) {
		if (ro > 180) tank[index].unit->turnRight();
		else tank[index].unit->turnLeft();
	}

	float gRo = tank[index].unit->gunRo - ro;
	if (gRo < 0) gRo += 360;
	if (gRo > 2) {
		if (gRo > 180) tank[index].unit->gunTurnLeft();
		else tank[index].unit->gunTurnRight();
	}


}
void ai_marine(int index, int curTime, float ro, double distant) {
	int f_dis;
	if (dynamic_cast<CTank_t1*>(tank[index].unit)) f_dis = TANK_T1_SHELL_SPEED*FRAME_PER_SECOND*LIMIT_SHOOT_TIME / 1000;
	else if (dynamic_cast<CTank_t2*>(tank[index].unit)) f_dis = TANK_T2_SHELL_SPEED*FRAME_PER_SECOND*LIMIT_SHOOT_TIME / 1000;
	else if (dynamic_cast<CTank_t3*>(tank[index].unit)) f_dis = TANK_T3_SHELL_SPEED*FRAME_PER_SECOND*LIMIT_SHOOT_TIME / 1000;
	else if (dynamic_cast<CTank_t4*>(tank[index].unit)) f_dis = TANK_T4_SHELL_SPEED*FRAME_PER_SECOND*LIMIT_SHOOT_TIME / 1000;
	
	if (f_dis * 2 / 3 < distant) {
		if (abs(ro) > 2) {
			if (ro > 180) tank[index].unit->turnRight();
			else tank[index].unit->turnLeft();
		}
	}
	else {
		float tmpRo = ro + ai[index].plus_ro;
		if (tmpRo < 0) tmpRo += 360;

		if (tmpRo < 180) {
			if (abs(tmpRo - 90) > 2) {
				if (tmpRo < 90) tank[index].unit->turnRight();
				else tank[index].unit->turnLeft();
			}
		}
		else {
			if (abs(tmpRo - 270) > 2) {
				if (tmpRo > 270) tank[index].unit->turnLeft();
				else tank[index].unit->turnRight();
			}
		}
	}
	float gRo = tank[index].unit->gunRo - ro;
	if (gRo < 0) gRo += 360;
	if (gRo > 2) {
		if (gRo > 180) tank[index].unit->gunTurnLeft();
		else tank[index].unit->gunTurnRight();
	}
}
void ai_siege(int index, int curTime, float ro, double distant) {
	int f_dis;
	if (dynamic_cast<CTank_t1*>(tank[index].unit)) f_dis = TANK_T1_SHELL_SPEED*FRAME_PER_SECOND*LIMIT_SHOOT_TIME / 1000;
	else if (dynamic_cast<CTank_t2*>(tank[index].unit)) f_dis = TANK_T2_SHELL_SPEED*FRAME_PER_SECOND*LIMIT_SHOOT_TIME / 1000;
	else if (dynamic_cast<CTank_t3*>(tank[index].unit)) f_dis = TANK_T3_SHELL_SPEED*FRAME_PER_SECOND*LIMIT_SHOOT_TIME / 1000;
	else if (dynamic_cast<CTank_t4*>(tank[index].unit)) f_dis = TANK_T4_SHELL_SPEED*FRAME_PER_SECOND*LIMIT_SHOOT_TIME / 1000;

	if (f_dis * 5 / 6 < distant) {
		if (abs(ro) > 2) {
			if (ro > 180) tank[index].unit->turnRight();
			else tank[index].unit->turnLeft();
		}
	}
	else {
		float tmpRo = ro + ai[index].plus_ro;
		if (tmpRo < 0) tmpRo += 360;

		if (tmpRo < 180) {
			if (abs(tmpRo - 90) > 2) {
				if (tmpRo < 90) tank[index].unit->turnRight();
				else tank[index].unit->turnLeft();
			}
		}
		else {
			if (abs(tmpRo - 270) > 2) {
				if (tmpRo > 270) tank[index].unit->turnLeft();
				else tank[index].unit->turnRight();
			}
		}
	}
	float gRo = tank[index].unit->gunRo - ro;
	if (gRo < 0) gRo += 360;
	if (gRo > 2) {
		if (gRo > 180) tank[index].unit->gunTurnLeft();
		else tank[index].unit->gunTurnRight();
	}
	ai[index].targetIndex = nextTarget(index, ai[index].targetIndex, tank[index].team);

	//if (tank[index].team == 1) {
	//	double st = 0;
	//	int sti = -1;
	//	for (int i = t2s; i <= t2e; i++) {
	//		if (tank[i].exist) {
	//			int tmpSt = abs(tank[i].unit->location.x - tank[index].unit->location.x) + abs(tank[i].unit->location.y - tank[index].unit->location.y);
	//			if (sti == -1 || st > tmpSt) {
	//				st = tmpSt;
	//				sti = i;
	//			}
	//		}
	//	}
	//	if (sti != -1) {
	//		ai[index].targetIndex = sti;
	//	}
	//}
	//if (tank[index].team == 2) {
	//	double st = 0;
	//	int sti = -1;
	//	for (int i = t1s; i <= t1e; i++) {
	//		if (tank[i].exist) {
	//			int tmpSt = abs(tank[i].unit->location.x - tank[index].unit->location.x) + abs(tank[i].unit->location.y - tank[index].unit->location.y);
	//			if (sti == -1 || st > tmpSt) {
	//				st = tmpSt;
	//				sti = i;
	//			}
	//		}
	//	}
	//	if (sti != -1) {
	//		ai[index].targetIndex = sti;
	//	}
	//}
}

int nextTarget(int index, int tg, int myTeam) {
	int nextTg = -1;

	if (tank[index].team == 1) {
		double st = 0;
		int sti = -1;
		for (int i = t2s; i <= t2e; i++) {
			if (tank[i].exist) {
				int tmpSt = abs(tank[i].unit->location.x - tank[index].unit->location.x) + abs(tank[i].unit->location.y - tank[index].unit->location.y);
				if (sti == -1 || st > tmpSt) {
					st = tmpSt;
					sti = i;
				}
			}
		}
		if (sti != -1) {
			nextTg = sti;
		}
	}
	if (tank[index].team == 2) {
		double st = 0;
		int sti = -1;
		for (int i = t1s; i <= t1e; i++) {
			if (tank[i].exist) {
				int tmpSt = abs(tank[i].unit->location.x - tank[index].unit->location.x) + abs(tank[i].unit->location.y - tank[index].unit->location.y);
				if (sti == -1 || st > tmpSt) {
					st = tmpSt;
					sti = i;
				}
			}
		}
		if (sti != -1) {
			nextTg = sti;
		}
	}
	if (nextTg == -1) return tg;
	else return nextTg;
}