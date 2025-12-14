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
private:
	Module* listener;
	Application* App;
};