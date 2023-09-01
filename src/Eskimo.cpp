#include "Eskimo.h"
#include "game.h"

Animation* current_animation;
float blended;

void Eskimo::Init(Shader* shader)
{
	mesh = Mesh::Get("data/seres/eskimo.MESH");
	texture = Texture::Get("data/seres/eskimo.tga");
	this->shader = shader;
	this->Type = ESKIMO;
	this->speed = 8;
	snowFlake_Value = 100;
	picked_SnowFlake = false;
	Hp = 200;
	objType_On_Hand = NONE;

	
	animA = Animation::Get("data/seres/idle.skanim");
	animB = Animation::Get("data/seres/running.skanim");

	current_animation = animA;

	
}

void Eskimo::update(float seconds_elapsed) {

	Matrix44 myaw;
	myaw.setRotation(yaw, Vector3(0.f, 1.f, 0.f));
	Vector3 camera_front = myaw.frontVector();
	Vector3 camera_right = myaw.rightVector();
	Vector3 move_dir;

	Vector3 position = model.getTranslation();

	if (Input::isKeyPressed(SDL_SCANCODE_W) || Input::isKeyPressed(SDL_SCANCODE_UP)) move_dir = move_dir + camera_front;
	if (Input::isKeyPressed(SDL_SCANCODE_S) || Input::isKeyPressed(SDL_SCANCODE_DOWN))  move_dir = move_dir - camera_front;
	if (Input::isKeyPressed(SDL_SCANCODE_A) || Input::isKeyPressed(SDL_SCANCODE_LEFT))  move_dir = move_dir + camera_right;
	if (Input::isKeyPressed(SDL_SCANCODE_D) || Input::isKeyPressed(SDL_SCANCODE_RIGHT))  move_dir = move_dir - camera_right;

	if (move_dir.length() > 0 && blended < 1) {
		blended += 3 * seconds_elapsed;
	}
	else if (move_dir.length() <= 0 && blended > 0) {
		blended -= 1 * seconds_elapsed;
	}
	else if (blended >= 1) {
		blended = 1;
		current_animation = animB;
	}
	else {
		blended = 0;
		current_animation = animA;
	}

	if (Input::isKeyPressed(SDL_SCANCODE_1)){
		if (!obj_On_Hand) {
			if (snowFlake_Value >= Game::instance->_world->Pengu_Price)
			{
				Set_SnowFlake_Value(-Game::instance->_world->Pengu_Price);
				obj_On_Hand = true;
				objType_On_Hand = PENGU;
			}
		}
	}
	if (Input::isKeyPressed(SDL_SCANCODE_2)) {
		if (!obj_On_Hand) {
			if (snowFlake_Value >= Game::instance->_world->IceShield_Price)
			{
				Set_SnowFlake_Value(-Game::instance->_world->IceShield_Price);
				obj_On_Hand = true;
				objType_On_Hand = ICE_SHIELD;
			}
		}
	}



	/*if (SDL_BUTTON_LEFT) {
		place_Pengu();
	}*/

	move_dir *= speed;

	position = position + (move_dir * seconds_elapsed);

	if (position.x < -2)
		position.x = -2;
	if (position.x > 26)
		position.x = 26;
	if (position.z < -2)
		position.z = -2;
	if (position.z > 50)
		position.z = 50;
	model.setTranslation(position.x, position.y, position.z);
	model.rotate(yaw, Vector3(0, 1, 0));

	if (obj_On_Hand) {

		Vector3 v = model.getTranslation();
		Vector3 front = model.frontVector();
		switch (objType_On_Hand)
		{
		case NONE:
			break;
		case PENGU:
			v = v + front * 2;
			pengu_Placeable->model.setTranslation(v.x, v.y+4, v.z);
			pengu_Placeable->model.scale(5, 5, 5);
			pengu_Placeable->model.rotate(yaw, Vector3(0, 1, 0));
			break;
		case ICE_SHIELD:
			v  = v + front * 2;
			iceShield_Placeable->model.setTranslation(v.x, v.y, v.z);
			iceShield_Placeable->model.rotate(yaw, Vector3(0, 1, 0));
			break;
		default:
			break;
		}
		
	}

	for (int i = 0; i < lPengus.size(); i++)
		lPengus[i]->update(seconds_elapsed);

	current_animation->assignTime(Game::instance->time);
	
}

void Eskimo::render()
{// Get the last camera that was activated 
	Camera* camera = Camera::current;

	// Enable shader and pass uniforms 
	shader->enable();
	shader->setUniform("u_color", Vector4(1, 1, 1, 1));
	shader->setUniform("u_time", time);
	shader->setUniform("u_viewprojection", camera->viewprojection_matrix);

	if (!isInstanced) {
		shader->setUniform("u_model", model);
	}
	else {

	}

	//shader->setUniform("u_Ka", Vector3(1.f, 1.f, 1.f));
	//shader->setUniform("u_Kd", Vector3(0.86, 0.65, 0.7));
	//shader->setUniform("u_Ks", Vector3(1.f, 1.f, 1.f));

	if (texture != nullptr) {
		shader->setTexture("u_texture", texture, 0);
	}

	
	

	// Create temporary skeleton to store blended pose
	Skeleton blended_skeleton;

	// Blend animations A and B using weight 0.5 and store
	// it in “blended_skeleton”
	blendSkeleton(&animA->skeleton,
		&animB->skeleton,
		blended, &blended_skeleton);

	// Get bone matrix to manipulate it (reference!)
	//Matrix44& head = blended_skeleton.getBoneMatrix(
	//	"mixamorig_Head");
	// Transform it
	//head.scale(2, 2, 2);

	// Render the mesh using the shader
	/*
	if (!isInstanced) {
		mesh->renderAnimated(GL_TRIANGLES, &blended_skeleton);
	}
	else {
		mesh->renderAnimated(GL_TRIANGLES, &blended_skeleton);
		// mesh->renderInstanced(GL_TRIANGLES, models.data(), models.size());
	}
	*/
	
	if (blended != 1 && blended != 0) {
		mesh->renderAnimated(GL_TRIANGLES, &blended_skeleton);
	}
	else {
		mesh->renderAnimated(GL_TRIANGLES, &current_animation->skeleton);
	}
	

	// Disable shader after finishing rendering
	shader->disable();

	if (obj_On_Hand) {
		switch (objType_On_Hand)
		{
		case NONE:
			break;
		case PENGU:
			pengu_Placeable->render();
			break;
		case ICE_SHIELD:
			iceShield_Placeable->render();
			break;
		default:
			break;
		}
	}

	for (int i = 0; i < lPengus.size(); i++)
		lPengus[i]->render();
	for (int i = 0; i < lIce_Shields.size(); i++)
		lIce_Shields[i]->render();
}

void Eskimo::place_Pengu()
{
	Block* b = nullptr;
	b = Get_Placeable_Block();
	if (b != nullptr) {
		b->obj_Placed = true;
		obj_On_Hand = false;
		objType_On_Hand = NONE;
		Pengu* pengu = new Pengu();
		pengu->Init(pengu_Placeable->shader);
		pengu->row = b->row;
		pengu->column = b->column;
		Vector3 v = b->model.getTranslation();
		pengu->model.setTranslation(v.x, v.y + 1.45, v.z);
		pengu->model.scale(5, 5, 5);
		pengu->shader_Shoot = iceShield_Placeable->shader; // We use the shader of the iceShield because it uses the same shader
		lPengus.push_back(pengu);
	}

}

void Eskimo::place_IceShield()
{
	Block* b = nullptr;
	b = Get_Placeable_Block();
	if (b != nullptr) {
		b->obj_Placed = true;
		obj_On_Hand = false;
		objType_On_Hand = NONE;
		Ice_Shield* is = new Ice_Shield();
		is->Init(iceShield_Placeable->shader);
		is->row = b->row;
		is->column = b->column;
		Vector3 v = b->model.getTranslation();
		is->model.setTranslation(v.x, v.y, v.z);
		lIce_Shields.push_back(is);
	}
}

void Eskimo::Init_Placeable_Pengu(Shader* shader)
{
	pengu_Placeable = new Pengu();
	pengu_Placeable->Init(shader);
	pengu_Placeable->model.scale(5, 5, 5);
}

void Eskimo::Init_Placeable_IceShield(Shader* shader)
{
	iceShield_Placeable = new Ice_Shield();
	iceShield_Placeable->Init(shader);
}

void Eskimo::Find_Pengus_Target(std::vector<Bear*> bears)
{
	for (int i = 0; i < lPengus.size(); i++) {
		bool find_Target = false;
		for (int j = 0; j < bears.size(); j++) {
			if (lPengus[i]->row == bears[j]->row)
			{
				lPengus[i]->has_Target = true;
				find_Target = true;
				break;
			}
		}
		if(!find_Target)
			lPengus[i]->has_Target = false;
	}
}

Block* Eskimo::Get_Placeable_Block()
{
	Block* res = nullptr;

	for (int i = 0; i < lBlocks.size(); i++) {
		Block* b = lBlocks[i];
		if (model.getTranslation().distance(b->model.getTranslation()) < 3 && !b->obj_Placed) {
			res = b;
			break;
		}
	}
	
	return res;
}

void Eskimo::Set_SnowFlake_Value(int value)
{
	this->snowFlake_Value += value;
	if (this->snowFlake_Value > Max_snowFlake_Value)
		this->snowFlake_Value = Max_snowFlake_Value;
	if (this->snowFlake_Value < 0)
		this->snowFlake_Value = 0;
	//std::cout << "snowFlake_Value:" + this->snowFlake_Value << std::endl;
}

void Eskimo::getDamage(int damage)
{
	this->Hp -= damage;
	if (this->Hp < 0) {
		Hp = 0;
	}
}

void Eskimo::move(Vector3 movement, float seconds_elapsed) {

}


