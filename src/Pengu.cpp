#include "Pengu.h"

void Pengu::Init(Shader* shader)
{
	mesh = Mesh::Get("data/seres/pengu.obj");
	this->shader = shader;
	this->Type = PENGU;
	this->Hp = 100;
	this->shoot_ColdDown = 3000;
	can_Shoot = true;
}

void Pengu::update(float elapsed_time)
{
	if (has_Target) {
		long now = SDL_GetTicks();
		if (can_Shoot) {
			shoot();
			can_Shoot = false;
			next_Shoot_Time = now + shoot_ColdDown;
		}
		else {
			if (now >= next_Shoot_Time) {
				can_Shoot = true;
			}
		}
	}

	for (int i = 0; i < lIce_Shoots.size(); i++) {
		if(lIce_Shoots[i]->hitted)
		{
			lIce_Shoots.erase(std::remove(lIce_Shoots.begin(), lIce_Shoots.end(), lIce_Shoots[i]), lIce_Shoots.end());
		}else
			lIce_Shoots[i]->update(elapsed_time);
	}
}

void Pengu::shoot()
{
	/*long now = SDL_GetTicks();
	std::cout << "Pengu: Shoot!!! Time:" << now << std::endl;*/
	Ice_Shoot* is = new Ice_Shoot();
	is->Init(this->shader_Shoot);//or shader_Shoot
	
	Vector3 v = model.getTranslation();
	is->model.setTranslation(v.x, v.y + 0.6, v.z + 0.6);

	lIce_Shoots.push_back(is);

	//channel = iceShoot->play("data/music/plantsVsZombies.wav", 1, false);
	//world->children.push_back(is);
}

void Pengu::render()
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

	// Render the mesh using the shader
	if (!isInstanced) {
		mesh->render(GL_TRIANGLES);
	}
	else {
		mesh->renderInstanced(GL_TRIANGLES, models.data(), models.size());
	}

	// Disable shader after finishing rendering
	shader->disable();

	for (int i = 0; i < lIce_Shoots.size(); i++)
		lIce_Shoots[i]->render();
}

void Pengu::getDamage(int damage)
{
	this->Hp -= damage;
	if (this->Hp < 0) {
		Hp = 0;
	}
}
