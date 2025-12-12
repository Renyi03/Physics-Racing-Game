#include "MikuSnail.h"
#include "ModulePhysics.h"
#include "Box.h"
#include "Snail.h"
#include "PhysicEntity.h"

void MikuSnail::UseAbility() {
	body->body->ApplyLinearImpulseToCenter( 2*forward, true);
}

void MikuSnail::Update()
{
	Snail::Update();

	if (IsKeyPressed(KEY_E)) {
		UseAbility();
	}
}