#include "Projectile.h"
#include "ModulePhysics.h"
#include "Application.h"

Projectile::Projectile(ModulePhysics* physics):PhysicsEntity(physics->CreateCircle(posX, posY, radious))
{
}

bool Projectile::Start() 
{
	textureE = LoadTexture("Assets/Textures/ProjectileE.png");
	textureN = LoadTexture("Assets/Textures/ProjectileN.png");
	textureH = LoadTexture("Assets/Textures/ProjectileH.png");
	textureY = LoadTexture("Assets/Textures/ProjectileY.png");
	textureP = LoadTexture("Assets/Textures/ProjectileP.png");

	body = App->physics->CreateCircle(posX, posY, radious);
}

bool Projectile::CleanUp() 
{

}

void Projectile::Update() 
{

}

void Projectile::FireArrow(Vector2 direction)
{

}
