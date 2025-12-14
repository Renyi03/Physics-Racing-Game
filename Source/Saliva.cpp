#include "Saliva.h"
#include "ModuleGame.h"
#include "Application.h"
#include "ModulePhysics.h"

#include <vector>

void Saliva::Update()
{
	if (active) {
		timer += GetFrameTime();
		if (timer >= 2.0f) {
			active = false;
			body->listener->App->physics->DestroyBody(body);
		}
	}
	Box::Update();
}

