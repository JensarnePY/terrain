#include "Mesh.h"

Mesh::Mesh(std::vector <Vertex>& vertices, std::vector <GLuint>& indices, std::vector<Texture>& textures)
{
	Mesh::vertices = vertices;
	Mesh::indices = indices;
	Mesh::textures = textures;

	VAO.Bind();
	VBO VBO(vertices);
	EBO EBO(indices);
	VAO.LinkAttrib(VBO, 0, 3, GL_FLOAT, sizeof(Vertex), (void*)0);
	//VAO.LinkAttrib(VBO, 1, 3, GL_FLOAT, sizeof(Vertex), (void*)(3 * sizeof(float)));
	//VAO.LinkAttrib(VBO, 2, 3, GL_FLOAT, sizeof(Vertex), (void*)(6 * sizeof(float)));
	VAO.LinkAttrib(VBO, 3, 2, GL_FLOAT, sizeof(Vertex), (void*)(9 * sizeof(float)));
	// Unbind all to prevent accidentally modifying them
	VAO.Unbind();
	VBO.Unbind();
	EBO.Unbind();
}

void Mesh::nymesh(std::vector <Vertex>& vertices, std::vector <GLuint>& indices, std::vector<Texture>& textures)
{
	Mesh::vertices = vertices;
	Mesh::indices = indices;
	Mesh::textures = textures;

	VAO.Bind();
	VBO VBO(vertices);
	EBO EBO(indices);
	VAO.LinkAttrib(VBO, 0, 3, GL_FLOAT, sizeof(Vertex), (void*)0);
	VAO.LinkAttrib(VBO, 1, 2, GL_FLOAT, sizeof(Vertex), (void*)(3 * sizeof(float)));
	// Unbind all to prevent accidentally modifying them
	VAO.Unbind();
	VBO.Unbind();
	EBO.Unbind();
}


void Mesh::Draw(Shader& shader, Camera& camera)
{
	shader.Activate();
	VAO.Bind();

	//for (int i; i > textures.size(); i++) {
	//	std::string num = std::to_string(i);
	//	textures[i].texUnit(shader, (std::string("tex") + num), i);
	//	textures[i].Bind();
	//}

	textures[0].texUnit(shader, "tex0", 0);
	textures[0].Bind();
	textures[1].texUnit(shader, "tex1", 1);
	textures[1].Bind();
	textures[2].texUnit(shader, "tex2", 2);
	textures[2].Bind();
	
	glUniform1f(glGetUniformLocation(shader.ID, "time"), glfwGetTime());
	glUniform3f(glGetUniformLocation(shader.ID, "camPos"), camera.Position.x, camera.Position.y, camera.Position.z);

	camera.Matrix(shader, "camMatrix");

	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
}