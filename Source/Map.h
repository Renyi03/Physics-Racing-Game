#pragma once

#include "Module.h"
#include "Application.h"
#include "Grass.h"
#include "Mud.h"
#include "Ice.h"

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
	Grass* grass;
	PhysBody* grassOutsideBody;
	PhysBody* grassInsideBody;
	Ice* ice;
	PhysBody* iceBody;
	Mud* mud;
	PhysBody* mud1Body;
	PhysBody* mud2Body;
	PhysBody* mud3Body;
	PhysBody* mud4Body;
	PhysBody* mud5Body;
	PhysBody* mud6Body;
	
};