#pragma once 
#include <vector> 
#include "raylib.h" 
#include "box2d/box2d.h" 
class Snail;
class SnailAI {
public:
	SnailAI(Snail* owner);
	void Update();
	void SetWaypoints(const std::vector<Vector2>& points);
	float maxSpeed = 2.5f; // slower than player 
	float steeringForce = 0.8f;
private:
	Snail* snail;
	std::vector<Vector2> waypoints;
	int currentWaypoint = 0;
	float stuckTimer = 0.0f;
};