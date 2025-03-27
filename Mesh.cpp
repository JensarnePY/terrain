#include "Mesh.h"

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

	VBO.Delete();
	EBO.Delete();
}

void Mesh::clear() {
	indices.clear();
	textures.clear();
	vertices.clear();
	VAO.ID = 0;
}

void Mesh::Draw(Shader& shader, Camera& camera, glm::vec3 pos)
{
	shader.Activate();
	VAO.Bind();

	glm::vec3 objectPos = pos;
	glm::mat4 objectModel = glm::mat4(1.0f);
	objectModel = glm::translate(objectModel, objectPos);
	//shaderProgram.Activate();
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1, GL_FALSE, glm::value_ptr(objectModel));
	
	for (auto& tex : textures) {
		tex.Bind();
	}
	
	glUniform1f(glGetUniformLocation(shader.ID, "time"), glfwGetTime());
	glUniform3f(glGetUniformLocation(shader.ID, "camPos"), camera.Position.x, camera.Position.y, camera.Position.z);

	

	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
}