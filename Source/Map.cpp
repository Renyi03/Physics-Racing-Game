#include "Map.h"
#include "Application.h"

Map::Map(Application* app, Module* _listener)
{
}

Map::~Map()
{
}

bool Map::Start() {
	mapTexture = LoadTexture("Assets/Textures/Racing_Map.png");
	return true;
}

bool Map::Update() {
	return true;
}

bool Map::CleanUp() {
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