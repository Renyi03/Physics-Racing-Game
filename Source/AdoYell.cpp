#include "AdoYell.h"
#include "ModulePhysics.h"
#include "Snail.h"

AdoYell::AdoYell(ModulePhysics* physics, Snail* ownerSnail, float radius)
	: PhysicEntity(physics->CreateCircleSensor(ownerSnail->GetPosition().x, ownerSnail->GetPosition().y, radius)
		, ownerSnail->listener)
{
	owner = ownerSnail;
	body->ctype = ColliderType::ADO_YELL;
}

AdoYell::~AdoYell()
{
}

void AdoYell::Update()
{
	if (!active) return;

	yellTimer += GetFrameTime();

	b2Vec2 pos(owner->GetPosition().x / PIXELS_PER_METER, owner->GetPosition().y / PIXELS_PER_METER);
	body->body->SetTransform(pos, 0.0f);

	if (yellTimer >= yellDuration)
	{
		active = false;
	}
}

Snail* AdoYell::GetOwner()
{
	return owner;
}
