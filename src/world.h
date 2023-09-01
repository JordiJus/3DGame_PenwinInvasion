#pragma once
#include "includes.h"
#include "framework.h"
#include "Entity.h"
#include "mesh.h"
#include "texture.h"
#include "shader.h"
#include <string>


#include "utils.h"
#include "EntityMesh.h"
#include "Ice_Shoot.h"
#include "Pengu.h"
#include "Eskimo.h"
#include "Bear.h"
#include "Ground.h"
#include "Sky.h"
#include "Block.h"
#include "snowflake.h"
#include <fstream>
#include <map>
#include <typeinfo>

#include <iostream>
#include <cstdlib> 
#include <ctime>

class World
{
public:
	World(); 			// Constructor
	virtual ~World(); 	// Destructor

	// Some attributes 
	std::string name;
	Matrix44 model;

	Mesh* mesh = nullptr;
	Texture* texture = nullptr;
	Shader* shader = nullptr;
	Vector4 color;

	float Dead_Line = -4;
	int Pengu_Price = 100;
	int IceShield_Price = 50;
	int Draw_Distance = 5000;
	int Row;
	int Column;
	int Life = 50;

	// Methods that should be overwritten
	// by derived classes 
	virtual void render();
	virtual void update(float elapsed_time);

	// Pointers to children
	std::vector<Entity*> children;

	void addChild(Entity* child);
	void removeChild(Entity* child);

	// Get transform in world coordinates
	Matrix44 getGlobalMatrix();


	// To parse scene
	bool parseScene(const char* filename);

	struct sRenderData {
		Texture* texture = nullptr;
		Shader* shader = Shader::Get("data/shaders/basic.vs", "data/shaders/phong.fs");
		std::vector<Matrix44> models;
	};

	Entity root;
	std::map<std::string, sRenderData> meshes_to_load;

	std::vector<Block*> Get_Grids();
	Block* Get_Grid(int row, int col);
	Eskimo* Get_Eskimo();
	std::vector<Bear*> Get_Bears();
	//std::vector<Pengu*> Get_Pengus();

	void Spawn_Bear(Shader* shader);
	void Spawn_SnowFlake(Shader* shader);
	void Remove_SnowFlake();
	//void Bear_Raid(Shader* shader);
};

