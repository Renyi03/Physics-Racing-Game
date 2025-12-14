#include "Map.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModulePhysics.h"
#include "Grass.h"
#include "Mud.h"
#include "Ice.h"

Map::Map(Application* app, Module* _listener)
{
	App = app;
	listener = _listener;
}

Map::~Map()
{
	CleanUp();
}

bool Map::Start() {
	mapTexture = LoadTexture("Assets/Textures/Racing_Map2.png");

	//Create checkpoints
	checkpoint1 = CreateCheckPoint(300, 500, 50, 10, 0);
	checkpoint2 = CreateCheckPoint(300, 300, 50, 10, 1);
	checkpoint3 = CreateCheckPoint(300, 100, 50, 10, 2);
	checkpoints.push_back(checkpoint1);
	checkpoints.push_back(checkpoint2);
	checkpoints.push_back(checkpoint3);

	//Create map elements
	grassOutsideBody = App->physics->CreateChainSensor(0, 0, grass->verticesOutside, 198);
	grassInsideBody = App->physics->CreateChainSensor(0, 0, grass->verticesInside, 158);
	iceBody = App->physics->CreateChainSensor(0, 0, ice->vertices, 58);
	mud1Body = App->physics->CreateChainSensor(0, 0, mud->verticesMud1, 34);
	mud2Body = App->physics->CreateChainSensor(0, 0, mud->verticesMud2, 62);
	mud3Body = App->physics->CreateChainSensor(0, 0, mud->verticesMud3, 72);
	mud4Body = App->physics->CreateChainSensor(0, 0, mud->verticesMud4, 62);
	mud5Body = App->physics->CreateChainSensor(0, 0, mud->verticesMud5, 40);
	mud6Body = App->physics->CreateChainSensor(0, 0, mud->verticesMud6, 66);

	return true;
}

bool Map::Update() {
	return true;
}

bool Map::CleanUp() {
	UnloadTexture(mapTexture);
	TraceLog(LOG_INFO, "Map texture unloaded");
	return true;
}

void Map::DrawMapTexture() {
	if (mapTexture.id != 0)
	{
		DrawTextureEx(
			mapTexture,
			Vector2{ (float)App->renderer->camera.x, (float)App->renderer->camera.y },
			0.0f,
			1.0f,
			WHITE
		);
	}
}

PhysBody* Map::CreateCheckPoint(float x, float y, float w, float h, int num)
{
	PhysBody* cp = App->physics->CreateRectangleSensor(x, y, w, h);
	cp->checkpointIndex = num;
	cp->ctype = ColliderType::CHECKPOINT;
	return cp;
}

void Map::OnCollisionWithSnail(PhysBody* bodyA, PhysBody* bodyB)
{
	
}
