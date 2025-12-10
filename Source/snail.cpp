#include "Snail.h"

void Snail::Update()
{
	Move();
}

void Snail::Move()
{
	if (IsKeyDown(KEY_W)) {
		velocity.y = -speed;
		body->body->SetLinearVelocity(velocity);
	}
	if (IsKeyDown(KEY_S)) {
		velocity.y = speed;
		body->body->SetLinearVelocity(velocity);
	}
	if (IsKeyDown(KEY_A)) {
		velocity.x = -speed;
		body->body->SetLinearVelocity(velocity);
	}
	if (IsKeyDown(KEY_D)) {
		velocity.x = speed;
		body->body->SetLinearVelocity(velocity);
	}
}
