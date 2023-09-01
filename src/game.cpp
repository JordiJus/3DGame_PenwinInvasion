#include "game.h"
#include "utils.h"
#include "mesh.h"
#include "texture.h"
#include "fbo.h"
#include "shader.h"
#include "input.h"
#include "animation.h"

#include <cmath>
#include "Entity.h"
#include "EntityMesh.h"
#include "Ice_Shoot.h"
#include "Ice_Shield.h"
#include "Pengu.h"
#include "Eskimo.h"
#include "Bear.h"
#include "Ground.h"
#include "Sky.h"
#include "Block.h"
#include "snowflake.h"

#include "world.h"
#include "parseScene.cpp"
#include "stage.h"



//some globals
Mesh* mesh = NULL;

Animation* anim = NULL;
float mouse_speed = 100.0f;
FBO* fbo = NULL;

Game* Game::instance = NULL;

Stage* current_stage;
Menu* menu_stage = new Menu();
Field* field_stage = new Field();
Death* death_stage = new Death();




Game::Game(int window_width, int window_height, SDL_Window* window)
{
	this->window_width = window_width;
	this->window_height = window_height;
	this->window = window;
	instance = this;
	must_exit = false;

	fps = 0;
	frame = 0;
	time = 0.0f;
	elapsed_time = 0.0f;
	mouse_locked = false;

	//OpenGL flags
	glEnable( GL_CULL_FACE ); //render both sides of every triangle
	glEnable( GL_DEPTH_TEST ); //check the occlusions using the Z buffer

	//create our camera
	camera = new Camera();
	camera->lookAt(Vector3(0.f,100.f, 100.f),Vector3(0.f,0.f,0.f), Vector3(0.f,1.f,0.f)); //position the camera and point to 0,0,0
	camera->setPerspective(70.f,window_width/(float)window_height,0.1f,10000.f); //set the projection, we want to be perspective

	_world = new World();

	std::cout << audio->Init() << std::endl;

	channel = audio->play("data/music/plantsVsZombies.wav", 1, true);

	menu_stage->init();
	field_stage->init();
	death_stage->init();
	current_stage = menu_stage;
	current_stage->whatStage = 1;
	//hide the cursor
	SDL_ShowCursor(mouse_locked); //hide or show the mouse
}

//what to do when the image has to be draw
void Game::render(void)
{
	current_stage->render();

	//swap between front buffer and back buffer
	SDL_GL_SwapWindow(this->window);
}

void Game::update(double seconds_elapsed)
{
	current_stage->update(seconds_elapsed);

	if (current_stage->isFinished == true) {
		if (current_stage->whatStage == 1) {
			SDL_ShowCursor(false);
			current_stage = field_stage;
			menu_stage->restart();
		} else if (current_stage->whatStage == 2) {
			SDL_ShowCursor(true);
			current_stage = death_stage;
			std::cout << field_stage->game_Points << std::endl;
			field_stage->restart();
			field_stage->init();
		}
		else if (current_stage->whatStage == 3) {
			SDL_ShowCursor(true);
			current_stage = menu_stage;
			death_stage->restart();
		}
	}

	//to navigate with the mouse fixed in the middle
	
	
}

//Keyboard event handler (sync input)
void Game::onKeyDown( SDL_KeyboardEvent event )
{
	switch(event.keysym.sym)
	{
		case SDLK_ESCAPE: must_exit = true; break; //ESC key, kill the app
		case SDLK_F1: Shader::ReloadAll(); break; 
		case SDLK_0: current_stage->isFinished = true;
	}
}

void Game::onKeyUp(SDL_KeyboardEvent event)
{
}

void Game::onGamepadButtonDown(SDL_JoyButtonEvent event)
{

}

void Game::onGamepadButtonUp(SDL_JoyButtonEvent event)
{

}

void Game::onMouseButtonDown( SDL_MouseButtonEvent event )
{
	current_stage->MouseButDown(event);
}

void Game::onMouseButtonUp(SDL_MouseButtonEvent event)
{
}

void Game::onMouseWheel(SDL_MouseWheelEvent event)
{
	mouse_speed *= event.y > 0 ? 1.1 : 0.9;
}

void Game::onResize(int width, int height)
{
    std::cout << "window resized: " << width << "," << height << std::endl;
	glViewport( 0,0, width, height );
	camera->aspect =  width / (float)height;
	window_width = width;
	window_height = height;
}

