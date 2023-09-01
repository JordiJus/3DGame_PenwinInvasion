#include "Ground.h"
void Ground::Init(Shader* shader)
{
	mesh = Mesh::Get("data/terrain/plane.obj");
	this->shader = shader;
	this->Draw_requied = true;
	this->Type = FLOOR;
}