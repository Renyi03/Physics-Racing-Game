#include "Snail.h"
#include "ModuleGame.h"

void Snail::Update()
{
	Move();
	Box::Update();
}

void Snail::Move()
{
	b2Vec2 force(0.0f, 0.0f);

	if (IsKeyDown(KEY_W)) {
		force.y -= moveForce;
	}
	if (IsKeyDown(KEY_S)) {
		force.y += moveForce;
	}
	if (IsKeyDown(KEY_A)) {
		force.x -= moveForce;
	}
	if (IsKeyDown(KEY_D)) {
		force.x += moveForce;
	}

	//Apply friction to the force
	ApplyFriction(staticFrictionCoeff, dynamicFrictionCoeff);
	
	//Now apply the force reduced by friction
	body->body->ApplyForceToCenter(force, true);
}

void Snail::ApplyFriction(float staticFriction, float dynamicFriction)
{
	b2Vec2 velocity = body->body->GetLinearVelocity();
	float speedSquared = velocity.LengthSquared();

	//If the snail is moving
	if (speedSquared > 0.001f) {
		//Calculate friction force opposite to velocity direction
		b2Vec2 frictionDir = velocity;
		frictionDir.Normalize();
		frictionDir *= -1.0f;

		//Dynamic friction when moving
		float N = mass * 9.8f;
		float frictionMagnitude = N * dynamicFriction;

		b2Vec2 frictionForce = frictionDir;
		frictionForce *= frictionMagnitude;

		body->body->ApplyForceToCenter(frictionForce, true);
	}
	else if (speedSquared < 0.001f){
		//Step speed if too slow
		body->body->SetLinearVelocity({ 0.0f, 0.0f });
	}
}
