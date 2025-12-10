#include "Snail.h"
#include "ModuleGame.h"

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
	float forceX = 20.0f;


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

void Snail::ApplyFriction(float i_staticFricion, float i_dynamicFriction)
{
	float forceX = 20.0f;
	if (body->body->GetLinearVelocity().LengthSquared() < 0.001f)
	{
		float N = mass * 9.8f;
		float staticFriction = N * i_staticFricion;
		forceX = std::max(0.0f, forceX - staticFriction);
	}
	else
	{
		float N = mass * 9.8f;
		float dynamicFriction = N * i_dynamicFriction;
		forceX = std::max(0.0f, forceX - dynamicFriction);
	}
	float dynamicFrictionCoed = 0.5f;
	body->body->ApplyForce(b2Vec2(forceX, 0.0f), b2Vec2_zero, true);
}
