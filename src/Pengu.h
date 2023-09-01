#pragma once
#include "EntityMesh.h"
#include "Ice_Shoot.h"

class Pengu : public EntityMesh {
public:
	void Init(Shader* shader);
	Shader* shader_Shoot = nullptr;
	int row;
	int column;
	int Hp;
	float shoot_ColdDown;
	float next_Shoot_Time;
	void update(float elapsed_time);
	bool can_Shoot = true;
	bool has_Target = false;
	void shoot();
	std::vector<Ice_Shoot*> lIce_Shoots;
	void render();
	void getDamage(int damage);

	//HCHANNEL channel;
	//Audio* iceShoot = new Audio();
};

