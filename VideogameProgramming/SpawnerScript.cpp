#include "SpawnerScript.h"

void SpawnerScript::startScript()
{
}

void SpawnerScript::tickScript(float deltaTime)
{
	ComponentHandle<GameController> game;
	world->each<GameController>([&](Entity* ent, ComponentHandle<GameController> gameController) {game = gameController; });
	if (game->pause) {
		firstSpawn = true;
		return;
	}else{

		// spawn bullets on mouse click
		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
			if (!firstSpawn){
				if (firstClick) {
					firstClick = false;
					t = 0;
					glm::vec3 userPos = glm::vec3(0.0f);
					glm::vec3 objRot = glm::vec3(0.0f);
					glm::vec3 userLookAt = glm::vec3(0.0f);
					world->each<UserComponent>([&](Entity* ent, ComponentHandle<UserComponent> user) {
						userPos = ent->get<Transform3D>()->position;
						objRot = ent->get<Transform3D>()->rotation;

						glm::mat4 rotationMatrix =
							glm::rotate(glm::mat4(1.0f), objRot.y, glm::vec3(0.0f, 1.0f, 0.0f)) *
							glm::rotate(glm::mat4(1.0f), objRot.x, glm::vec3(1.0f, 0.0f, 0.0f)) *
							glm::rotate(glm::mat4(1.0f), objRot.z, glm::vec3(0.0f, 0.0f, 1.0f));

						glm::vec3 forward = glm::normalize(glm::vec3(rotationMatrix * glm::vec4(0.0f, 0.0f, -1.0f, 0.0f)));
						userLookAt = glm::normalize(forward);
						});

					//userLookAt = -userLookAt;
					cout << "spawned at " << userPos.x << "," << userPos.y << "," << userPos.z << endl;
					Entity* ent = world->create();
					ent->assign<Transform3D>(userPos, 5, objRot);
					ent->assign<MeshComponent>("Textures/flat_normal.png", "Meshes/cube.obj", "bullet");
					ent->assign<BulletComponent>(userPos, userLookAt);
					ent->assign<CubeCollider>(2, 2, 2);
				}
			}
			
		}
		else  {
			firstClick = true;
			if (firstSpawn) {
				firstSpawn = false;
			}
		}
		t += deltaTime / 2.0f;
		if (t > delay) {
			cout << "spawning new asteroid" << endl;
			Entity* ent = world->create();
			ent->assign<Transform3D>(glm::vec3(1000000, 1, 0.), 5);
			ent->assign<MeshComponent>("Textures/asteroids/color.png", "Meshes/asteroids/asteroide1.obj" , "default", "Textures/asteroids/normal.png");
			ent->assign<EnemyComponent>(1, counter);
			ent->assign<CubeCollider>(25, 25, 25);/**/
			//counter++;
			t = 0;
		}
		
	}
}
