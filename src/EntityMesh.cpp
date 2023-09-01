#include "EntityMesh.h"

#include "game.h"

void EntityMesh::Init(Mesh* mesh, Texture* texture, Shader* shader) {
	this->mesh = mesh;
	this->texture = texture;
	this->shader = shader;
}


void EntityMesh::render()
{
	// Get the last camera that was activated 
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

	// Render the mesh using the shader
	if (!isInstanced) {
		mesh->render(GL_TRIANGLES);
	}
	else {
		mesh->renderInstanced(GL_TRIANGLES, models.data(), models.size());
	}

	// Disable shader after finishing rendering
	shader->disable();
};


void EntityMesh::update(float elapsed_time)
{
}

#pragma region Ice_Shoot

//void Ice_Shoot::Init(Shader* shader)
//{
//	mesh = Mesh::Get("data/seres/ice.obj");
//	this->shader = shader;
//	this->Type = ICE_SHOOT;
//	int damage = 20;
//	int speed = 10;
//}
//
//void Ice_Shoot::update(float elapsed_time)
//{
//	Vector3 position = model.getTranslation();
//
//	position = position + (Vector3(0.f, 0.f, 1.f) * elapsed_time * 10);
//
//	model.setTranslation(position.x, position.y, position.z);
//
//	model.rotate(M_PI / 2, Vector3(0.0f, -1.0f, 0.0f));
//}
#pragma endregion

#pragma region  Pengu

//void Pengu::Init(Shader* shader)
//{
//	mesh = Mesh::Get("data/seres/pengu.obj");
//	this->shader = shader;
//	this->Type = PENGU;
//	this->Hp = 100;
//	this->shoot_ColdDown = 1200;
//	can_Shoot = true;
//}
//
//void Pengu::update(float elapsed_time)
//{
//	long now = SDL_GetTicks();
//	if (can_Shoot) {
//		shoot();
//		can_Shoot = false;
//		next_Shoot_Time = now + shoot_ColdDown;
//	}
//	else {
//		if (now >= next_Shoot_Time) {
//			can_Shoot = true;
//		}
//	}
//
//	for (int i = 0; i < lIce_Shoots.size(); i++)
//		lIce_Shoots[i]->update(elapsed_time);
//}
//
//void Pengu::shoot()
//{
//	/*long now = SDL_GetTicks();
//	std::cout << "Pengu: Shoot!!! Time:" << now << std::endl;*/
//	Ice_Shoot* is = new Ice_Shoot();
//	is->Init(this->shader);
//	
//	Vector3 v = model.getTranslation();
//	is->model.setTranslation(v.x, v.y + 0.6, v.z + 0.6);
//
//	lIce_Shoots.push_back(is);
//	//world->children.push_back(is);
//}
//
//void Pengu::render()
//{// Get the last camera that was activated 
//	Camera* camera = Camera::current;
//
//	// Enable shader and pass uniforms 
//	shader->enable();
//	shader->setUniform("u_color", Vector4(1, 1, 1, 1));
//	shader->setUniform("u_time", time);
//	shader->setUniform("u_viewprojection", camera->viewprojection_matrix);
//
//	if (!isInstanced) {
//		shader->setUniform("u_model", model);
//	}
//	else {
//
//	}
//
//	//shader->setUniform("u_Ka", Vector3(1.f, 1.f, 1.f));
//	//shader->setUniform("u_Kd", Vector3(0.86, 0.65, 0.7));
//	//shader->setUniform("u_Ks", Vector3(1.f, 1.f, 1.f));
//
//	if (texture != nullptr) {
//		shader->setTexture("u_texture", texture, 0);
//	}
//
//	// Render the mesh using the shader
//	if (!isInstanced) {
//		mesh->render(GL_TRIANGLES);
//	}
//	else {
//		mesh->renderInstanced(GL_TRIANGLES, models.data(), models.size());
//	}
//
//	// Disable shader after finishing rendering
//	shader->disable();
//
//	for (int i = 0; i < lIce_Shoots.size(); i++)
//		lIce_Shoots[i]->render();
//}

#pragma endregion

#pragma region Eskimo
//
//void Eskimo::Init(Shader* shader)
//{
//	mesh = Mesh::Get("data/seres/eskimo.obj");
//	this->shader = shader;
//	this->Type = ESKIMO;
//	this->speed = 15;
//
//}
//
//void Eskimo::update(float seconds_elapsed) {
//
//	Matrix44 myaw;
//	myaw.setRotation(yaw, Vector3(0.f, 1.f, 0.f));
//	Vector3 camera_front = myaw.frontVector();
//	Vector3 camera_right = myaw.rightVector();
//	Vector3 move_dir;
//
//	Vector3 position = model.getTranslation();
//
//	if (Input::isKeyPressed(SDL_SCANCODE_W) || Input::isKeyPressed(SDL_SCANCODE_UP)) move_dir = move_dir + camera_front;
//	if (Input::isKeyPressed(SDL_SCANCODE_S) || Input::isKeyPressed(SDL_SCANCODE_DOWN))  move_dir = move_dir - camera_front;
//	if (Input::isKeyPressed(SDL_SCANCODE_A) || Input::isKeyPressed(SDL_SCANCODE_LEFT))  move_dir = move_dir + camera_right;
//	if (Input::isKeyPressed(SDL_SCANCODE_D) || Input::isKeyPressed(SDL_SCANCODE_RIGHT))  move_dir = move_dir - camera_right;
//
//	move_dir *= speed;
//
//	position = position + (move_dir * seconds_elapsed);
//
//	model.setTranslation(position.x, position.y, position.z);
//	model.rotate(yaw, Vector3(0, 1, 0));
//
//}
//
//void Eskimo::move(Vector3 movement, float seconds_elapsed) {
//
//}

#pragma endregion

#pragma region Bear
//void Bear::Init(Shader* shader)
//{
//	mesh = Mesh::Get("data/seres/bear.obj");
//	this->shader = shader;
//	this->Type = BEAR;
//}
#pragma endregion


#pragma region Ground
//void Ground::Init(Shader* shader)
//{
//	mesh = Mesh::Get("data/terrain/terrain_low.ASE");
//	texture = Texture::Get("data/terrain/terrain.tga");
//	this->shader = shader;
//	this->Draw_requied = true;
//	this->Type = FLOOR;
//}
#pragma endregion



#pragma region Sky

//void Sky::Init(Texture* texture, Shader* shader) {
//	mesh = Mesh::Get("data/terrain/cielo.ASE");
//	this->shader = shader;
//	this->texture = texture;
//	this->Draw_requied = true;
//	this->Type = SKY;
//}
#pragma endregion

#pragma region Block

//void Block::Init(Shader* shader) {
//	mesh = Mesh::Get("data/grid.obj");
//	this->shader = shader;
//	this->texture = texture;
//	this->Draw_requied = true;
//	this->Type = GRID;
//}
#pragma endregion


