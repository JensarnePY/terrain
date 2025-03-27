
#include <algorithm>
#include"Mesh.h"
#include"chunk.h"

class Terrain {
public:
	int chunksize = 128;
	int renderDistent = 10;
	std::vector<Chunk> chunks;

	Terrain() = default;
	void update(Camera& camera, std::vector<Texture>& textures);
	void render(Shader& shader, Camera &camera, std::vector<Texture>& textures);

};


bool chunkExists(const std::vector<Chunk>& chunks, const glm::vec2& pos)
{	
	for (const auto& chunk : chunks) {
		if (chunk.position == pos) {
			return 1;
		}
	}
	return 0;
}

bool compareByMagnitude(const glm::vec2& a, const glm::vec2& b, const glm::vec2& pos) {
	return glm::distance(a, pos) < glm::distance(b, pos);
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
			}
		}
	}

	// sort ChunkToLoad
	glm::vec2 playerPosition = glm::vec2(camera.Position.x, camera.Position.z);
	std::sort(ChunkToLoad.begin(), ChunkToLoad.end(), [&playerPosition](const glm::vec2& a, const glm::vec2& b) {return compareByMagnitude(a, b, playerPosition);});


	if (!ChunkToLoad.size() == 0) {
		double start = glfwGetTime();
		Chunk chunk2(ChunkToLoad[0].x, ChunkToLoad[0].y/*z*/, chunksize);
		chunk2.init(textures, camera.Position);
		chunks.push_back(chunk2);
		std::cout << glfwGetTime() - start << "\n";
	}

}

void Terrain::render(Shader& shader, Camera& camera, std::vector<Texture>& textures) {
	for (auto &chunk : chunks) {
		chunk.render(shader, camera, textures);
	}
}

int main()
{
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	int width = 1280;
	int height = 720;
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
	textures[0].texUnit(shaderProgram, "tex0", 0);
	textures[1].texUnit(shaderProgram, "tex1", 1);
	textures[2].texUnit(shaderProgram, "tex2", 2);

	// Enables Depth Buffer
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST);

	Camera camera(width, height, glm::vec3(0.0f, 10.0f, 0.0f));

	//glm::vec3(glm::vec3(1.0f) * glm::vec3(0.5f));
	Terrain terrain;

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
			std::string FPS = std::to_string(int((1.0 / timediff) * cn));
			std::string ms = std::to_string((timediff / cn) * 1000);
			//dt = (timediff / cn) * 1000;
			std::string newTitle = FPS + "FPS / " + ms + "ms";
			glfwSetWindowTitle(window, newTitle.c_str());

			time2 = glfwGetTime();
			cn = 0;
			glfwGetWindowSize(window, &width, &height);
			glViewport(0, 0, width, height);
			camera.height = height;
			camera.width = width;
		}

		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		camera.Inputs(window, dt);
		camera.updateMatrix(50.0f, 0.1f, 10000.0f);
		camera.Matrix(shaderProgram, "camMatrix");
		terrain.update(camera, textures);
		terrain.render(shaderProgram, camera, textures);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	shaderProgram.Delete();
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}
