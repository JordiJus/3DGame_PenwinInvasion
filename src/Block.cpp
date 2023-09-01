#include "Block.h"

void Block::Init(Shader* shader) {
	mesh = Mesh::Get("data/grid.obj");
	this->shader = shader;
	this->texture = texture;
	this->Draw_requied = true;
	this->Type = GRID;
}