#pragma once
#include "Box.h"
#include "ModulePhysics.h"
#include "Application.h"
#include "ModuleAudio.h"

class Projectile: public PhysicEntity 
{
public:
	Projectile(ModulePhysics* physics);
	bool Start();
	bool CleanUp();
	void Update();
	void FireArrow(Vector2 direction);

protected:
	PhysBody* body;
	Texture2D textureE;
	Texture2D textureN;
	Texture2D textureH;
	Texture2D textureY;
	Texture2D textureP;

	float radious = 4.0f;
	float speed = 0.5f;

	Application* App;
};