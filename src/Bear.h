#pragma once
#include "EntityMesh.h"
class Bear : public EntityMesh {
public:
	int Hp;
	int row;
	int column;
	int move_speed = 2;
	int hit_ColdDown = 3000;
	int damage;
	int points;
	bool can_Hit = true;
	int next_Hit_Time;
	void Init(Shader* shader);
	void update(float elapsed_time);
	void getDamage(int damage);
};

