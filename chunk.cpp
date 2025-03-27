
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

static float getdb(const glm::vec3& pos, const glm::vec3& pos2) {
	return glm::sqrt(pow(pos.x - pos2.x, 2) + pow(pos.y - pos2.y, 2) + pow(pos.z - pos2.z, 2));
}

void Chunk::init(std::vector<Texture>& textures, glm::vec3 campos) {

	float db = getdb(position_vec3, campos);
	if (db < size * 4) {
		detGen(textures, &mesh);
		lodLevel = 1;
	}
	
	else if (db < size * 8) {
		lodGen(textures, 2, &mesh);
		lodLevel = 2;
	}

	else if (db < size * 12) {
		lodGen(textures, 4, &mesh);
		lodLevel = 4;
	}

	else if (db < size * 16) {
		lodGen(textures, 8, &mesh);
		lodLevel = 8;
	}

	else if (db < size * 20) {
		lodGen(textures, 16, &mesh);
		lodLevel = 16;
	}

	else if (db < size * 24) {
		lodGen(textures, 32, &mesh);
		lodLevel = 32;
	}

	else{
		lodGen(textures, 64, &mesh);
		lodLevel = 64;
	}
	//lodGen(textures, 4, &meshLod);
	
	/*
	std::vector <Vertex> vertices;
	std::vector <GLuint> indices;

	int i = 0;
	int sizeM = 4;
	for (int ix = 0; ix < size; ix++) {
		for (int iz = 0; iz < size; iz++) {

			if (ix % sizeM == 0 && iz % sizeM == 0) {

				int dx = ix;
				int dz = iz;

				float fx = (float)(dx + x);
				float fz = (float)(dz + z);

				vertices.push_back(Vertex{ glm::vec3((short)(0 + dx), noise(Noise, fx       ,fz),         (short)(dz)),     glm::vec2(0, 0) });
				vertices.push_back(Vertex{ glm::vec3((short)(0 + dx), noise(Noise, fx       ,fz + 4.0f),  (short)(4 + dz)), glm::vec2(0, 1) });
				vertices.push_back(Vertex{ glm::vec3((short)(4 + dx), noise(Noise, fx + 4.0f,fz + 4.0f),  (short)(4 + dz)), glm::vec2(1, 1) });
				vertices.push_back(Vertex{ glm::vec3((short)(4 + dx), noise(Noise, fx + 4.0f,fz),         (short)(dz)),     glm::vec2(1, 0) });
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
	*/
}

void Chunk::detGen(std::vector<Texture>& textures, Mesh* tempmesh) {
	std::vector <Vertex> vertices;
	std::vector <GLuint> indices;

	int i = 0;
	for (int ix = 0; ix < size; ix++) {
		for (int iz = 0; iz < size; iz++) {

			int dx = ix;
			int dz = iz;

			float fx = (float)(dx + x);
			float fz = (float)(dz + z);

			vertices.push_back(Vertex{ glm::vec3((short)(0 + dx), noise(Noise, fx       ,fz),         (short)(dz)),     glm::vec2(0, 0) });
			vertices.push_back(Vertex{ glm::vec3((short)(0 + dx), noise(Noise, fx       ,fz + 1.0f),  (short)(1 + dz)), glm::vec2(0, 1) });
			vertices.push_back(Vertex{ glm::vec3((short)(1 + dx), noise(Noise, fx + 1.0f,fz + 1.0f),  (short)(1 + dz)), glm::vec2(1, 1) });
			vertices.push_back(Vertex{ glm::vec3((short)(1 + dx), noise(Noise, fx + 1.0f,fz),         (short)(dz)),     glm::vec2(1, 0) });
			indices.push_back(i + 0);
			indices.push_back(i + 1);
			indices.push_back(i + 2);
			indices.push_back(i + 0);
			indices.push_back(i + 2);
			indices.push_back(i + 3);
			i += 4;
			
		}
	}

	tempmesh->nymesh(vertices, indices, textures);
}

void Chunk::lodGen(std::vector<Texture>& textures, int sizeM, Mesh* tempmesh) {

	std::vector <Vertex> vertices;
	std::vector <GLuint> indices;
	int i = 0;
	for (int ix = 0; ix < size; ix++) {
		for (int iz = 0; iz < size; iz++) {

			if (ix % sizeM == 0 && iz % sizeM == 0) {

				int dx = ix;
				int dz = iz;

				float fx = (float)(dx + x);
				float fz = (float)(dz + z);

				vertices.push_back(Vertex{ glm::vec3((short)(0 + dx), noise(Noise, fx       ,fz),         (short)(dz)),     glm::vec2(0, 0) });
				vertices.push_back(Vertex{ glm::vec3((short)(0 + dx), noise(Noise, fx       ,fz + sizeM),  (short)(sizeM + dz)), glm::vec2(0, 1) });
				vertices.push_back(Vertex{ glm::vec3((short)(sizeM + dx), noise(Noise, fx + sizeM,fz + sizeM),  (short)(sizeM + dz)), glm::vec2(1, 1) });
				vertices.push_back(Vertex{ glm::vec3((short)(sizeM + dx), noise(Noise, fx + sizeM,fz),         (short)(dz)),     glm::vec2(1, 0) });
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
	tempmesh->nymesh(vertices, indices, textures);
}



void Chunk::render(Shader& shader, Camera& camera, std::vector<Texture>& textures) {
	mesh.Draw(shader, camera, position_vec3);
	
	float db = getdb(position_vec3, camera.Position);
	if (db < size * 4 && lodLevel != 1) {
		lodLevel = 1;
		detGen(textures, &mesh);
	}

	else if (db > size * 4 && db < size * 8 && lodLevel != 2) {
		lodLevel = 2;
		lodGen(textures, 2, &mesh);
	}

	else if (db > size * 8 && db < size * 12 && lodLevel != 4) {
		lodLevel = 4;
		lodGen(textures, 4, &mesh);
	}
	
	else if (db > size * 12 && db < size * 16 && lodLevel != 8) {
		lodLevel = 8;
		lodGen(textures, 8, &mesh);
	}
	
	else if (db > size * 16 && db < size * 20 && lodLevel != 16){
		lodLevel = 16;
		lodGen(textures, 16, &mesh);
	}
	
	else if (db > size * 20 && db < size * 24 && lodLevel != 32) {
		lodLevel = 32;
		lodGen(textures, 32, &mesh);
	}
	
	else if(db > size * 24 && lodLevel != 64){
		lodLevel = 64;
		lodGen(textures, 64, &mesh);
	}

}