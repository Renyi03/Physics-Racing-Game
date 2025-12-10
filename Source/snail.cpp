#include "Snail.h"

void Snail::Update()
{
	texture = LoadTexture("Assets/Textures/Enhypen_Snail.png");
	int x, y;
	body->GetPhysicPosition(x, y);
	Vector2 position{ (float)x, (float)y };
	float scale = 1.0f;
	Rectangle source = { 0.0f, 0.0f, (float)texture.width, (float)texture.height };
	Rectangle dest = { position.x, position.y, (float)texture.width * scale, (float)texture.height * scale };
	Vector2 origin = { (float)texture.width / 2.0f, (float)texture.height / 2.0f };
	float rotation = body->GetRotation() * RAD2DEG;
	DrawTexturePro(texture, source, dest, origin, rotation, WHITE);
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
