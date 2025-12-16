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
	if (body && body->body) {
		active = false;
	}
}

void AdoYell::Start() {
	yellTexture = LoadTexture("Assets/Textures/Yell.png");
}

void AdoYell::Update()
{
	if (!active) return;

	yellTimer += GetFrameTime();

	b2Vec2 pos(owner->GetPosition().x / PIXELS_PER_METER, owner->GetPosition().y / PIXELS_PER_METER);
	body->body->SetTransform(pos, 0.0f);

	DrawTextureEx(
		yellTexture,
		Vector2{ owner->listener->App->renderer->camera.x + METERS_TO_PIXELS(pos.x) - 120.0f, owner->listener->App->renderer->camera.y + METERS_TO_PIXELS(pos.y) - 120.0f },
		0.0f,
		1.0f,
		WHITE
	);

	if (yellTimer >= yellDuration)
	{
		active = false;
	}
}
