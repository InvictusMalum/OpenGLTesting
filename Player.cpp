#include <algorithm>
#include <iostream>
#include <numeric>
#include <string>
#include <list>
#include<math.h>
#include"Player.h"

using namespace std;


Vector2 Player::findValidSpawn()
{
	return Vector2(0.0f, 0.0f);
}

Player::Player()
{
	pos = findValidSpawn();
	angle = 0;
}

Player::Player(Vector2 pos_)
{
	pos = pos_;
	angle = 0;
}

Player::Player(Vector2 pos_, float ang_)
{
	pos = pos_;
	angle = ang_;
}

void Player::Turn(float ang_)
{
	angle += ang_;
	if (angle < 0) {
		angle += 360;
	}
	else if (angle >= 360)
	{
		angle -= 360;
	}
	std::cout << angle << std::endl;
}

Ray Player::CreateRay(float rayAngle)
{
	Vector2 increment = Ray::getVectorAtAngle(rayAngle);
	return Ray(Vector2(pos.x, pos.y), increment);
}

void Player::PopulateRays()
{
	float anglePerRay = FOV / (numRays-1);
	float startAngle = -FOV / 2;
	for (int i = 0; i < numRays; i++)
	{
		rays.push_back(CreateRay(startAngle+anglePerRay*i));
	}
	//std::for_each(rays.begin(), rays.end(), [](Ray n) { std::cout << n.increaseIncrement.x << ' ' << n.increaseIncrement.y << std::endl; });
}

void Player::DrawSelf()
{
	
}

