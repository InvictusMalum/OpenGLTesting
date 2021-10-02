#pragma once
#include"Vector2.h"

class Player
{
public:
	Vector2 pos;
	double ang;

	int FOV;

	Vector2 createRay(int angle);
};