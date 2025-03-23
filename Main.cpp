
#include <algorithm>
#include"Mesh.h"
#include"chunk.h"

class Terrain {
public:
	int chunksize = 128;
	int renderDistent = 10;
	std::vector<Chunk> chunks;

	Terrain(std::vector<Texture>& textures);
	void update(Camera& camera, std::vector<Texture>& textures);
	void render(Shader& shader, Camera &camera);

};

Terrain::Terrain(std::vector<Texture>& textures) {
	//Chunk chunk(-chunksize / 2, -chunksize / 2, chunksize);
	//chunk.init(textures);
	//chunks.push_back(chunk);
}


bool chunkExists(const std::vector<Chunk>& chunks, const glm::vec2& pos)
{	
	for (const auto& chunk : chunks) {
		if (chunk.position == pos) {
			//std::cout << "1" << "\n";
			return 1;
		}
	}
	//std::cout << "0" << "\n";
	return 0;


	//return std::any_of(chunks.begin(), chunks.end(), [&pos](const Chunk& chunk)
	//	{
	//		return chunk.position == pos;
	//	});
}


void Terrain::update(Camera& camera, std::vector<Texture>& textures) {

	std::vector<glm::vec2>ChunkToLoad;
	int current_chunkX = static_cast<int>(std::floor(camera.Position.x / chunksize));
	int current_chunkZ = static_cast<int>(std::floor(camera.Position.z / chunksize));
	for (int x = -renderDistent; x <= renderDistent; x++) {
		for (int z = -renderDistent; z <= renderDistent; z++) {

			glm::vec2 newChunkPos(
				(current_chunkX + x) * chunksize,
				(current_chunkZ + z) * chunksize);

			if (!chunkExists(chunks, newChunkPos)) {
				ChunkToLoad.push_back(glm::vec2(newChunkPos.x, newChunkPos.y/*z*/));
				Chunk chunk2(ChunkToLoad[0].x, ChunkToLoad[0].y/*z*/, chunksize);
				chunk2.init(textures);
				chunks.push_back(chunk2);
				ChunkToLoad.clear();
			}
		}
	}


	//for (const auto& chunk : chunks) {
	//	for (const auto& chunk2 : chunks) {
	//		if (chunk.position == chunk2.position) {
	//			std::cout << "dup\n";
	//		}
	//	}
	//}


	//for (auto load : ChunkToLoad) {
	//	Chunk chunk2(load.x, load.z, chunksize);
	//	chunk2.init(textures);
	//	chunks.push_back(chunk2);
	//	break;
	//}
	

}

void Terrain::render(Shader& shader, Camera& camera) {
	for (auto &chunk : chunks) {
		chunk.render(shader, camera);
	}
}

int width = 1200;
int height = 1200;

int main()
{
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(width, height, "", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	gladLoadGL();
	glViewport(0, 0, width, height);

	Shader shaderProgram("res/default.vert", "res/default.frag");
	// Texture data
	std::vector<Texture>textures{
		Texture("res/grass.png", 0, GL_RGBA, GL_UNSIGNED_BYTE),
		Texture("res/water.png", 1, GL_RGBA, GL_UNSIGNED_BYTE),
		Texture("res/stone.png", 2, GL_RGBA, GL_UNSIGNED_BYTE),
	};
	
	
	
	glm::vec3 objectPos = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::mat4 objectModel = glm::mat4(1.0f);
	objectModel = glm::translate(objectModel, objectPos);
	shaderProgram.Activate();
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "model"), 1, GL_FALSE, glm::value_ptr(objectModel));


	// Enables Depth Buffer
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST);

	Camera camera(width, height, glm::vec3(0.0f, 10.0f, 0.0f));

	//glm::vec3(glm::vec3(1.0f) * glm::vec3(0.5f));
	Terrain terrain(textures);

	int cn = 0;
	double time = glfwGetTime();
	double time2 = glfwGetTime();
	while (!glfwWindowShouldClose(window))
	{

		cn++;
		double timediff = glfwGetTime() - time2;
		double dt = glfwGetTime() - time;
		time = glfwGetTime();
		if (glfwGetTime() - time2 >= 0.25)
		{
			std::string FPS = std::to_string((1.0 / timediff) * cn);
			std::string ms = std::to_string((timediff / cn) * 1000);
			std::string newTitle = FPS + "FPS / " + ms + "ms";
			glfwSetWindowTitle(window, newTitle.c_str());

			time2 = glfwGetTime();
			cn = 0;
			glfwGetWindowSize(window, &width, &height);
			glViewport(0, 0, width, height);
			//camera.setWinSize(width, height);
		}

		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		camera.Inputs(window);
		camera.updateMatrix(60.0f, 0.1f, 10000.0f);

		terrain.update(camera, textures);
		terrain.render(shaderProgram, camera);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	shaderProgram.Delete();
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}
