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
	PhysBody* CreateCheckPoint(float x, float y, float w, float h, float r, int num);
	PhysBody* Map::CreateMapElement(int x, int y, const int* points, int size, ColliderType type);
	const std::vector<Vector2>& GetWaypoints() const { return waypoints; }
	std::vector<PhysBody*> checkpoints;
private:
	Application* App;
	Module* listener;
	Texture2D mapTexture;
	std::vector<Vector2> waypoints;
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