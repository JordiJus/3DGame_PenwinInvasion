#pragma once
#include "EntityMesh.h"

class Ice_Shoot : public EntityMesh {
public:
	void Init(Shader* shader);
	int damage;
	int speed;
	bool hitted;
	void update(float elapsed_time);
	float disappear_Time;
};

