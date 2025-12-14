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

	Texture2D mapTexture;


	void DrawMapTexture();
	PhysBody* CreateCheckPoint(float x, float y, float w, float h, float rotation, int num);
	void OnCollisionWithSnail(PhysBody* bodyA, PhysBody* bodyB);
	std::vector<PhysBody*> checkpoints;
private:
	Application* App;
	Module* listener;
	PhysBody* checkpoint1;
	PhysBody* checkpoint2;
	PhysBody* checkpoint3;
};