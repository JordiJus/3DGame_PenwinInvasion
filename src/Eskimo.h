#pragma once
#include "EntityMesh.h"
#include "Pengu.h"
#include "Block.h"
#include "Bear.h"
#include "Ice_Shield.h"
#include "animation.h"



class Eskimo : public EntityMesh {
public:

	int Hp;
	float yaw = 0.f;
	float pitch = 0.f;
	int speed;
	const int Max_snowFlake_Value = 9999;
	int snowFlake_Value;
	bool obj_On_Hand = false;
	Type_OBJ objType_On_Hand = NONE;
	bool picked_SnowFlake = false;
	//bool is_Place_Pengu = false;
	Pengu* pengu_Placeable;
	Ice_Shield* iceShield_Placeable;
	std::vector<Block*> lBlocks;
	std::vector<Pengu*> lPengus;
	std::vector<Ice_Shield*> lIce_Shields;
	//Shader* shader_Pengu = nullptr;
	void Init(Shader* shader);
	void move(Vector3 movement, float seconds_elapsed);
	void update(float elapsed_time);
	void render();
	void place_Pengu();
	void place_IceShield();
	void Init_Placeable_Pengu(Shader* shader);
	void Init_Placeable_IceShield(Shader* shader);
	void Find_Pengus_Target(std::vector<Bear*> bears);
	Block* Get_Placeable_Block();
	void Set_SnowFlake_Value(int value);
	void getDamage(int damage);

	Animation* animA;
	Animation* animB;
};

