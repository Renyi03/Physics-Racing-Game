#pragma once

#include "Globals.h"
#include "Module.h"

#include "p2Point.h"

#include "raylib.h"
#include <vector>

class Snail;
class EnhypenSnail;
class ChopinSnail;
class AdoSnail;
class MikuSnail;

class PhysBody;
class PhysicEntity;


class ModuleGame : public Module
{
public:
	ModuleGame(Application* app, bool start_enabled = true);
	~ModuleGame();

	bool Start();
	update_status Update();
	bool CleanUp();
	void OnCollision(PhysBody* bodyA, PhysBody* bodyB);

public:
	std::vector<PhysicEntity*> entities;

	Texture2D plane;
	Texture2D car;
	Texture2D ship;
	Texture2D bike;

	EnhypenSnail* enhypenSnail;
	ChopinSnail* chopinSnail;
	AdoSnail* adoSnail;
	MikuSnail* mikuSnail;

	vec2<int> ray;
	bool ray_on;

	bool snailChosen = false;
	
	std::vector<float> m_staticFrictions = { 0.0f, 0.1f, 0.3f, 0.5f };
	std::vector<float> m_dynamicFrictions = { 0.0f, 0.1f, 0.3f, 0.5f };
};
