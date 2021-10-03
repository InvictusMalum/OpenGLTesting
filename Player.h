#pragma once
#include"Vector2.h"
#include"Ray.h"
#include<list>

using namespace std;

class Player
{
public:
	Vector2 findValidSpawn();
	Player();
	Player(Vector2 pos_);
	Player(Vector2 pos_, float ang_);
	Ray createRay(float angle);
	void populateRays();

	Vector2 pos;
	double angle;
	int FOV = 60;
	int numRays = 4;

	std::list<Ray> rays;
};