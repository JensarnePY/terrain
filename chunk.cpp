
#include "Chunk.h"

Chunk::Chunk(int x, int z, int size) {
	Chunk::x = x;
	Chunk::z = z;
	Chunk::position = glm::vec2(x, z);
	Chunk::position_vec3 = glm::vec3(x, 0, z);
	Chunk::size = size;
	Chunk::Noise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
}

static float noise(FastNoiseLite Noise, float x, float z) {
	return Noise.GetNoise(x/15, z/15) * 250 + (Noise.GetNoise(x, z) * 10);
}



void Chunk::init(std::vector<Texture>& textures) {

	std::vector <Vertex> vertices;
	std::vector <GLuint> indices;
	int i = 0;
	for (int dx = 0; dx < size; dx++) {
		for (int dz = 0; dz < size; dz++) {

			float fx = (float)(dx + x);
			float fz = (float)(dz + z);

			vertices.push_back(Vertex{ glm::vec3((short)(    dx), noise(Noise, fx       ,fz       ),  (short)(  dz)), glm::vec2((int8_t)(0), (int8_t)(0))});
			vertices.push_back(Vertex{ glm::vec3((short)(    dx), noise(Noise, fx       ,fz + 1.0f),  (short)(1+dz)), glm::vec2((int8_t)(0), (int8_t)(1)) });
			vertices.push_back(Vertex{ glm::vec3((short)(1 + dx), noise(Noise, fx + 1.0f,fz + 1.0f),  (short)(1+dz)), glm::vec2((int8_t)(1), (int8_t)(1)) });
			vertices.push_back(Vertex{ glm::vec3((short)(1 + dx), noise(Noise, fx + 1.0f,fz       ),  (short)(  dz)), glm::vec2((int8_t)(1), (int8_t)(0)) });
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

	vertices.clear();
	indices.clear();
	i = 0;
	int sizeM = 4;
	for (int ix = 0; ix < size; ix++) {
		for (int iz = 0; iz < size; iz++) {

			if (ix % sizeM == 0 && iz % sizeM == 0) {

				int dx = ix ;
				int dz = iz ;

				float fx = (float)(dx + x);
				float fz = (float)(dz + z);

				vertices.push_back(Vertex{ glm::vec3((short)(0 + dx), noise(Noise, fx       ,fz),         (short)(dz)),     glm::vec2((int8_t)(0), (int8_t)(0)) });
				vertices.push_back(Vertex{ glm::vec3((short)(0 + dx), noise(Noise, fx       ,fz + 4.0f),  (short)(4 + dz)), glm::vec2((int8_t)(0), (int8_t)(1)) });
				vertices.push_back(Vertex{ glm::vec3((short)(4 + dx), noise(Noise, fx + 4.0f,fz + 4.0f),  (short)(4 + dz)), glm::vec2((int8_t)(1), (int8_t)(1)) });
				vertices.push_back(Vertex{ glm::vec3((short)(4 + dx), noise(Noise, fx + 4.0f,fz),         (short)(dz)),     glm::vec2((int8_t)(1), (int8_t)(0)) });
				indices.push_back(i + 0);
				indices.push_back(i + 1);
				indices.push_back(i + 2);
				indices.push_back(i + 0);
				indices.push_back(i + 2);
				indices.push_back(i + 3);
				i += 4;
			}

			
		}
	}
	

	Chunk::meshLod.nymesh(vertices, indices, textures);
}

static float getdb(const glm::vec3& pos, const glm::vec3& pos2) {
	return glm::sqrt(pow(pos.x - pos2.x, 2) + pow(pos.y - pos2.y, 2) + pow(pos.z - pos2.z, 2));
}

void Chunk::render(Shader& shader, Camera& camera) {
	if (getdb(position_vec3, camera.Position) < size * 4) {
		mesh.Draw(shader, camera, position_vec3);
	}
	else {
		meshLod.Draw(shader, camera, position_vec3);
	}
		
}