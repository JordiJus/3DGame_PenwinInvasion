#include "Bear.h"
#include "game.h"
void Bear::Init(Shader* shader)
{
	mesh = Mesh::Get("data/seres/bear.obj");
	this->shader = shader;
	this->Type = BEAR;
	this->Hp = 100;
	damage = 50;
	points = Hp;
	move_speed = 2;
}

void Bear::update(float elapsed_time)
{
	Vector3 position = model.getTranslation();

#pragma region Check Finished Game
	if (position.z <= Game::instance->_world->Dead_Line)
	{
		Game::instance->_world->Life -= 10;
		std::cout << Game::instance->_world->Life << std::endl;
		this->getDamage(Hp);
	}
#pragma endregion

#pragma region Collisions
	Eskimo* eskimo = Game::instance->_world->Get_Eskimo();
	Vector3 col_p;
	Vector3 col_n;

	bool found_Target = false;
	long now = SDL_GetTicks();
	for (int i = 0; i < eskimo->lPengus.size(); i++) {
		if (mesh->testSphereCollision(eskimo->lPengus[i]->model, position, 0.705f, col_p, col_n)) {
			if (eskimo->lPengus[i]->row == this->row) {
				found_Target = true;
				if (can_Hit) {
					eskimo->lPengus[i]->getDamage(this->damage);
					can_Hit = false;
					next_Hit_Time = now + hit_ColdDown;
				}
				else {
					if (now >= next_Hit_Time) {
						can_Hit = true;
					}
				}
				break;
			}
		}
	}
	if (!found_Target) {
		for (int i = 0; i < eskimo->lIce_Shields.size(); i++) {
			if (mesh->testSphereCollision(eskimo->lIce_Shields[i]->model, position, 1.7f, col_p, col_n)) {
				if (eskimo->lIce_Shields[i]->row == this->row) {
					found_Target = true;
					if (can_Hit) {
						eskimo->lIce_Shields[i]->getDamage(this->damage);
						can_Hit = false;
						next_Hit_Time = now + hit_ColdDown;
					}
					else {
						if (now >= next_Hit_Time) {
							can_Hit = true;
						}
					}
					break;
				}
			}
		}
	}

	if (!found_Target) {
		if (mesh->testSphereCollision(eskimo->model, position, 2, col_p, col_n)) {
			found_Target = true;
			if (can_Hit) {
				eskimo->getDamage(this->damage);
				can_Hit = false;
				next_Hit_Time = now + hit_ColdDown;
			}
			else {
				if (now >= next_Hit_Time) {
					can_Hit = true;
				}
			}
		}
	}
	

#pragma endregion


#pragma region Move


	if (found_Target)
		move_speed = 0;
	else
		move_speed = 2;

	position = position + (Vector3(0.f, 0.f, -1.f) * elapsed_time * move_speed);

	model.setTranslation(position.x, position.y, position.z);

	float angle_in_rad = 3.14159f; // 180 c
	Vector3 axis(0.0f, 1.0f, 0.0f);

	model.rotate(angle_in_rad, axis);
#pragma endregion

}

void Bear::getDamage(int damage) {
	this->Hp -= damage;
	if (this->Hp < 0) {
		Hp = 0;
	}
}