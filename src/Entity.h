#pragma once
#include "framework.h"
#include <string>
#include "camera.h"
class Entity
{
public:
	Entity(); 			// Constructor
	virtual ~Entity(); 	// Destructor

	// Some attributes 
	std::string name;
	Matrix44 model;
	
	std::vector<Matrix44> models;
	bool isInstanced = false;

	bool Draw_requied = false;

	// Methods that should be overwritten
	// by derived classes 
	virtual void render();
	virtual void update(float elapsed_time);

	// Pointer to parent
	Entity* parent;

	// Pointers to children
	std::vector<Entity*> children;

	void addChild(Entity* child);
	void removeChild(Entity* child);

	// Get transform in world coordinates
	Matrix44 getGlobalMatrix();

};

class Material {
public:
	Vector3 Ka;
	Vector3 Kd;
	Vector3 Ks;

	Material(Vector3 Ka, Vector3 Kd, Vector3 Ks) {
		this->Ka = Ka;
		this->Kd = Kd;
		this->Ks = Ks;
	}
};

class Light {
public:
	Vector3 Ia;
	Vector3 Id;
	Vector3 Is;


	Light(Vector3 Ia, Vector3 Id, Vector3 Is) {
		this->Ia = Ia;
		this->Id = Id;
		this->Is = Is;
	}
};
