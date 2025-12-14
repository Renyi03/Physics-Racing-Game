#pragma once

#include "Module.h"
#include "Application.h"

class Map {
public:
	Map(Application* app, Module* _listener);
	~Map();

	bool Start();
	bool Update();
	bool CleanUp();
	void DrawMapTexture();
	PhysBody* CreateCheckPoint(float x, float y, float w, float h, int num);
	void OnCollisionWithSnail(PhysBody* bodyA, PhysBody* bodyB);
	std::vector<PhysBody*> checkpoints;
private:
	Application* App;
	Module* listener;
	Texture2D mapTexture;
	PhysBody* checkpoint1;
	PhysBody* checkpoint2;
	PhysBody* checkpoint3;
};