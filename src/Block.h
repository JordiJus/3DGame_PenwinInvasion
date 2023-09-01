#pragma once
#include "EntityMesh.h"
class Block: public EntityMesh {
public:
	int row;
	int column;
	bool obj_Placed = false;
	void Init(Shader* shader);
};

