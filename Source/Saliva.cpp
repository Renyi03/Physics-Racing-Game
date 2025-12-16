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
            if (body) {
                listener->App->physics->DestroyBody(body);
                body = nullptr;
            }
        }
    }
    if (active) {
        Box::Update();
    }
}

