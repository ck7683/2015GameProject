#pragma once
#include "Const.h"
#include <cmath>

class CShell
{
public:
	bool exist;

	int atk;
	float x;
	float y;
	float ro;
	float speed;
public:
	CShell();
	CShell(int atk, float x, float y, float ro, float speed);
	~CShell();
	point nextLocation();
	void move();
	void set(int p_atk, float p_x, float p_y, float p_ro, float p_speed);
	void init();
};

