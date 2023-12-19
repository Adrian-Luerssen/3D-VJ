#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include <math.h>
#include <ctime>
#include <chrono>
#include <stb/stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "VBO.h"
#include "VAO.h"
#include "EBO.h"
#include "Texture.h"
#include "Renderer.h"
#include "RenderSystem.h"
#include "ScriptSystem.h"
#include "ScriptManager.h"
#include "FirstPersonCameraScript.h"
#include "Script.h"

#include "ECS.h"
#include "SpawnerScript.h"
#include "SkyboxScript.h"

#include "AsteroidScript.h"
#include "BulletScript.h"
#include "UserScript.h"
#include "UIScript.h"
using std::cout;
using std::endl;
using std::chrono::duration_cast;
using std::chrono::milliseconds;
using std::chrono::seconds;
using std::chrono::system_clock;
using namespace ECS;

GLFWwindow* window; // Game window
const unsigned int width = 800;
const unsigned int height = 800;

float t = 0;
time_t current_time;

World* world;


void SetupGLFW() {

	glfwInit();

	// Tell GLFW we are using OpenGL 3.3 and the CORE profile (only the modern functions)
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

}

bool SetupWindow() {
	//Create a GLFWwindow with size 800x800
	window = glfwCreateWindow(width, height, "ProgramacioVideojocs", NULL, NULL);
	if (window == NULL) {

		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return false;
	}

	// Make window part of the current context
	glfwMakeContextCurrent(window);

	//Load GLAD and specify the viewport
	gladLoadGL();
	glViewport(0, 0, width, height);

	glEnable(GL_BLEND);
	glEnable(GL_CULL_FACE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	return true;
}

Entity* CreateEntity2D(glm::vec2 position, float rotation, float scale, const char* filepath, glm::vec3 color,
	bool autoSize = true, glm::vec2 size = glm::vec2(1.0, 1.0), const char* shaderName = "unlit") {
	Entity* ent = world->create();
	ent->assign<Transform2D>(position, rotation, scale);
	ent->assign<Sprite>(filepath, color, autoSize, size, shaderName);

	return ent;
}


Entity* CreateEntity3DWithMesh(glm::vec3 position, float scale, const char* meshFilepath, const char* texFilepath, const char* normalsFilepath = "Textures/flat_normals.png") {
	Entity* ent = world->create();
	ent->assign<Transform3D>(position, scale);
	ent->assign<MeshComponent>(texFilepath, meshFilepath, "default", normalsFilepath);

	return ent;
}

Entity* CreateEntity3DEmpty() {
	Entity* ent = world->create();

	return ent;
}

Entity* CreateCamera(glm::vec3 position) {
	Entity* ent = world->create();
	ent->assign<Camera>(position, glm::vec3(0., 0., -1.), glm::vec3(0., 1., 0.));

	return ent;
}

Entity* CreateSkybox(const char* meshFilepath, const char* texFilepath) {
	Entity* ent = world->create();
	ent->assign<Skybox>(texFilepath, meshFilepath);

	return ent;
}

void SetupWorld() {

	RenderSystem* rs = new RenderSystem(width, height);

	world = World::createWorld();
	world->registerSystem(rs);
	ScriptSystem* scriptSystem = new ScriptSystem();
	world->registerSystem(scriptSystem);

	ScriptManager* scriptManager = scriptSystem->getScriptManager();

	Entity* ent = CreateCamera(glm::vec3(30.0f, 2.f, 30.0f));
	FirstPersonCameraScript* fps = new FirstPersonCameraScript(window, world, ent);

	ent->assign<ScriptComponent>(scriptManager->AddScript(fps));

	rs->setCamera(ent);

	/**/Entity* spawner = CreateEntity3DEmpty();
	spawner->assign<GameController>();
	SpawnerScript* spawner_script = new SpawnerScript(window, world, spawner);
	spawner->assign<ScriptComponent>(scriptManager->AddScript(spawner_script));

	Entity* bulletManager = CreateEntity3DEmpty();
	BulletScript* bulletScript = new BulletScript(window, world, bulletManager);
	bulletManager->assign<ScriptComponent>(scriptManager->AddScript(bulletScript));

	Entity* uiManager = CreateEntity3DEmpty();
	UIScript* uiScript = new UIScript(window, world, uiManager);
	uiManager->assign<ScriptComponent>(scriptManager->AddScript(uiScript));

	Entity* asteroidManager = CreateEntity3DEmpty();
	AsteroidScript* asteroidScript = new AsteroidScript(window, world, asteroidManager);
	asteroidManager->assign<ScriptComponent>(scriptManager->AddScript(asteroidScript));

	Entity* skybox = CreateSkybox("Meshes/flipped_sphere.obj", "Textures/space/background.png");
	//Entity* skybox2 = CreateSkybox("Meshes/flipped_sphere.obj", "Textures/space/nebula_1.png");
	//Entity* skybox3 = CreateSkybox("Meshes/flipped_sphere.obj", "Textures/space/nebula_2.png");
	Entity* skybox4 = CreateSkybox("Meshes/flipped_sphere.obj", "Textures/space/space2.png");

	//Entity* floor = CreateEntity3DWithMesh(glm::vec3(32, 0, 18), 30, "Meshes/plane.obj", "Textures/background_brown.png", "Textures/sand/Sand_norm.png");


	//Entity* wall = CreateEntity3DWithMesh(glm::vec3(4, 2, 4), 2, "Meshes/asteroide1.obj", "Textures/sand/Sand_norm.png", "Textures/asteroide1/Mat1_Normal_DirectX.png");
	//wall->assign<CubeCollider>(2.1, 2.1, 2.1);
	//wall->assign<UserComponent>();



	Entity* sprite = CreateEntity2D(glm::vec2(400., 400.), 0.f, 1.f, "Textures/text/score_text.png", glm::vec3(1., 1., 1.));
	sprite->assign<UIComponent>("ScoreText","Game");
	Entity* sprite2 = CreateEntity2D(glm::vec2(400., 400.), 0.f, 1.f, "Textures/text/0.png", glm::vec3(1., 1., 1.));
	sprite2->assign<ScoreComponent>(1);
	sprite->assign<UIComponent>("ScoreVal", "Game");



	Entity* obj1 = CreateEntity3DWithMesh(glm::vec3(0., 1.5, 0.), 1, "Meshes/spaceship.obj", "Textures/spaceship/color.png", "Textures/spaceship/normal.png");
	obj1->assign<UserComponent>();
	obj1->get<MeshComponent>()->roughnessFilepath = "Textures/spaceship/roughness.png";
	obj1->get<MeshComponent>()->metallicFilepath = "Textures/spaceship/metallic.png";
	obj1->get<MeshComponent>()->emissiveFilepath = "Textures/spaceship/emissive.png";
	obj1->get<MeshComponent>()->shaderName = "user";
	UserScript* us = new UserScript(window, world, obj1);
	obj1->assign<ScriptComponent>(scriptManager->AddScript(us));
	

}

int main() {

	SetupGLFW();

	if (!SetupWindow()) {
		return -1;
	}

	SetupWorld();

	float dt = 0;
	float time = clock();

	//Program core loop
	while (!glfwWindowShouldClose(window)) {

		// Specify the color of the background
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		// Clean the back buffer and assign the new color to it
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		dt = clock() - time;
		time = clock();
		if (dt < 50) {
			world->tick(dt);
		}

		glfwSwapBuffers(window); //Swap buffers

		// Take care of GLFW events
		glfwPollEvents();
	}

	// Cleanup

	glfwDestroyWindow(window);
	glfwTerminate();

	world->destroyWorld();

	return 0;
}