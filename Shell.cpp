#include "stdafx.h"
#include "Shell.h"


CShell::CShell()
	:atk(0), x(0), y(0), ro(0), speed(0), exist(false) {

}
CShell::CShell(int p_atk, float p_x, float p_y, float p_ro, float p_speed)
	:atk(p_atk),x(p_x),y(p_y),ro(p_ro),speed(p_speed), exist(true)
{

}


CShell::~CShell()
{
}


point CShell::nextLocation()
{
	point t;
	t.x = x + speed*cos(ro*PI / 180.0);
	t.y = y + speed*sin(ro*PI / 180.0);
	return t;
}


void CShell::move()
{
	point tmp = nextLocation();
	x = tmp.x;
	y = tmp.y;
}

void CShell::set(int p_atk, float p_x, float p_y, float p_ro, float p_speed) {
	atk = p_atk;
	x = p_x;
	y = p_y;
	ro = p_ro;
	speed = p_speed;
	exist = true;
}

void CShell::init()
{
	atk = 0;
	x = 0;
	y = 0;
	ro = 0;
	speed = 0;
	exist = false;
}
