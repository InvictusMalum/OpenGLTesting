#pragma once

#ifndef RAY_H
#define RAY_H

#include"Vector2.h"

using namespace std;

class Ray
{
public:
	Ray();
	Ray(Vector2 startPos_);
	Ray(Vector2 startPos_, Vector2 increaseIncrement_);
	bool CheckCollide(Vector2 point, Vector2 lineMap[][2]);
	void CastUntilHits();
	void setStart(Vector2 startPos_);
	void setIncreaseIncrement(Vector2 increaseIncrement_);

	static Vector2 getVectorAtAngle(float angle);
	
	Vector2 start;
	Vector2 increaseIncrement;
	Vector2 totalIncrease;
	Vector2 end;
};

#endif