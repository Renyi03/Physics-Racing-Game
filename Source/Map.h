#pragma once

#include "Module.h"
class Application;

class Map {
public:
	Map(Application* app, Module* _listener);
	~Map();

	bool Start();
	bool Update();
	bool CleanUp();
private:
	Texture2D mapTexture;
};