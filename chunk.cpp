
#include "Chunk.h"

Chunk::Chunk(int x, int z, int size) {
	Chunk::x = x;
	Chunk::y = 0;
	Chunk::z = z;
	Chunk::position = glm::vec3(x, 0, z);
	Chunk::size = size;
	Chunk::Noise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
}

static float noise(FastNoiseLite Noise, float x, float z) {
	return (Noise.GetNoise(x, z) * 10) + pow(Noise.GetNoise(x / 10, z / 10) * 2, 2) * 50 - 30;
}

static glm::vec3 calNormal() {

}

void Chunk::init(std::vector<Texture>& textures) {

	std::vector <Vertex> vertices;
	std::vector <GLuint> indices;

	int i = 0;
	for (int dx = x; dx < x + size; dx++) {
		for (int dz = z; dz < z + size; dz++) {

			float fx = (float)dx;
			float fz = (float)dz;

			vertices.push_back(Vertex{ glm::vec3(0.0f + dx, noise(Noise, fx    ,fz    ),  0.0f + dz), glm::vec2(0.0f, 0.0f)});
			vertices.push_back(Vertex{ glm::vec3(0.0f + dx, noise(Noise, fx    ,fz + 1),  1.0f + dz), glm::vec2(0.0f, 1.0f) });
			vertices.push_back(Vertex{ glm::vec3(1.0f + dx, noise(Noise, fx + 1,fz + 1),  1.0f + dz), glm::vec2(1.0f, 1.0f) });
			vertices.push_back(Vertex{ glm::vec3(1.0f + dx, noise(Noise, fx + 1,fz    ),  0.0f + dz), glm::vec2(1.0f, 0.0f) });
			indices.push_back(i + 0);
			indices.push_back(i + 1);
			indices.push_back(i + 2);
			indices.push_back(i + 0);
			indices.push_back(i + 2);
			indices.push_back(i + 3);
			i += 4;
		}
	}

	Chunk::mesh.nymesh(vertices, indices, textures);
}

void Chunk::render(Shader& shader, Camera& camera) {
	mesh.Draw(shader, camera);
}