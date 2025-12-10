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
	DrawTexturePro(texture, source, dest, origin, rotation, WHITE);
	Move();
	GetPosition();
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

void Snail::Trail() {
    int x = GetPosition().x;
    int y = GetPosition().y;

	DrawRectangle(x, y, 26, 48, Fade(BLACK, 0.6f));
}

Vector2 Snail::GetPosition() const
{
    int x = 0, y = 0;
    if (body != nullptr)
        body->GetPhysicPosition(x, y);
    return { (float)x, (float)y };
}
