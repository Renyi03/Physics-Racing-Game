#include "Snail.h"
#include "ModuleGame.h"
#include "Application.h"
#include "ModulePhysics.h"
#include "Map.h"

#include <vector>

void Snail::Update()
{
	Move();

	if (active) {
		if (IsKeyPressed(KEY_SPACE) && !isSlobber) {
			isSlobber = true;
			saliva.clear();
		}
		Saliva();
	}

	Box::Update();
}

void Snail::Move()
{
	b2Vec2 inputDir(0.0f, 0.0f);

	if (active) {
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
	}

	bool has_input = inputDir.LengthSquared() > 0.0f;

	// normalize input direction if going in diagonal
	if (has_input) {
		inputDir.Normalize();
	}

	// snail's current forward direction
	float angle = body->body->GetAngle() - 90 * DEG2RAD;
	forward = { float(cos(angle)), float(sin(angle)) };

	// apply force to the "forward" direction (direction snail is facing)
	b2Vec2 force(0.0f, 0.0f);
	if (has_input) {
		float forwardInput = b2Dot(inputDir, forward); //checking if player input is same as snail is facing 
		float forceMagnitude = moveForce * fmax(0.3f, fabs(forwardInput)); // minimum 30% force when snail is not facing the input direction. slow when not facing "correct" direction
		force = b2Vec2(forward.x * forceMagnitude, forward.y * forceMagnitude);
	}

	//Apply friction
	ApplyFriction(staticFrictionCoeff, dynamicFrictionCoeff);
	//Apply forward force
	body->body->ApplyForceToCenter(force, true);

	Rotate(inputDir, forward, has_input);

	b2Vec2 right(-forward.y, forward.x);
	ApplyLateralFriction(right);
}

void Snail::ApplyLateralFriction(const b2Vec2& right)
{
	//little bit of drifting!!!

	b2Vec2 vel = body->body->GetLinearVelocity();
	float lateralVel = b2Dot(vel, right);
	b2Vec2 lateralImpulse = b2Vec2(
		-lateralVel * lateralDamping * body->body->GetMass() * right.x,
		-lateralVel * lateralDamping * body->body->GetMass() * right.y
	);

	body->body->ApplyLinearImpulseToCenter(lateralImpulse, true);
}


void Snail::Rotate(b2Vec2 inputDir, const b2Vec2& forward, bool has_input) {
	b2Vec2 vel = body->body->GetLinearVelocity();
	float currentAngle = body->body->GetAngle();

	float targetAngle = currentAngle;
	bool shouldRotate = false;

	// if there's input, rotate towards input direction
	if (has_input) {
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
		float speed_factor = fminf(1.0f, speed / rotation_speed_scale); // adjust 50.0f based on max speed
		float turnRate = rotation_base_rate * (0.5f + speed_factor * 0.5f) * GetFrameTime();

		//low speed circular movement instead of just rotating in place
		if (speed < low_speed_threshold && has_input)
		{
			// Get current forward direction
			float fAngle = currentAngle - 90 * DEG2RAD;
			b2Vec2 forward(cos(fAngle), sin(fAngle));

			// Small forward push so turning makes a circle instead of rotating in place
			b2Vec2 tiny_push(forward.x * tiny_push_strength, forward.y * tiny_push_strength);
			body->body->ApplyForceToCenter(tiny_push, true);
		}

		//if snail is almost facing the target angle, snap to it, otherwise gradually move toward it
		if (fabs(diff) < turnRate)
			currentAngle = targetAngle;
		else
			currentAngle += turnRate * (diff > 0 ? 1.0f : -1.0f);
		
		// apply to body
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
		float renderX = pos.x + listener->App->renderer->camera.x;
		float renderY = pos.y + listener->App->renderer->camera.y;
		DrawRectangle((int)renderX - 2, (int)renderY - 2, 4, 4, GRAY);
	}
}

void Snail::Saliva()
{
	if (!isSlobber) return;

	b2Vec2 vel = body->body->GetLinearVelocity();
	float speed = vel.Length();

	salivaTimer += GetFrameTime(); // raylib-style frame delta time

	if (salivaTimer >= salivaInterval)
	{
		salivaTimer = 0.0f;
		saliva.push_back(GetPosition());
	}

	int i = 0;
	for (const auto& pos : saliva)
	{
		
		float renderX = pos.x + listener->App->renderer->camera.x;
		float renderY = pos.y + listener->App->renderer->camera.y;
		DrawRectangle((int)renderX - 2, (int)renderY - 2, 20, 20, GREEN);
		//salivaBodies[i].body->SetTransform(b2Vec2((int)renderX - 2, (int)renderY - 2), 0);

		i++;
	}

	slobberTimer += GetFrameTime();
	if (slobberTimer >= 2.0f) {
		slobberTimer = 0.0f;
		isSlobber = false;
	}
}

Vector2 Snail::GetPosition() const
{
    int x = 0, y = 0;
    if (body != nullptr)
        body->GetPhysicPosition(x, y);
    return { (float)x, (float)y };
}

void Snail::OnCollisionWithMap(PhysBody* mapObject)
{
	//Checks for the type of map object the snail collided with
	if (mapObject == nullptr) {
		return;
	}
	switch (mapObject->ctype) {
	case ColliderType::CHECKPOINT:
		int checkpointNum = mapObject->checkpointIndex;
		ModuleGame* game = dynamic_cast<ModuleGame*>(listener);
		game->CheckpointManager(this, checkpointNum);
		break;

	case ColliderType::ICE:
		//collidingWithGrass = false;
		dynamicFrictionCoeff = 0.1f;
		break;

	case ColliderType::MUD:
		//collidingWithGrass = false;
		dynamicFrictionCoeff = 2.5f;
		break;

	case ColliderType::GRASS:
		dynamicFrictionCoeff = 1.5f;
		break;

	default:
		dynamicFrictionCoeff = 0.3f;
		break;
	}
}
