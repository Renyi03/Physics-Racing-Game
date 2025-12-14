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
	PhysBody* checkpoint4;
	PhysBody* checkpoint5;
	PhysBody* checkpoint6;
	PhysBody* checkpoint7;
	PhysBody* checkpoint8;
	PhysBody* checkpoint9;
	PhysBody* checkpoint10;
	PhysBody* checkpoint11;
	PhysBody* checkpoint12;
};