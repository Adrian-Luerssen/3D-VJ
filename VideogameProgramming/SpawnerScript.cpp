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
	}
	else {

		glm::vec3 userPos = glm::vec3(0.0f);
		// spawn bullets on mouse click
		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
			if (!firstSpawn) {
				if (firstClick) {
					firstClick = false;
					t = 0;
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
					// Assuming userPos is the ship's position, and userLookAt is the ship's facing direction

					// Calculate the offset distance for each side
					float offset = 3.0f;  // Adjust this value based on your requirements

					// Calculate the offset vectors for both sides
					glm::vec3 offsetLeft = glm::normalize(glm::cross(userLookAt, glm::vec3(0, 1, 0))) * offset;
					glm::vec3 offsetRight = -offsetLeft;  // Opposite direction for the right side

					// Calculate the spawn positions for both bullets
					glm::vec3 bulletSpawnPosLeft = userPos + offsetLeft;
					glm::vec3 bulletSpawnPosRight = userPos + offsetRight;

					cout << "Left bullet spawned at " << bulletSpawnPosLeft.x << "," << bulletSpawnPosLeft.y << "," << bulletSpawnPosLeft.z << endl;
					cout << "Right bullet spawned at " << bulletSpawnPosRight.x << "," << bulletSpawnPosRight.y << "," << bulletSpawnPosRight.z << endl;

					// Spawn left bullet
					Entity* entLeft = world->create();
					entLeft->assign<Transform3D>(bulletSpawnPosLeft, 5, objRot);
					entLeft->assign<MeshComponent>("Textures/flat_normal.png", "Meshes/bullet.obj", "bullet");
					entLeft->assign<BulletComponent>(bulletSpawnPosLeft, userLookAt);
					entLeft->assign<CubeCollider>(2, 2, 2);

					// Spawn right bullet
					Entity* entRight = world->create();
					entRight->assign<Transform3D>(bulletSpawnPosRight, 5, objRot);
					entRight->assign<MeshComponent>("Textures/flat_normal.png", "Meshes/bullet.obj", "bullet");
					entRight->assign<BulletComponent>(bulletSpawnPosRight, userLookAt);
					entRight->assign<CubeCollider>(2, 2, 2);


				}
			}

		}
		else {
			firstClick = true;
			if (firstSpawn) {
				firstSpawn = false;
			}
		}
		t += deltaTime / 2.0f;
		if (t > delay) {
			Entity* ent = world->create();
			ent->assign<Transform3D>(glm::vec3(1000000, 1, 0.), 5);
			int random = rand() % 10 + 1;
			cout << "spawning new asteroid type "<< random << endl;
			switch (random) {
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
			}
			ent->assign<EnemyComponent>(1, counter,"Asteroid");
			ent->assign<CubeCollider>(25, 25, 25);/**/
			//counter++;
			t = 0;
		}

		if (t > delay / 2) {
			world->each<EnemyComponent>([&](Entity* shipEntity, ComponentHandle<EnemyComponent> enemyComp) {
				if (enemyComp->name != "Ship") return;
				int shipID = enemyComp->id;

				world->each<EnemyShipCanon>([&](Entity* ent, ComponentHandle<EnemyShipCanon> enemyCanon) {
					// Calculate direction vector from enemy cannon to user position
					glm::vec3 canonPos = ent->get<Transform3D>()->position;
					glm::vec3 cannonRot = ent->get<Transform3D>()->rotation;

					glm::mat4 rotationMatrix =
						glm::rotate(glm::mat4(1.0f), cannonRot.y, glm::vec3(0.0f, 1.0f, 0.0f)) *
						glm::rotate(glm::mat4(1.0f), cannonRot.x, glm::vec3(1.0f, 0.0f, 0.0f)) *
						glm::rotate(glm::mat4(1.0f), cannonRot.z, glm::vec3(0.0f, 0.0f, 1.0f));

					glm::vec3 forward = glm::normalize(glm::vec3(rotationMatrix * glm::vec4(0.0f, 0.0f, -1.0f, 0.0f)));
					glm::vec3 cannonLookAt = glm::normalize(forward);
					Entity* entLeft = world->create();
					entLeft->assign<Transform3D>(canonPos, 10, cannonRot);
					entLeft->assign<MeshComponent>("Textures/flat_normal.png", "Meshes/bullet.obj", "bullet");
					entLeft->assign<BulletComponent>(canonPos, -cannonLookAt);
					entLeft->assign<CubeCollider>(2, 2, 2);
					});

				});

		}
		
	}
}
