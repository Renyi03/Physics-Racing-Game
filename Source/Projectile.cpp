#include "Projectile.h"
#include "ModulePhysics.h"
#include "Snail.h"

Projectile::Projectile(ModulePhysics* physics, Snail* _snail)
	: PhysicEntity(physics->CreateCircle(_snail->GetPosition().x, _snail->GetPosition().y, 4.0f, 8000.0f), _snail->listener)
{
	snail = _snail;
	body->ctype = ColliderType::PROJECTILE;
}

Projectile::~Projectile()
{
}

void Projectile::Start()
{
	textureE = LoadTexture("Assets/Textures/ProjectileE.png");
	textureN = LoadTexture("Assets/Textures/ProjectileN.png");
	textureH = LoadTexture("Assets/Textures/ProjectileH.png");
	textureY = LoadTexture("Assets/Textures/ProjectileY.png");
	textureP = LoadTexture("Assets/Textures/ProjectileP.png");
}

void Projectile::Update() 
{
	if (!active) return;

	int x, y;
	body->GetPhysicPosition(x, y);

	DrawTextureEx(
		textureE,
		Vector2{ (float)snail->listener->App->renderer->camera.x, (float)snail->listener->App->renderer->camera.y },
		0.0f,
		1.0f,
		WHITE
	);

	if (x <= 0.0f || y <= 0.0f || x >= MAP_WIDTH || y >= MAP_HEIGHT)
	{
		active = false;
	}
}
