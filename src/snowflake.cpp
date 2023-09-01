#include "snowflake.h"
#include "game.h"

void snowflake::Init(Shader* shader)
{
	mesh = Mesh::Get("data/seres/snowflake.obj");
	this->shader = shader;
	this->Type = SNOWFLAKE;
	value = 50;
}

void snowflake::update(float elapsed_time)
{
#pragma region Rotate
	Matrix44 currentRotation = model.getRotationOnly();

	//float currentAngle = std::atan2(currentRotation.M[1][0], currentRotation.M[0][0]);

	//float newAngle = currentAngle + 0.1f;

	Vector3 axis(0.0f, 1.0f, 0.0f);

	model.rotate(M_PI * 2.f * elapsed_time, axis);
#pragma endregion

#pragma region Collisions
	Eskimo* eskimo = Game::instance->_world->Get_Eskimo();

	Vector3 col_p;
	Vector3 col_n;

	if (mesh->testSphereCollision(eskimo->model, model.getTranslation(), 2, col_p, col_n)) {
		eskimo->Set_SnowFlake_Value(value);
		eskimo->picked_SnowFlake = true;
		Game::instance->_world->Remove_SnowFlake();
	}
#pragma endregion
}
