#include "Projectile.h"
#include "ModulePhysics.h"
#include "Snail.h"

#include <stdio.h>
#include <time.h>


Projectile::Projectile(ModulePhysics* physics, Snail* _snail)
	: PhysicEntity(physics->CreateCircle(_snail->GetPosition().x + 30 * (_snail->GetForward().x), _snail->GetPosition().y + 30 * (_snail->GetForward().y), 8.0f, 8000.0f), _snail->listener)
{
	snail = _snail;
	body->ctype = ColliderType::PROJECTILE;
}

Projectile::~Projectile()
{
}


int Projectile::randVal(int min, int max) {

	return (rand() % (max - min + 1)) + min;
}


void Projectile::Start()
{
	srand(time(NULL));

	textureE = LoadTexture("Assets/Textures/ProjectileE.png");
	textureN = LoadTexture("Assets/Textures/ProjectileN.png");
	textureH = LoadTexture("Assets/Textures/ProjectileH.png");
	textureY = LoadTexture("Assets/Textures/ProjectileY.png");
	textureP = LoadTexture("Assets/Textures/ProjectileP.png");

	int randomTexture = randVal(1, 7);
	switch (randomTexture) {
		case(1): {
			textureUsed = textureE;
			break;
		}
		case(2): {
			textureUsed = textureN;
			break;
		}
		case(3): {
			textureUsed = textureH;
			break;
		}
		case(4): {
			textureUsed = textureY;
			break;
		}
		case(5): {
			textureUsed = textureP;
			break;
		}
		case(6): {
			textureUsed = textureE;
			break;
		}
		case(7): {
			textureUsed = textureN;
			break;
		}
	}
}


void Projectile::Update() 
{
	if (!active) return;

	int x, y;
	body->GetPhysicPosition(x, y);
	
	DrawTextureEx(
		textureUsed,
		Vector2{ (float)snail->listener->App->renderer->camera.x + x - 8.0f, (float)snail->listener->App->renderer->camera.y + y -8.0f},
		0.0f,
		1.0f,
		WHITE
	);

	if (x <= 10.0f || y <= 10.0f || x >= (MAP_WIDTH - 10) || y >= (MAP_HEIGHT - 10))
	{
		active = false;
	}
}
