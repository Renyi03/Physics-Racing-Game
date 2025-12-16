#pragma once
#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"
#include "Box.h"

class Snail;

class AdoYell : public PhysicEntity
{
public:
	AdoYell(ModulePhysics* physics, Snail* ownerSnail, float radius);

	~AdoYell();

	void Update();
	Snail* GetOwner();
	PhysBody* GetBody() const { return body; }
public:
	bool active = true;

private:
	Snail* owner = nullptr;
	float yellTimer = 0.0f;
	float yellDuration = 2.5f;
};