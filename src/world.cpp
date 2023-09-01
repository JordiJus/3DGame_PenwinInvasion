#include "world.h"
#include "EntityMesh.h"

World::World() {
	Row = 5;
	Column = 9;
	//bear_Raid = false;
}

World::~World() {

}

void World::render() {
	Camera* camera = Camera::current;
	
	for (int i = 0; i < children.size(); i++) {
		Entity* child = children[i];
		if (child != nullptr) {
			if (child->Draw_requied)
				child->render();
			else {
				EntityMesh* obj = (EntityMesh*)child;
				Vector3 sphere_center = obj->model * obj->mesh->box.center;
				float sphere_radius = obj->mesh->radius;

				// Discard objects whose bounding sphere 
				// is not inside the camera frustum
				if (camera->testSphereInFrustum(sphere_center,
					sphere_radius) == false && camera->eye.distance(obj->getGlobalMatrix().getTranslation()) > Draw_Distance)
					continue;

				// Render object if inside frustum
				obj->render();
			}
		}
	}
}

void World::update(float elapsed_time) {
	for (int i = 0; i < children.size(); i++) {
		Entity* child = children[i];
		if (child != nullptr) {
			child->update(elapsed_time);
		}
		
	}
}

bool World::parseScene(const char* filename)
{
	// You could fill the map manually to add shader and texture for each mesh
	// If the mesh is not in the map, you can use the MTL file to render its colors
	// meshes_to_load["meshes/example.obj"] = { Texture::Get("texture.tga"), Shader::Get("shader.vs", "shader.fs") };

	std::cout << " + Scene loading: " << filename << "..." << std::endl;

	std::ifstream file(filename);

	if (!file.good()) {
		std::cerr << "Scene [ERROR]" << " File not found!" << std::endl;
		return false;
	}

	std::string scene_info, mesh_name, model_data;
	file >> scene_info; file >> scene_info;
	int mesh_count = 0;

	// Read file line by line and store mesh path and model info in separated variables
	while (file >> mesh_name >> model_data)
	{
		// Get all 16 matrix floats
		std::vector<std::string> tokens = tokenize(model_data, ",");

		// Fill matrix converting chars to floats
		Matrix44 model;
		for (int t = 0; t < tokens.size(); ++t) {
			model.m[t] = (float)atof(tokens[t].c_str());
		}

		// Add model to mesh list (might be instanced!)
		sRenderData& render_data = meshes_to_load[mesh_name];
		render_data.models.push_back(model);
		mesh_count++;
	}

	// Iterate through meshes loaded and create corresponding entities
	for (auto data : meshes_to_load) {

		mesh_name = "data/" + data.first;
		sRenderData& render_data = data.second;

		// No transforms, anything to do here
		if (render_data.models.empty())
			continue;

		// Create instanced entity

		
		if (render_data.models.size() > 1) {
			EntityMesh* new_entity = new EntityMesh();
			new_entity->Init(Mesh::Get(mesh_name.c_str()), render_data.texture, render_data.shader);
			// Add all instances
			new_entity->isInstanced = true;
			new_entity->models = render_data.models;
			// Add entity to scene root
			children.push_back(new_entity);
		}
		

		// Create normal entity
		//else {
			EntityMesh* new_entity = new EntityMesh();
			new_entity->Init(Mesh::Get(mesh_name.c_str()), render_data.texture, render_data.shader);
			new_entity->model = render_data.models[0];
			// Add entity to scene root
			children.push_back(new_entity);
		//}
	}

	std::cout << "Scene [OK]" << " Meshes added: " << mesh_count << std::endl;
	return true;

}

std::vector<Block*> World::Get_Grids()
{
	std::vector<Block*> blocks;

	for (int i = 0; i < this->children.size(); i++) {
		if (((EntityMesh*)children[i])->Type == GRID)
		{
			blocks.push_back((Block*)children[i]);
		}
	}

	return blocks;
}

Block* World::Get_Grid(int row, int col)
{
	Block* block = nullptr;
	std::vector<Block*> blocks = Get_Grids();
	for (int i = 0; i < blocks.size(); i++) {
		if (blocks[i]->row == row && blocks[i]->column == col) {
			block = blocks[i];
			break;
		}
	}
	return block;
}

Eskimo* World::Get_Eskimo()
{
	Eskimo* eskimo = nullptr;

	for (int i = 0; i < this->children.size(); i++) {
		if (((EntityMesh*)children[i])->Type == ESKIMO)
		{
			eskimo = (Eskimo*)children[i];
		}
	}

	return eskimo;
}

std::vector<Bear*> World::Get_Bears()
{
	std::vector<Bear*> Bears;

	for (int i = 0; i < this->children.size(); i++) {
		if (((EntityMesh*)children[i])->Type == BEAR)
		{
			Bears.push_back((Bear*)children[i]);
		}
	}

	return Bears;
}

//std::vector<Pengu*> World::Get_Pengus()
//{
//	std::vector<Pengu*> pengus;
//
//	for (int i = 0; i < this->children.size(); i++) {
//		if (((EntityMesh*)children[i])->Type == PENGU)
//		{
//			pengus.push_back((Pengu*)children[i]);
//		}
//	}
//
//	return pengus;
//}

void World::Spawn_Bear(Shader* shader)
{
	Bear* bear = new Bear();
	bear->Init(shader);
#pragma region set position

	std::vector<Block*> blocks = Get_Grids();

	std::srand(static_cast<unsigned int>(std::time(nullptr)));

	int randomNumber = std::rand() % 5;

	Block* block = nullptr;

	for (int i = 0; i < blocks.size(); i++) {
		Block* b = blocks[i];
		if (b->row == randomNumber && b->column == Column - 1) {
			block = b;
			break;
		}
	}

	if (block != nullptr) {
		Vector3 v = block->model.getTranslation();
		bear->model.setTranslation(v.x, v.y, v.z);
		bear->row = block->row;
		bear->column = block->column;
	}

#pragma endregion

	children.push_back(bear);
}

void World::Spawn_SnowFlake(Shader* shader)
{
	snowflake* s = new snowflake();
	s->Init(shader);

#pragma region set position

	std::vector<Block*> blocks = Get_Grids();

	Block* block = nullptr;

	do {
		std::srand(static_cast<unsigned int>(std::time(nullptr)));

		int rand_Row = std::rand() % this->Row;
		int rand_Col = std::rand() % this->Column - 1;		
		for (int i = 0; i < blocks.size(); i++) {
			Block* b = blocks[i];
			if (!b->obj_Placed && b->row == rand_Row && b->column == rand_Col) {
				block = b;
				break;
			}
		}
	} while (block == nullptr);
	
	if (block != nullptr) {
		Vector3 v = block->model.getTranslation();
		s->model.setTranslation(v.x, v.y + 2, v.z);
	}

#pragma endregion

	children.push_back(s);
}

void World::Remove_SnowFlake()
{
	for (int i = 0; i < this->children.size(); i++) {
		if (((EntityMesh*)children[i])->Type == SNOWFLAKE)
		{
			children.erase(std::remove(children.begin(), children.end(), children[i]), children.end());
			break;
		}
	}
}

//void World::Bear_Raid(Shader* shader)
//{
//	if (!bear_Raid) {
//		bear_Raid = true;
//	}
//}


