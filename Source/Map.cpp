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
	checkpoint1 = CreateCheckPoint(602, 1230+158/2, 10, 158, 0, 1);
	checkpoint2 = CreateCheckPoint(1110, 1230+158/2, 10, 158, 0, 1);
	checkpoint3 = CreateCheckPoint(300, 100, 50, 10, 0, 2);
	checkpoints.push_back(checkpoint1);
	checkpoints.push_back(checkpoint2);
	checkpoints.push_back(checkpoint3);
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
