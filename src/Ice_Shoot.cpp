#include "Ice_Shoot.h"
#include "game.h"

void Ice_Shoot::Init(Shader* shader)
{
	mesh = Mesh::Get("data/seres/ice_pick.obj");
	this->shader = shader;
	this->Type = ICE_SHOOT;
	this->damage = 20;
	this->speed = 10;
	hitted = false;

	long now = SDL_GetTicks();
	disappear_Time = now + 4000;
}

void Ice_Shoot::update(float elapsed_time)
{
	Vector3 position = model.getTranslation();

	position = position + (Vector3(0.f, 0.f, 1.f) * elapsed_time * 10);

	model.setTranslation(position.x, position.y, position.z);

	model.rotate(M_PI / 2, Vector3(0.0f, -1.0f, 0.0f));

#pragma region Collisions
	std::vector<Bear*> bears = Game::instance->_world->Get_Bears();

	Vector3 col_p;
	Vector3 col_n;

	for (int i = 0; i < bears.size(); i++) {
		if (mesh->testSphereCollision(bears[i]->model, position, 2, col_p, col_n)) {
			//std::cout << bears[i]->health << std::endl;
			bears[i]->getDamage(this->damage);
			hitted = true;
		}
	}
#pragma endregion

	long now = SDL_GetTicks();

	if (now >= disappear_Time) {
		hitted = true;
	}
}