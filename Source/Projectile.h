#pragma once
#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"
#include "Box.h"

class Snail;

class Projectile: public PhysicEntity 
{
public:
	Projectile(ModulePhysics* physics, Snail* _snail);

	~Projectile();

	void Start();
	void Update();

	Texture2D textureE;
	Texture2D textureN;
	Texture2D textureH;
	Texture2D textureY;
	Texture2D textureP;
protected:
	Snail* snail;

	float radius = 4.0f;
	float speed = 0.5f;
};