#pragma once
#include <vector>
#include "world.h"
#include "includes.h"
#include "game.h"


class Stage {
public:
	Stage();
	virtual ~Stage();

	bool canPress;
	bool isFinished;
	int whatStage;
	int nextStage;

	


	// Methods that should be overwritten
	// by derived classes 
	virtual void init() {};
	virtual void restart() {};
	virtual void render() {};
	virtual void update(float elapsed_time) {};
	bool addButton(float center_x, float center_y, float w, float h, Shader* quad_shader, Texture* texture, bool canPress);
	virtual void MouseButDown(SDL_MouseButtonEvent event) {};
	

	// Pointer to parent
	Stage* parent;

	// Pointers to children
	std::vector<Stage*> children;
};

class Menu : public Stage {
public:
	Menu() {};

	Shader* quad_shader;
	Texture* startBut;
	Texture* quitBut;
	Texture* titleBut;



	void init();
	void restart();
	void render();
	void update();
	void MouseButDown(SDL_MouseButtonEvent event);
};

class Field : public Stage {
public:
	Field() {};
	World* _world;
	Camera* camera;
	std::vector<Pengu*> pengus;
	Eskimo* eskimo;
	std::vector<Bear*> Bears;

	Shader* shaderPhong = NULL;
	Shader* shaderPhongVC = NULL;
	Shader* shaderBasic = NULL;
	Shader* shaderTexture = NULL;
	Shader* shaderSkinning = NULL;
	Texture* textureSky = NULL;
	bool can_SpawnBear = true;
	bool spawn_Bear = false;
	float next_spawn_Bear_Time;
	float spawn_Bear_ColdDown = 10000;
	float spawn_BearRaid_ColdDown = 1500;
	//bool picked_SnowFlake = false;
	bool spawn_SnowFlake = false;
	float next_spawn_SnowFlake_Time;
	float spawn_SnowFlake_ColdDown = 6000;
	int game_Points = 0;
	int num_Bearkilled = 0;
	int num_StartBearRaid = 10;
	int num_BearRaid = 0;
	int num_BearRaid_Limit = 10;

	float angle = 0;

	Ground* ground;
	Sky* sky = new Sky();

	void init();
	void restart();
	void render();
	void update(float seconds_elapsed);
	void MouseButDown(SDL_MouseButtonEvent event);
};

class Death : public Stage {
public:
	Death() {};

	Texture* gameOver;
	Texture* retryBut;

	Shader* quad_shader;

	void init();
	void restart();
	void render();
	void update(float seconds_elapsed);
	void MouseButDown(SDL_MouseButtonEvent event);
};
