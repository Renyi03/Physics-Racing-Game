#include "Snail.h"
#include "ModuleGame.h"

void Snail::Update()
{
	Move();
	Trail();
	Box::Update();
}

void Snail::Move()
{
	b2Vec2 inputDir(0.0f, 0.0f);

	if (IsKeyDown(KEY_W)) {
		inputDir.y -= 1.0f;
	}
	if (IsKeyDown(KEY_S)) {
		inputDir.y += 1.0f;
	}
	if (IsKeyDown(KEY_A)) {
		inputDir.x -= 1.0f;
	}
	if (IsKeyDown(KEY_D)) {
		inputDir.x += 1.0f;
	}

	// normalize input direction if going in diagonal
	if (inputDir.LengthSquared() > 0.0f) {
		inputDir.Normalize();
	}

	// snail's current forward direction
	float angle = body->body->GetAngle() - 90 * DEG2RAD;
	b2Vec2 forward(cos(angle), sin(angle));

	// apply force to the "forward" direction (direction snail is facing)
	b2Vec2 force(0.0f, 0.0f);
	if (inputDir.LengthSquared() > 0.0f) {
		float forwardInput = b2Dot(inputDir, forward); //checking if player input is same as snail is facing 
		float forceMagnitude = moveForce * fmax(0.3f, fabs(forwardInput)); // minimum 30% force when snail is not facing the input direction. slow when not facing "correct" direction
		force.x = forward.x * forceMagnitude;
		force.y = forward.y * forceMagnitude;
	}

	//Apply friction
	ApplyFriction(staticFrictionCoeff, dynamicFrictionCoeff);
	//Apply forward force
	body->body->ApplyForceToCenter(force, true);

	Rotate(inputDir);
	ApplyLateralFriction();
}

void Snail::ApplyLateralFriction()
{
	//little bit of drifting!!!

	b2Vec2 vel = body->body->GetLinearVelocity();
	float angle = body->body->GetAngle() - 90 * DEG2RAD;
	b2Vec2 forward(cos(angle), sin(angle));
	// sideways vector (perpendicular)
	b2Vec2 right(-forward.y, forward.x);
	// lateral speed = projection of velocity on sideways axis
	float lateralVel = b2Dot(vel, right);

	float damping = 0.2f;  // adjust between 0.1-0.5 for more/less drift
	b2Vec2 lateralImpulse = -lateralVel * damping * body->body->GetMass() * right;

	// apply to body
	body->body->ApplyLinearImpulseToCenter(lateralImpulse, true);
}


void Snail::Rotate(b2Vec2 inputDir) {
	b2Vec2 vel = body->body->GetLinearVelocity();
	float currentAngle = body->body->GetAngle();

	float targetAngle = currentAngle;
	bool shouldRotate = false;

	// if there's input, rotate towards input direction
	if (inputDir.LengthSquared() > 0.01f) {
		targetAngle = atan2(inputDir.y, inputDir.x) + 90.0f * DEG2RAD;
		shouldRotate = true;
	}
	// if moving but no input, rotate towards velocity
	else if (vel.LengthSquared() > 0.1f) {
		targetAngle = atan2(vel.y, vel.x) + 90.0f * DEG2RAD;
		shouldRotate = true;
	}

	if (shouldRotate)
	{
		//rotates efficiently (takes shortest path to rotate)
		float diff = targetAngle - currentAngle;
		diff = atan2(sinf(diff), cosf(diff));

		// speed-dependent turn rate: slower when moving slowly
		float speed = vel.Length();
		float baseRate = 3.0f;
		float speedFactor = fmin(1.0f, speed / 50.0f); // adjust 50.0f based on max speed
		float turnRate = baseRate * (0.5f + speedFactor * 0.5f) * GetFrameTime();

		if (fabs(diff) < turnRate)
			currentAngle = targetAngle;
		else
			currentAngle += turnRate * (diff > 0 ? 1.0f : -1.0f);

		body->body->SetTransform(body->body->GetPosition(), currentAngle);
	}
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

void Snail::Trail() {
	b2Vec2 vel = body->body->GetLinearVelocity();
	float speed = vel.Length();

	// Only leave trail when moving
	if (speed > 0.5f)
	{
		trailTimer += GetFrameTime(); // raylib-style frame delta time

		if (trailTimer >= trailInterval)
		{
			trailTimer = 0.0f;
			trail.push_back(GetPosition());
		}
	}

	for (const auto& pos : trail)
	{
		DrawRectangle((int)pos.x - 2, (int)pos.y - 2, 4, 4, GRAY);
	}
}

Vector2 Snail::GetPosition() const
{
    int x = 0, y = 0;
    if (body != nullptr)
        body->GetPhysicPosition(x, y);
    return { (float)x, (float)y };
}
