#include "Ice_Shield.h"

void Ice_Shield::Init(Shader* shader)
{
	mesh = Mesh::Get("data/seres/icecube.obj");
	this->shader = shader;
	this->Type = ICE_SHIELD;
	Hp = Hp_MAX;
	reduced = false;
}

void Ice_Shield::update(float elapsed_time)
{

}

void Ice_Shield::getDamage(int damage)
{
	this->Hp -= damage;
	if (this->Hp <= Hp_MAX/2 && !reduced) {
		this->model.scale(0.7, 0.7, 0.7);
		reduced = true;
	}
	if (this->Hp < 0) {
		Hp = 0;
	}

	//std::cout << Hp << std::endl;
}
