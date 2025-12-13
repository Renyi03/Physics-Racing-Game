#include "Map.h"

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