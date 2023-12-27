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
#include "SoundSystem.h"
#include "EnemyShipScript.h"
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
	SoundSystem* soundSystem = new SoundSystem();
	world->registerSystem(soundSystem);

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

	Entity* enemyShipManager = CreateEntity3DEmpty();
	EnemyShipScript* enemyShipScript = new EnemyShipScript(window, world, enemyShipManager);
	enemyShipManager->assign<ScriptComponent>(scriptManager->AddScript(enemyShipScript));

	Entity* skybox = CreateSkybox("Meshes/flipped_sphere.obj", "Textures/space/background.png");
	//Entity* skybox2 = CreateSkybox("Meshes/flipped_sphere.obj", "Textures/space/nebula_1.png");
	//Entity* skybox3 = CreateSkybox("Meshes/flipped_sphere.obj", "Textures/space/nebula_2.png");
	Entity* skybox4 = CreateSkybox("Meshes/flipped_sphere.obj", "Textures/Enemy/cannon_normal.png");

	//Entity* floor = CreateEntity3DWithMesh(glm::vec3(32, 0, 18), 30, "Meshes/plane.obj", "Textures/background_brown.png", "Textures/sand/Sand_norm.png");


	//Entity* wall = CreateEntity3DWithMesh(glm::vec3(4, 2, 4), 2, "Meshes/asteroide1.obj", "Textures/sand/Sand_norm.png", "Textures/asteroide1/Mat1_Normal_DirectX.png");
	//wall->assign<CubeCollider>(2.1, 2.1, 2.1);
	//wall->assign<UserComponent>();





	Entity* obj1 = CreateEntity3DWithMesh(glm::vec3(0., 1.5, 0.), 1, "Meshes/spaceship.obj", "Textures/spaceship/color.png", "Textures/spaceship/normal.png");
	obj1->assign<UserComponent>();
	obj1->get<MeshComponent>()->roughnessFilepath = "Textures/spaceship/roughness.png";
	obj1->get<MeshComponent>()->metallicFilepath = "Textures/spaceship/metallic.png";
	obj1->get<MeshComponent>()->emissiveFilepath = "Textures/spaceship/emissive.png";
	obj1->get<MeshComponent>()->mixedAO = "Textures/spaceship/mixed_ao.png";
	obj1->get<MeshComponent>()->shaderName = "user";
	obj1->assign<CubeCollider>(25,10,50);
	UserScript* us = new UserScript(window, world, obj1);
	obj1->assign<ScriptComponent>(scriptManager->AddScript(us));
	obj1->assign<SoundComponent>("Sounds/Hit.wav", 1.0f);

	// spawn temp asteroids:

	for (int i = 1; i <= 60; i++) {
		Entity* ent = world->create();
		ent->assign<Transform3D>(glm::vec3(1000000, 1, 0.), 5);
		switch (i) {
		case 1:
			ent->assign<MeshComponent>("Textures/asteroids/color.png", "Meshes/asteroids/asteroide1.obj", "default", "Textures/asteroids/normal.png");
			break;
		case 2:
			ent->assign<MeshComponent>("Textures/asteroids/color.png", "Meshes/asteroids/asteroide2.obj", "default", "Textures/asteroids/normal.png");
			break;
		case 3:
			ent->assign<MeshComponent>("Textures/asteroids/color.png", "Meshes/asteroids/asteroide3.obj", "default", "Textures/asteroids/normal.png");
			break;
		case 4:
			ent->assign<MeshComponent>("Textures/asteroids/color.png", "Meshes/asteroids/asteroide4.obj", "default", "Textures/asteroids/normal.png");
			break;
		case 5:
			ent->assign<MeshComponent>("Textures/asteroids/color.png", "Meshes/asteroids/asteroide5.obj", "default", "Textures/asteroids/normal.png");
			break;
		case 6:
			ent->assign<MeshComponent>("Textures/asteroids/color.png", "Meshes/asteroids/asteroide6.obj", "default", "Textures/asteroids/normal.png");
			break;
		case 7:
			ent->assign<MeshComponent>("Textures/asteroids/color.png", "Meshes/asteroids/asteroide7.obj", "default", "Textures/asteroids/normal.png");
			break;
		case 8:
			ent->assign<MeshComponent>("Textures/asteroids/color.png", "Meshes/asteroids/asteroide8.obj", "default", "Textures/asteroids/normal.png");
			break;
		case 9:
			ent->assign<MeshComponent>("Textures/asteroids/color.png", "Meshes/asteroids/asteroide9.obj", "default", "Textures/asteroids/normal.png");
			break;
		case 10:
			ent->assign<MeshComponent>("Textures/asteroids/color.png", "Meshes/asteroids/asteroide10.obj", "default", "Textures/asteroids/normal.png");
			break;
		case 11:
			ent->assign<MeshComponent>("Textures/Enemy/spaceship_baseColor.png", "Meshes/Enemy/shard1.obj", "user", "Textures/Enemy/spaceship_normal.png");
			break;
		case 12:
			ent->assign<MeshComponent>("Textures/Enemy/spaceship_baseColor.png", "Meshes/Enemy/shard2.obj", "user", "Textures/Enemy/spaceship_normal.png");
			break;
		case 13:
			ent->assign<MeshComponent>("Textures/Enemy/spaceship_baseColor.png", "Meshes/Enemy/shard3.obj", "user", "Textures/Enemy/spaceship_normal.png");
			break;
		case 14:
			ent->assign<MeshComponent>("Textures/Enemy/spaceship_baseColor.png", "Meshes/Enemy/shard4.obj", "user", "Textures/Enemy/spaceship_normal.png");
			break;
		case 15:
			ent->assign<MeshComponent>("Textures/Enemy/spaceship_baseColor.png", "Meshes/Enemy/shard5.obj", "user", "Textures/Enemy/spaceship_normal.png");
			break;
		case 16:
			ent->assign<MeshComponent>("Textures/Enemy/spaceship_baseColor.png", "Meshes/Enemy/shard6.obj", "user", "Textures/Enemy/spaceship_normal.png");
			break;
		case 17:
			ent->assign<MeshComponent>("Textures/Enemy/spaceship_baseColor.png", "Meshes/Enemy/shard7.obj", "user", "Textures/Enemy/spaceship_normal.png");
			break;
		case 18:
			ent->assign<MeshComponent>("Textures/Enemy/spaceship_baseColor.png", "Meshes/Enemy/shard8.obj", "user", "Textures/Enemy/spaceship_normal.png");
			break;
		case 19:
			ent->assign<MeshComponent>("Textures/Enemy/spaceship_baseColor.png", "Meshes/Enemy/shard9.obj", "user", "Textures/Enemy/spaceship_normal.png");
			break;
		case 20:
			ent->assign<MeshComponent>("Textures/Enemy/spaceship_baseColor.png", "Meshes/Enemy/shard10.obj", "user", "Textures/Enemy/spaceship_normal.png");
			break;
		case 21:
			ent->assign<MeshComponent>("Textures/Enemy/spaceship_baseColor.png", "Meshes/Enemy/shard11.obj", "user", "Textures/Enemy/spaceship_normal.png");
			break;
		case 22:
			ent->assign<MeshComponent>("Textures/Enemy/spaceship_baseColor.png", "Meshes/Enemy/shard12.obj", "user", "Textures/Enemy/spaceship_normal.png");
			break;
		case 23:
			ent->assign<MeshComponent>("Textures/Enemy/spaceship_baseColor.png", "Meshes/Enemy/shard13.obj", "user", "Textures/Enemy/spaceship_normal.png");
			break;
		case 24:
			ent->assign<MeshComponent>("Textures/Enemy/spaceship_baseColor.png", "Meshes/Enemy/shard14.obj", "user", "Textures/Enemy/spaceship_normal.png");
			break;
		case 25:
			ent->assign<MeshComponent>("Textures/Enemy/spaceship_baseColor.png", "Meshes/Enemy/shard15.obj", "user", "Textures/Enemy/spaceship_normal.png");
			break;
		case 26:
			ent->assign<MeshComponent>("Textures/Enemy/spaceship_baseColor.png", "Meshes/Enemy/shard16.obj", "user", "Textures/Enemy/spaceship_normal.png");
			break;
		case 27:
			ent->assign<MeshComponent>("Textures/Enemy/spaceship_baseColor.png", "Meshes/Enemy/shard17.obj", "user", "Textures/Enemy/spaceship_normal.png");
			break;
		case 28:
			ent->assign<MeshComponent>("Textures/Enemy/spaceship_baseColor.png", "Meshes/Enemy/shard18.obj", "user", "Textures/Enemy/spaceship_normal.png");
			break;
		case 29:
			ent->assign<MeshComponent>("Textures/Enemy/spaceship_baseColor.png", "Meshes/Enemy/shard19.obj", "user", "Textures/Enemy/spaceship_normal.png");
			break;
		case 30:
			ent->assign<MeshComponent>("Textures/Enemy/spaceship_baseColor.png", "Meshes/Enemy/shard20.obj", "user", "Textures/Enemy/spaceship_normal.png");
			break;
		case 31:
			ent->assign<MeshComponent>("Textures/Enemy/spaceship_baseColor.png", "Meshes/Enemy/shard21.obj", "user", "Textures/Enemy/spaceship_normal.png");
			break;
		case 32:
			ent->assign<MeshComponent>("Textures/Enemy/spaceship_baseColor.png", "Meshes/Enemy/shard22.obj", "user", "Textures/Enemy/spaceship_normal.png");
			break;
		case 33:
			ent->assign<MeshComponent>("Textures/Enemy/spaceship_baseColor.png", "Meshes/Enemy/shard23.obj", "user", "Textures/Enemy/spaceship_normal.png");
			break;
		case 34:
			ent->assign<MeshComponent>("Textures/Enemy/spaceship_baseColor.png", "Meshes/Enemy/shard24.obj", "user", "Textures/Enemy/spaceship_normal.png");
			break;
		case 35:
			ent->assign<MeshComponent>("Textures/asteroids/color.png", "Meshes/asteroids/shards/shard1.obj", "default", "Textures/asteroids/normal.png");
			break;
		case 36:
			ent->assign<MeshComponent>("Textures/asteroids/color.png", "Meshes/asteroids/shards/shard2.obj", "default", "Textures/asteroids/normal.png");
			break;
		case 37:
			ent->assign<MeshComponent>("Textures/asteroids/color.png", "Meshes/asteroids/shards/shard3.obj", "default", "Textures/asteroids/normal.png");
			break;
		case 38:
			ent->assign<MeshComponent>("Textures/asteroids/color.png", "Meshes/asteroids/shards/shard4.obj", "default", "Textures/asteroids/normal.png");
			break;
		case 39:
			ent->assign<MeshComponent>("Textures/asteroids/color.png", "Meshes/asteroids/shards/shard5.obj", "default", "Textures/asteroids/normal.png");
			break;
		case 40:
			ent->assign<MeshComponent>("Textures/asteroids/color.png", "Meshes/asteroids/shards/shard6.obj", "default", "Textures/asteroids/normal.png");
			break;
		case 41:
			ent->assign<MeshComponent>("Textures/asteroids/color.png", "Meshes/asteroids/shards/shard7.obj", "default", "Textures/asteroids/normal.png");
			break;
		case 42:
			ent->assign<MeshComponent>("Textures/asteroids/color.png", "Meshes/asteroids/shards/shard8.obj", "default", "Textures/asteroids/normal.png");
			break;
		case 43:
			ent->assign<MeshComponent>("Textures/asteroids/color.png", "Meshes/asteroids/shards/shard9.obj", "default", "Textures/asteroids/normal.png");
			break;
		case 44:
			ent->assign<MeshComponent>("Textures/asteroids/color.png", "Meshes/asteroids/shards/shard10.obj", "default", "Textures/asteroids/normal.png");
			break;
		case 45:
			ent->assign<MeshComponent>("Textures/asteroids/color.png", "Meshes/asteroids/shards/shard11.obj", "default", "Textures/asteroids/normal.png");
			break;
		case 46:
			ent->assign<MeshComponent>("Textures/asteroids/color.png", "Meshes/asteroids/shards/shard12.obj", "default", "Textures/asteroids/normal.png");
			break;
		case 47:
			ent->assign<MeshComponent>("Textures/asteroids/color.png", "Meshes/asteroids/shards/shard13.obj", "default", "Textures/asteroids/normal.png");
			break;
		case 48:
			ent->assign<MeshComponent>("Textures/asteroids/color.png", "Meshes/asteroids/shards/shard14.obj", "default", "Textures/asteroids/normal.png");
			break;
		case 49:
			ent->assign<MeshComponent>("Textures/asteroids/color.png", "Meshes/asteroids/shards/shard15.obj", "default", "Textures/asteroids/normal.png");
			break;
		case 50:
			ent->assign<MeshComponent>("Textures/asteroids/color.png", "Meshes/asteroids/shards/shard16.obj", "default", "Textures/asteroids/normal.png");
			break;
		case 51:
			ent->assign<MeshComponent>("Textures/asteroids/color.png", "Meshes/asteroids/shards/shard17.obj", "default", "Textures/asteroids/normal.png");
			break;
		case 52:
			ent->assign<MeshComponent>("Textures/asteroids/color.png", "Meshes/asteroids/shards/shard18.obj", "default", "Textures/asteroids/normal.png");
			break;
		case 53:
			ent->assign<MeshComponent>("Textures/asteroids/color.png", "Meshes/asteroids/shards/shard19.obj", "default", "Textures/asteroids/normal.png");
			break;
		case 54:
			ent->assign<MeshComponent>("Textures/asteroids/color.png", "Meshes/asteroids/shards/shard20.obj", "default", "Textures/asteroids/normal.png");
			break;
		case 55:
			ent->assign<MeshComponent>("Textures/asteroids/color.png", "Meshes/asteroids/shards/shard21.obj", "default", "Textures/asteroids/normal.png");
			break;
		case 56:
			ent->assign<MeshComponent>("Textures/asteroids/color.png", "Meshes/asteroids/shards/shard22.obj", "default", "Textures/asteroids/normal.png");
			break;
		case 57:
			ent->assign<MeshComponent>("Textures/asteroids/color.png", "Meshes/asteroids/shards/shard23.obj", "default", "Textures/asteroids/normal.png");
			break;
		case 58:
			ent->assign<MeshComponent>("Textures/asteroids/color.png", "Meshes/asteroids/shards/shard24.obj", "default", "Textures/asteroids/normal.png");
			break;
		case 59:
			ent->assign<MeshComponent>("Textures/asteroids/color.png", "Meshes/asteroids/shards/shard25.obj", "default", "Textures/asteroids/normal.png");
			break;
		case 60:
			ent->assign<MeshComponent>("Textures/flat_normal.png", "Meshes/bullet.obj", "bullet");
			break;

		}
		ent->assign<TemporaryComponent>();
	}

	Entity* ent1 = world->create();
	ent1->assign<Transform3D>(glm::vec3(100000, 1, 0.), 5);
	ent1->assign<MeshComponent>("Textures/Enemy/spaceship_baseColor.png", "Meshes/Enemy/enemy.obj", "user", "Textures/Enemy/spaceship_normal.png");
	ent1->get<MeshComponent>()->roughnessFilepath = "Textures/Enemy/spaceship_roughness2.png";
	//ent1->get<MeshComponent>()->metallicFilepath = "Textures/Enemy/spaceship_metallic.png";
	ent1->get<MeshComponent>()->emissiveFilepath = "Textures/Enemy/spaceship_emit.png";
	ent1->assign<TemporaryComponent>();


	Entity* ent2= world->create();
	ent2->assign<Transform3D>(glm::vec3(1000000, 1, 0.), 5);
	ent2->assign<MeshComponent>("Textures/Enemy/cannon_diffuse2.png", "Meshes/Enemy/canon.obj", "user", "Textures/Enemy/cannon1_normal.png");
	ent2->get<MeshComponent>()->roughnessFilepath = "Textures/Enemy/cannon_roughness.png";
	//canon->get<MeshComponent>()->metallicFilepath = "Textures/Enemy/cannon_metallic.png";
	ent2->get<MeshComponent>()->emissiveFilepath = "Textures/Enemy/cannon_emit.png";
	ent2->assign<TemporaryComponent>();

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