#include "Sky.h"
void Sky::Init(Texture* texture, Shader* shader) {
	mesh = Mesh::Get("data/terrain/cielo.ASE");
	this->shader = shader;
	this->texture = texture;
	this->Draw_requied = true;
	this->Type = SKY;
}