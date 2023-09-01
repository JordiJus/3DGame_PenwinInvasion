#pragma once
#include "mesh.h"
#include "texture.h"
#include "Entity.h"
#include "shader.h"
#include "includes.h"
#include "input.h"

enum Type_OBJ {
	NONE, ESKIMO, PENGU, BEAR, SKY, FLOOR, GRID,ICE_SHOOT, SNOWFLAKE, ICE_SHIELD
};

class EntityMesh: public Entity {

public:

	// Attributes of the derived class  
	Mesh* mesh = nullptr;
	Texture* texture = nullptr;
	Shader* shader = nullptr;
	Vector4 color;

	Type_OBJ Type = NONE;

	// Methods overwritten from base class
	void Init(Mesh* mesh, Texture* texture, Shader* shader);
	void render();
	void update(float elapsed_time);
};

//class Ice_Shoot : public EntityMesh {
//public:
//	void Init(Shader* shader);
//	int damage;
//	int speed;
//	void update(float elapsed_time);
//};

//class Pengu : public EntityMesh {
//public:
//	void Init(Shader* shader);
//	int Hp;
//	float shoot_ColdDown;
//	float next_Shoot_Time;
//	void update(float elapsed_time);
//	bool can_Shoot = true;
//	void shoot();
//	std::vector<Ice_Shoot*> lIce_Shoots;
//	void render();
//};


//class Eskimo : public EntityMesh {
//public:
//
//	float yaw = 0.f;
//	float pitch = 0.f;
//	int speed;
//
//
//
//	void Init(Shader* shader);
//	void move(Vector3 movement, float seconds_elapsed);
//	void update(float elapsed_time);
//};

//class Bear : public EntityMesh {
//public:
//	void Init(Shader* shader);
//};

//class Sky : public EntityMesh {
//public:
//	void Init(Texture* texture, Shader* shader);
//};

//class Ground : public EntityMesh {
//public:
//	void Init(Shader* shader);
//};

//class Block: public EntityMesh {
//public:
//	void Init(Shader* shader);
//};

