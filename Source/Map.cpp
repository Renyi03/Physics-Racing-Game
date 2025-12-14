#include "Map.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModulePhysics.h"

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
	checkpoint1 = CreateCheckPoint(602, 1198 + 215 / 2, 10, 215, 0, 0);
	checkpoint2 = CreateCheckPoint(1110, 1198 + 215 / 2, 10, 215, 0, 1);
	checkpoint3 = CreateCheckPoint(1581, 1198 + 215 / 2, 10, 215, -30, 2);
	checkpoint4 = CreateCheckPoint(1769, 1032 + 215 / 2, 10, 215, -60, 3);
	checkpoint5 = CreateCheckPoint(1850, 748 + 215 / 2, 10, 215, -100, 4);
	checkpoint6 = CreateCheckPoint(1538, 639 + 215 / 2, 10, 215, -45, 5);
	checkpoint7 = CreateCheckPoint(1220, 500 + 215 / 2, 10, 215, -100, 6);
	checkpoint8 = CreateCheckPoint(1095, 90 + 215 / 2, 10, 215, 20, 7);
	checkpoint9 = CreateCheckPoint(650, 408 + 215 / 2, 10, 215, -60, 8);
	checkpoint10 = CreateCheckPoint(302, 360 + 215 / 2, 10, 215, 0, 9);
	checkpoint11 = CreateCheckPoint(150, 688 + 215 / 2, 10, 215, -90, 10);
	checkpoint12 = CreateCheckPoint(370, 1008 + 215 / 2, 10, 215, -100, 11);
	checkpoints.push_back(checkpoint1);
	checkpoints.push_back(checkpoint2);
	checkpoints.push_back(checkpoint3);
	checkpoints.push_back(checkpoint4);
	checkpoints.push_back(checkpoint5);
	checkpoints.push_back(checkpoint6);
	checkpoints.push_back(checkpoint7);
	checkpoints.push_back(checkpoint8);
	checkpoints.push_back(checkpoint9);
	checkpoints.push_back(checkpoint10);
	checkpoints.push_back(checkpoint11);
	checkpoints.push_back(checkpoint12);
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

PhysBody* Map::CreateCheckPoint(float x, float y, float w, float h, float r, int num)
{
	PhysBody* cp = App->physics->CreateRectangleSensor(x, y, w, h, r);
	cp->checkpointIndex = num;
	cp->ctype = ColliderType::CHECKPOINT;
	return cp;
}

void Map::OnCollisionWithSnail(PhysBody* bodyA, PhysBody* bodyB)
{
	
}
