#pragma once
#include "Mesh.h"
#include "texture.h"
#include "FastNoiseLite.h"

class Chunk {
public:
	int x = 0;
	int y = 0;
	int z = 0;
	int size = 0;
	glm::vec3 position;
	Mesh mesh;

	FastNoiseLite Noise;

	Chunk(int x, int z, int size);
	//Chunk();

	void init(std::vector<Texture>& textures);
	void render(Shader& shader, Camera& camera);
};