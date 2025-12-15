#pragma once
#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleGame.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"
#include "Module.h"

enum PhysicCategory
{
	DEFAULT = 1 << 0,
	SNAIL_CATEGORY = 1 << 1,
	PLANE = 1 << 2,
	CAR = 1 << 3,
	SHIP = 1 << 4,
	BIKE = 1 << 5,
	ALL = 0xFFFF
};

enum PhysicGroup {
	LAND = 1,
};

class PhysicEntity
{
protected:

	PhysicEntity(PhysBody* _body, Module* _listener)
		: body(_body)
		, listener(_listener)
	{
		body->listener = listener;
	}

public:
	virtual ~PhysicEntity() {
		TraceLog(LOG_INFO, "PhysicEntity destructor called for body: %p", body);
		if (body && listener && listener->App && listener->App->physics)
		{
			listener->App->physics->DestroyBody(body);
			body = nullptr;
		}
	};
	virtual void Update() = 0;
	bool active = true;

	virtual int RayHit(vec2<int> ray, vec2<int> mouse, vec2<float>& normal)
	{
		return 0;
	}

public:
	PhysBody* body;
	Module* listener;
};