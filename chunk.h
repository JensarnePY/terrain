#pragma once
#include "Mesh.h"
#include "texture.h"
#include "FastNoiseLite.h"

class Chunk {
public:
	int x = 0;
	int z = 0;
	int size = 0;
	glm::vec2 position;
	glm::vec3 position_vec3;

	Mesh mesh;
	int lodLevel;

	FastNoiseLite Noise;

	Chunk(int x, int z, int size);

	void init(std::vector<Texture>& textures, glm::vec3 campos);
	void detGen(std::vector<Texture>& textures, Mesh* tempmesh);
	void lodGen(std::vector<Texture>& textures, int sizeM, Mesh* tempmesh);
	void render(Shader& shader, Camera& camera, std::vector<Texture>& textures);
};