#include "stage.h"



//STAGE BASE CLASS
Stage::Stage() {

}

Stage::~Stage() {

}


bool Stage::addButton(float center_x, float center_y, float w, float h, Shader* quad_shader, Texture* texture, bool canPress) {
	Mesh quad;

	float mouse_x = Input::mouse_position.x;
	float mouse_y = Input::mouse_position.y;

	float but_cx = ((center_x / Game::instance->window_width) * 2) - 1; // window_weight = 800
	float but_cy = ((center_y / Game::instance->window_height) * 2) - 1; // window_height = 600

	float but_w = ((w / Game::instance->window_width) * 2);
	float but_h = ((h / Game::instance->window_height) * 2);

	quad.createQuad(but_cx, but_cy, but_w, but_h, false);

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	quad_shader->enable();

	if (texture != nullptr) {
		quad_shader->setTexture("u_texture", texture, 0);
	}

	//quad_shader->setUniform("u_color", Vector4(0.0, 0.0, 1.0, 1.0));

	quad.render(GL_TRIANGLES);

	quad_shader->disable();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	center_y = Game::instance->window_height - center_y;

	if (mouse_x > center_x - (w / 2) && mouse_x < center_x + (w / 2) && mouse_y > center_y - (h / 2) && mouse_y < center_y + (h / 2) && canPress == true) {
		return true;
	}
	return false;
}

//MENU STAGE
void Menu::init() {
	quad_shader = Shader::Get("data/shaders/quad.vs", "data/shaders/texture.fs");
	startBut = Texture::Get("data/start_button.tga");
	quitBut = Texture::Get("data/exit_button.tga");
	titleBut = Texture::Get("data/penwin_invasion.tga");
	Game::instance->mouse_locked = false;
	isFinished = false;

	
}

void Menu::restart() {
	Game::instance->mouse_locked = false;
	isFinished = false;
}

void Menu::render() {
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);

	if (addButton(400, 200, 150, 50, quad_shader, quitBut, canPress)) {
		Game::instance->must_exit = true;
	}
	if (addButton(400, 300, 150, 50, quad_shader, startBut, canPress)) {
		isFinished = true;
	}
	if (addButton(400, 500, 400, 100, quad_shader, titleBut, canPress)) {}
	canPress = false;

	drawText(2, 2, "Press 1 to spend 2 SnowFlakes and place a pengu", Vector3(0, 0, 0), 2);
	drawText(2, 18, "Press 2 to spend 1 SnowFlake and place an ice shield", Vector3(0, 0, 0), 2);
}

void Menu::update() {
	
}

void Menu::MouseButDown(SDL_MouseButtonEvent event) {
	if (event.button == SDL_BUTTON_LEFT) //middle mouse
	{
		canPress = true;
	}
}

//GAME STAGE
void Field::init() {
	this->_world = Game::instance->_world;
	Game::instance->mouse_locked = true;
	_world->Life = 50;
	whatStage = 2;
	isFinished = false;

	camera = new Camera();
	camera->lookAt(Vector3(0.f, 100.f, 100.f), Vector3(0.f, 0.f, 0.f), Vector3(0.f, 1.f, 0.f)); //position the camera and point to 0,0,0
	camera->setPerspective(70.f, Game::instance->window_width / (float)Game::instance->window_height, 0.1f, 10000.f); //set the projection, we want to be perspective

	shaderPhong = Shader::Get("data/shaders/basic.vs", "data/shaders/phong.fs");
	shaderPhongVC = Shader::Get("data/shaders/basic.vs", "data/shaders/phongVC.fs");
	shaderBasic = Shader::Get("data/shaders/basic.vs", "data/shaders/basic_color.fs");
	shaderTexture = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
	shaderSkinning = Shader::Get("data/shaders/skinning.vs", "data/shaders/texture.fs");

	textureSky = new Texture();
	textureSky->load("data/terrain/cielo.tga");




	sky = new Sky();

	sky->Init(textureSky, shaderTexture);
	_world->children.push_back(sky);


	for (int i = 0; i < _world->Column; ++i) {
		for (int j = 0; j < _world->Row; ++j) {
			Block* b = new Block();
			b->Init(shaderPhong);
			b->model.setTranslation((j * 6), 0, (i * 6));
			b->row = j;
			b->column = i;
			if (b->column == 0)
				b->mesh = Mesh::Get("data/grid_blue.obj");
			_world->children.push_back(b);
		}
	}

	

	eskimo = new Eskimo();
	eskimo->Init(shaderSkinning);
	eskimo->model.setTranslation(12, 0, _world->Dead_Line);
	eskimo->model.scale(5,5,5);
	eskimo->lBlocks = _world->Get_Grids();
	eskimo->Init_Placeable_Pengu(shaderPhongVC);
	eskimo->Init_Placeable_IceShield(shaderPhong);
	

	_world->children.push_back(eskimo);

	ground = new Ground();
	ground->Init(shaderPhong);
	ground->model.scale(20, 20, 20);
	_world->children.push_back(ground);

	//_world->parseScene("data/myscene.scene");

	//render sky

	long now = SDL_GetTicks();

	next_spawn_Bear_Time = now + spawn_Bear_ColdDown;
	next_spawn_SnowFlake_Time = now + spawn_SnowFlake_ColdDown;

	_world->Spawn_SnowFlake(shaderPhong);

	// render floot
	Mesh* floorplane = new Mesh();
	floorplane->createPlane(10000.f);
	EntityMesh* floor = new EntityMesh();
	floor->mesh = floorplane;
	//floor->texture = null;
	floor->shader = shaderBasic;
	floor->Type = FLOOR;
}

void Field::restart() {
	Game::instance->mouse_locked = true;
	_world->Life = 50;
	whatStage = 2;
	isFinished = false;
	eskimo->Hp = 200;
	eskimo->model.setTranslation(12, 0, _world->Dead_Line);
	_world->children.clear();
	game_Points = 0;
	
	long now = SDL_GetTicks();

	spawn_Bear_ColdDown = 10000;
	next_spawn_Bear_Time = now + spawn_Bear_ColdDown;
	next_spawn_SnowFlake_Time = now + spawn_SnowFlake_ColdDown;


}

void Field::render() {

	

	//set the clear color (the background color)
	glClearColor(1.0, 1.0, 1.0, 1.0);

	// Clear the window and the depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//set the camera as default
	camera->enable();

	Vector3 eye;
	Vector3 center;

	eskimo->yaw -= Input::mouse_delta.x * Game::instance->elapsed_time * 10.f * DEG2RAD;
	eskimo->pitch -= Input::mouse_delta.y * Game::instance->elapsed_time * 10.f * DEG2RAD;


	eskimo->pitch = clamp(eskimo->pitch, PI * 0.1f , PI * 0.40f);



	Matrix44 myaw;
	Matrix44 mpitch;
	myaw.setRotation(eskimo->yaw, Vector3(0.f, 1.f, 0.f));
	mpitch.setRotation(eskimo->pitch, Vector3(-1.f, 0.f, 0.f));


	Vector3 front = (mpitch * myaw).frontVector();
	eye = eskimo->model.getTranslation() - front * 10.0f;
	center = eskimo->getGlobalMatrix() * Vector3(0.f, 2.f, 0.5f);

	Vector3 col_point;
	Vector3 col_normal;

	camera->lookAt(eye, center, Vector3(0, 1, 0));
	
	


	// Render sky
	glDisable(GL_DEPTH_TEST);

	sky->render();

	glEnable(GL_DEPTH_TEST);


	//ground->render();

	if (shaderPhong)
	{
		//enable shader
		shaderPhong->enable();

		//upload uniforms
		shaderPhong->setUniform("u_camera_pos", camera->eye);
		shaderPhong->setUniform("u_light_pos", Vector3(10, 0, 10));

		//do the draw call

		// Using entities container


		//disable shader
		shaderPhong->disable();
	}



	

	_world->render();

	Game::instance->mouse_locked = true;

	//Draw the floor grid
	drawGrid();

	//render the FPS, Draw Calls, etc
	drawText(2, 2, "Snowflakes: " + std::to_string(eskimo->snowFlake_Value / 50), Vector3(1, 1, 1), 2);
	drawText(2, 18, "Score: " + std::to_string(game_Points), Vector3(1, 1, 1), 2);
}

void Field::update(float seconds_elapsed) {
	float speed = 20 * seconds_elapsed; //the speed is defined by the seconds_elapsed so it goes constant

	//example
	angle += (float)seconds_elapsed * 10.0f;



	sky->model.setTranslation(camera->center.x, camera->center.y, camera->center.z);

	if (Input::isKeyPressed(SDL_SCANCODE_LSHIFT)) speed *= 2; //move faster with left shift

	eskimo->update(seconds_elapsed);


	std::vector<Bear*> bearVector = _world->Get_Bears();

	for (int i = bearVector.size() - 1; i >= 0; i--) {
		if (bearVector[i]->Hp <= 0) {
			_world->children.erase(std::remove(_world->children.begin(), _world->children.end(), bearVector[i]), _world->children.end());
			game_Points += bearVector[i]->points;
			num_Bearkilled += 1;
		}
	}

	for (int i = eskimo->lPengus.size() - 1; i >= 0; i--) {
		if (eskimo->lPengus[i]->Hp <= 0) {
			Block* block = _world->Get_Grid(eskimo->lPengus[i]->row, eskimo->lPengus[i]->column);
			if (block != nullptr)
				block->obj_Placed = false;
			eskimo->lPengus.erase(std::remove(eskimo->lPengus.begin(), eskimo->lPengus.end(), eskimo->lPengus[i]), eskimo->lPengus.end());
		}
	}

	for (int i = eskimo->lIce_Shields.size() - 1; i >= 0; i--) {
		if (eskimo->lIce_Shields[i]->Hp <= 0) {
			Block* block = _world->Get_Grid(eskimo->lIce_Shields[i]->row, eskimo->lIce_Shields[i]->column);
			if (block != nullptr)
				block->obj_Placed = false;
			eskimo->lIce_Shields.erase(std::remove(eskimo->lIce_Shields.begin(), eskimo->lIce_Shields.end(), eskimo->lIce_Shields[i]), eskimo->lIce_Shields.end());
		}
	}

	if (eskimo->Hp <= 0) {
		isFinished = true;
	}

	_world->update(seconds_elapsed);




#pragma region Spawn Bears
	long now = SDL_GetTicks();
	if (can_SpawnBear) {

		if (spawn_Bear) {
			_world->Spawn_Bear(shaderPhong);
			spawn_Bear = false;
			if (num_Bearkilled >= num_StartBearRaid)
			{
				num_StartBearRaid += 10;
				spawn_Bear_ColdDown /= 2;
				next_spawn_Bear_Time = now + spawn_Bear_ColdDown;
			}
			else
				next_spawn_Bear_Time = now + spawn_Bear_ColdDown;
		}
		else {
			if (now >= next_spawn_Bear_Time) {
				spawn_Bear = true;
			}
		}
	}
#pragma endregion

#pragma region Spawn Snow Flake

	if (spawn_SnowFlake) {
		spawn_SnowFlake = false;
		next_spawn_SnowFlake_Time = now + spawn_SnowFlake_ColdDown;
		_world->Spawn_SnowFlake(shaderPhong);
	}
	else {
		if (now >= next_spawn_SnowFlake_Time) {
			if (eskimo->picked_SnowFlake) {
				eskimo->picked_SnowFlake = false;
				spawn_SnowFlake = true;
			}
			else {
				next_spawn_SnowFlake_Time = now + spawn_SnowFlake_ColdDown;
			}
		}
	}

#pragma endregion



#pragma region Pengus Shoot Bears
	if (eskimo->lPengus.size() > 0) {
		std::vector<Bear*> bears = _world->Get_Bears();
		if (bears.size() > 0)
			eskimo->Find_Pengus_Target(bears);
	}
#pragma endregion

#pragma region World life 0
	if (Game::instance->_world->Life == 0) {
		// TODO: Restart everything after game over
		//Game::instance->_world->children.empty();
		//eskimo->lPengus.empty();
		//eskimo->lBlocks.empty();
		isFinished = true;
	}
#pragma endregion
	if (Game::instance->mouse_locked)
		Input::centerMouse();

}

void Field::MouseButDown(SDL_MouseButtonEvent event) {
	if (event.button == SDL_BUTTON_RIGHT) //middle mouse
	{
		Game::instance->mouse_locked = !Game::instance->mouse_locked;
		SDL_ShowCursor(!Game::instance->mouse_locked);
	}

	if (event.button == SDL_BUTTON_LEFT) //middle mouse
	{
		if (eskimo->obj_On_Hand) {
			switch (eskimo->objType_On_Hand)
			{
			case NONE:
				break;
			case PENGU:
				eskimo->place_Pengu();
				break;
			case ICE_SHIELD:
				eskimo->place_IceShield();
				break;
			default:
				break;
			}
		}
	}
}


// DEATH STAGE

void Death::init() {
	whatStage = 3;
	quad_shader = Shader::Get("data/shaders/quad.vs", "data/shaders/texture.fs");
	gameOver = Texture::Get("data/game_over.tga");
	retryBut = Texture::Get("data/retry_button.tga");
	canPress = false;
	isFinished = false;
}

void Death::restart() {
	canPress = false;
	isFinished = false;
}

void Death::render(){
	glClearColor(1.0, 1.0, 1.0, 1.0);

	glClear(GL_COLOR_BUFFER_BIT);

	if (addButton(400, 500, 400, 100, quad_shader, gameOver, canPress)) {}
	if (addButton(400, 200, 150, 50, quad_shader, retryBut, canPress)) {
		isFinished = true;
	}
	canPress = false;

	
}

void Death::update(float seconds_elapsed) {

}

void Death::MouseButDown(SDL_MouseButtonEvent event) {
	if (event.button == SDL_BUTTON_LEFT) //middle mouse
	{
		canPress = true;
	}
}